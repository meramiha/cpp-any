#ifndef ANY_H
#define ANY_H
#include <string>
#include <typeinfo>

namespace utils {
    class any {
        class any_storage_base {
        public:
            virtual void *get_pointer() = 0;

            virtual ~any_storage_base() = default;

            virtual std::unique_ptr<any_storage_base> copy_data() = 0;
        };

        template<typename T>
        class any_storage_derived final : public any_storage_base {
            T m_ptr;

        public:
            explicit any_storage_derived(T obj): m_ptr(obj) {
            }

            void *get_pointer() override {
                return static_cast<void *>(&this->m_ptr);
            }

            std::unique_ptr<any_storage_base> copy_data() override {
                return std::make_unique<any_storage_derived<T> >(m_ptr);
            }

            ~any_storage_derived() override = default;
        };


        std::unique_ptr<any_storage_base> m_value = nullptr;
        std::string m_type;

    public:
        any() = default;

        template<typename T>
        explicit any(T value): m_value(any_storage_derived<T>(value).copy_data()),
                               m_type(typeid(T).name()) {
            static_assert(std::is_copy_constructible_v<T>);
        }


        template<typename T>
        any &operator=(T value) {
            // delete this->m_value;

            this->m_value.reset();

            // NOLINTNEXTLINE
            m_value = any_storage_derived<T>(value).copy_data();
            m_type = typeid(T).name();

            return *this;
        }

        any(any const &other) {
            this->m_value.reset();

            this->m_value = other.m_value->copy_data();

            this->m_type = other.m_type; // NOLINT
        }

        any &operator=(any const &other) {
            this->m_value.reset();

            this->m_value = other.m_value->copy_data();

            this->m_type = other.m_type; // NOLINT

            return *this;
        }

        any(any &&other) noexcept : m_value(std::move(other.m_value)), m_type(std::move(other.m_type)) {
        }

        any &operator=(any &&other) noexcept {
            m_value = std::move(other.m_value);
            m_type = std::move(other.m_type);

            return *this;
        }

        [[nodiscard]] bool empty() const noexcept;

        [[nodiscard]] std::string to_string() const noexcept;

        template<typename T>
        friend T *any_cast(any *object);

        template<typename T>
        friend T any_cast(any &object);

        ~any() =default;
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
