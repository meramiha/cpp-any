#ifndef ANY_H
#define ANY_H
#include <string>
#include <typeinfo>
#include <memory>

namespace utils {
    class any {
        class any_storage_base {
        public:
            virtual void *get_pointer() = 0;

            virtual ~any_storage_base() = default;

            virtual std::type_info const &get_type() = 0;

            virtual std::unique_ptr<any_storage_base> get_unique_pointer_to_copy() = 0;
        };

        template<typename T>
        class any_storage_derived final : public any_storage_base {
            T m_ptr;

        public:
            explicit any_storage_derived(T obj): m_ptr(obj) {
            }

            std::type_info const &get_type() override {
                return typeid(T);
            }

            void *get_pointer() override {
                return static_cast<void *>(&this->m_ptr);
            }

            std::unique_ptr<any_storage_base> get_unique_pointer_to_copy() override {
                return std::make_unique<any_storage_derived<T> >(m_ptr);
            }

            ~any_storage_derived() override = default;
        };


        std::unique_ptr<any_storage_base> m_value = nullptr;

    public:
        any() = default;

        template<typename T>
        explicit any(T value): m_value(any_storage_derived<T>(value).get_unique_pointer_to_copy()) {
            static_assert(std::is_copy_constructible_v<T>);
        }


        template<typename T>
        any &operator=(T value) {
            this->m_value.reset();
            m_value = any_storage_derived<T>(value).get_unique_pointer_to_copy();

            return *this;
        }

        any(any const &other) {
            this->m_value.reset();
            this->m_value = other.m_value->get_unique_pointer_to_copy();
        }

        any &operator=(any const &other) {
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

        template<typename T>
        friend T *any_cast(any *object);

        template<typename T>
        friend T any_cast(any &object);

        ~any() = default;
    };

    class bad_any_cast final : std::bad_cast {
    public:
        [[nodiscard]] const char *what() const noexcept override;
    };

    template<typename T>
    T *any_cast(any *object) {
        if (object->m_value->get_type() != typeid(T)) {
            return nullptr;
        }

        return static_cast<T *>(object->m_value->get_pointer());
    }

    template<typename T>
    T any_cast(any &object) {
        if (object.m_value->get_type() != typeid(T)) {
            throw bad_any_cast();
        }

        return *static_cast<T *>(object.m_value->get_pointer());
    }
}

#endif //ANY_H
