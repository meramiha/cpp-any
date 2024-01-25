#ifndef ANY_H
#define ANY_H

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace utils {
class any {
    // NOLINTNEXTLINE (cppcoreguidelines-special-member-functions)
    class any_storage_base {
    public:
        virtual void *get_pointer() = 0;

        virtual ~any_storage_base() = default;

        virtual std::type_info const &get_type() = 0;

        virtual std::unique_ptr<any_storage_base>
        get_unique_pointer_to_copy() = 0;
    };

    template <typename T>
    // NOLINTNEXTLINE (cppcoreguidelines-special-member-functions)
    class any_storage_derived final : public any_storage_base {
        T m_obj;

    public:
        explicit any_storage_derived(T obj) : m_obj(obj) {
        }

        std::type_info const &get_type() override {
            return typeid(T);
        }

        void *get_pointer() override {
            return static_cast<void *>(&this->m_obj);
        }

        std::unique_ptr<any_storage_base> get_unique_pointer_to_copy()
            override {
            return std::make_unique<any_storage_derived<T> >(m_obj);
        }

        ~any_storage_derived() override = default;
    };

    std::unique_ptr<any_storage_base> m_value = nullptr;

public:
    any() = default;

    template <typename T>
    explicit any(T value)
        : m_value(any_storage_derived<T>(value).get_unique_pointer_to_copy()) {
        static_assert(std::is_copy_constructible_v<T>);
    }

    template <typename T>
    any &operator=(T value) {
        this->m_value.reset();
        m_value = any_storage_derived<T>(value).get_unique_pointer_to_copy();

        return *this;
    }

    any(const any &other) {
        this->m_value.reset();
        this->m_value = other.m_value->get_unique_pointer_to_copy();
    }

    any &operator=(const any &other) {
        this->m_value.reset();
        this->m_value = other.m_value->get_unique_pointer_to_copy();

        return *this;
    }

    any(any &&other) noexcept : m_value(std::move(other.m_value)) {
    }

    any &operator=(any &&other) noexcept {
        m_value = std::move(other.m_value);

        return *this;
    }

    [[nodiscard]] bool empty() const noexcept;

    template <typename T>
    friend const T *any_cast(const any *object) noexcept;

    template <typename T>
    friend T *any_cast(any *object) noexcept;

    ~any() = default;
};

class bad_any_cast final : std::bad_cast {
public:
    [[nodiscard]] const char *what() const noexcept override;
};

template <typename ValueType>
const ValueType *any_cast(const any *object) noexcept {
    if (object->m_value->get_type() != typeid(ValueType)) {
        return nullptr;
    }

    return static_cast<const ValueType *>(object->m_value->get_pointer());
}

template <typename ValueType>
ValueType *any_cast(any *object) noexcept {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-const-cast)
    return const_cast<ValueType *>(
        any_cast<ValueType>(static_cast<const any *>(object)));
}

template <typename T>
T any_cast(const any &object) {
    using raw_T = std::remove_reference_t<T>;

    auto ptr = any_cast<raw_T>(&object);

    if (ptr == nullptr) {
        throw bad_any_cast();
    }

    return static_cast<T>(*ptr);
}

template <typename T>
T any_cast(any &object) {
    using raw_T = std::remove_reference_t<T>;

    auto ptr = any_cast<raw_T>(&object);

    if (ptr == nullptr) {
        throw bad_any_cast();
    }

    return static_cast<T>(*ptr);
}

template <typename T>
// NOLINTNEXTLINE (cppcoreguidelines-rvalue-reference-param-not-moved)
T any_cast(any &&object) {
    using raw_T = std::remove_reference_t<T>;
    auto ptr = any_cast<raw_T>(&object);

    if (ptr == nullptr) {
        throw bad_any_cast();
    }

    return static_cast<T>(*ptr);
}

}  // namespace utils

#endif  // ANY_H
