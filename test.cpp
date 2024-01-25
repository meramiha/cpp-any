#include <any>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include "any.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using utils::any;
using utils::any_cast;
using utils::bad_any_cast;

void contruct_test() {
    any def;
    any copy_on_type(42);
    def = 3.14;
    def = string("2.71");
    any def_copy(def);
    def = copy_on_type;
    any e;
    assert(e.empty());
}

template <class T>
void check_cast(any &a, bool should_throw) {
    bool thrown = false;
    try {
        double res = any_cast<T>(a);
        std::cout << res;
    } catch (bad_any_cast const &err) {
        thrown = true;
        std::cerr << err.what() << std::endl;
    }
    assert(should_throw == thrown);
}

void retrieve_value_test() {
    any ia(42);
    auto res = any_cast<double>(&ia);
    assert(res == nullptr);
    check_cast<double>(ia, true);
    check_cast<int>(ia, false);
}

void swap_test(any &a, any &b) {
    std::swap(a, b);
}

void my_empty_test() {
    any a;
    assert(a.empty());

    a = 14;

    assert(!a.empty());
}

void my_pointer_test() {
    int obj = 1;

    int *ptr = &obj;

    any a(ptr);

    int *h = new int(1);

    any b(h);

    any c(*h);

    delete h;
}

void my_value_test() {
    double k = 2.39;

    any a(k);

    assert(k == any_cast<double>(a));
    assert(0.0 != any_cast<double>(a));
    int i = 155;
    a = i;

    assert(i == any_cast<int>(a));

    a = 4;
    assert(4 == any_cast<int>(a));
    assert(5 != any_cast<int>(a));
}

void my_non_trivially_copyable_test() {
    std::string s =
        "my really "
        "looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
        "ooooooooooooooooooooooooooooong string";

    std::string same_string =
        "my really "
        "looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
        "ooooooooooooooooooooooooooooong string";
    any a(s);

    static_assert(!std::is_trivially_copyable_v<std::string>);
    assert(same_string == any_cast<std::string>(a));
}

void my_custom_class_test() {  // this test only make sense with valgrind
    class B {
        int *ptr = nullptr;

    public:
        explicit B(const int value) : ptr(new int(value)) {
        }

        B &operator=(const B &other) {
            std::cerr << "B &operator=(const B &other) called" << std::endl;
            if (this == &other) {
                return *this;
            }
            delete ptr;
            ptr = new int(*other.ptr);
            return *this;
        }

        B &operator=(B &&other) noexcept {
            std::cerr << "B &operator=(B &&other) called" << std::endl;
            std::swap(this->ptr, other.ptr);
            return *this;
        }

        B(B const &other) : ptr(new int(*other.ptr)) {
            std::cerr << "B(B const &other) called" << std::endl;
        }

        B(B &&other) noexcept : ptr(other.ptr) {
            std::cerr << "B(B &&other) called" << std::endl;
            other.ptr = nullptr;
        }

        ~B() {
            delete ptr;

            std::cerr << "~B() called" << std::endl;
        }
    };

    static_assert(std::is_copy_constructible_v<B>);

    any a{B(0)};

    a = 1;

    B b = B(1);

    any a2{b};

    B b2 = B(1);
    any a3{std::move(b2)};
}

void my_const_test() {
    static_assert(
        std::is_same_v<const int &, decltype(any_cast<const int &>(any(1)))>);

    static_assert(std::is_same_v<int &, decltype(any_cast<int &>(any(1)))>);

    static_assert(std::is_same_v<int, decltype(any_cast<int>(any(1)))>);

    any a(0);

    static_assert(
        std::is_same_v<const int &, decltype(any_cast<const int &>(a))>);

    static_assert(std::is_same_v<int &, decltype(any_cast<int &>(a))>);

    static_assert(std::is_same_v<int, decltype(any_cast<int>(a))>);

    static_assert(std::is_same_v<int *, decltype(any_cast<int>(&a))>);

    any const_a(0);
    static_assert(
        std::is_same_v<const int *, decltype(any_cast<const int>(&const_a))>);
}

int main() {
    contruct_test();
    retrieve_value_test();
    any a(5), b(string("6"));
    swap_test(a, b);

    assert(any_cast<std::string>(a) == "6");
    assert(any_cast<int>(b) == 5);

    // my tests
    my_empty_test();
    my_pointer_test();
    my_value_test();
    my_non_trivially_copyable_test();
    my_custom_class_test();

    my_const_test();

    return 0;
}
