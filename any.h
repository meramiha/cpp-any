#ifndef ANY_H
#define ANY_H
#include <string>
#include <typeinfo>

namespace utils {
    class any_storage_base {
    public:
        virtual void *get_pointer() = 0;

        virtual ~any_storage_base() = 0;
    };

    template<typename T>
    class any_storage_derived : public any_storage_base {
        T m_ptr;

    public:
        // explicit any_storage_derived(T *ptr);

        explicit any_storage_derived(T obj): m_ptr(obj) {
        }

        void *get_pointer() override {
            return static_cast<void *>(&this->m_ptr);
        }
        // explicit any_storage_derived(any_storage_derived const& other){
        //     this->m_ptr = other.m_ptr;
        //
        // }

        ~any_storage_derived() override {
            // delete m_ptr;
        }
    };

    class any {
        // void *m_value = nullptr;

        any_storage_base *m_value = nullptr;
        std::string m_type;

    public:
        any() = default;

        // any(const any &other) = default;

        // any &operator=(const any &other) = default;

        template<typename T>
        explicit any(T const &value): m_value(new any_storage_derived<T>(value)),
                                      m_type(typeid(T).name()) {
        }

        template<typename T>
        any &operator=(T const &value) {
            // delete m_value;

            m_value = new any_storage_derived<T>(value);
            m_type = typeid(T).name();

            return *this;
        }

        [[nodiscard]] bool empty() const noexcept;

        [[nodiscard]] std::string to_string() const noexcept;

        template<typename T>
        friend T *any_cast(any *object);

        template<typename T>
        friend T any_cast(any &object);

        ~any();
    };

    class bad_any_cast final : std::bad_cast {
    public:
        [[nodiscard]] const char *what() const noexcept override;
    };

    template<typename T>
    T *any_cast(any *object) {
        if (object->m_type != typeid(T).name()) {
            return nullptr;
        }

        return static_cast<T *>(object->m_value->get_pointer());
    }

    template<typename T>
    T any_cast(any &object) {
        if (object.m_type != typeid(T).name()) {
            throw bad_any_cast();
        }

        return *static_cast<T *>(object.m_value->get_pointer());
    }
}

#endif //ANY_H
