#include <string>
#include "any.h"

namespace utils {
    [[nodiscard]] const char *bad_any_cast::what() const noexcept {
        return "";
    }


    [[nodiscard]] std::string any::to_string() const noexcept {
        return m_type;
    }

    // template<typename T>
    // any::any(T const &value): m_value(static_cast<void *>(&value)), m_type(typeid(T).name()) {
    // }

    // template<typename T>
    // any &any::operator=(T const &value) {
    //     m_value = static_cast<void *>(&value);
    //     m_type = typeid(T).name();
    //
    //     return *this;
    // }

    [[nodiscard]] bool any::empty() const noexcept {
        return this->m_value == nullptr;
    }

    // template<typename T>
    // T *any_cast(any *a) {
    //     if (a->m_type != typeid(T).name()) {
    //         return nullptr;
    //     }
    //
    //     return static_cast<T *>(a->m_value);
    // }

    // template<typename T>
    // T any_cast(any &a) {
    //     if (a.m_type != typeid(T).name()) {
    //         throw bad_any_cast();
    //     }
    //
    //     return static_cast<T *>(a.m_value);
    // }

    // any &any::operator=(const any &other) {
    //     this->m_value = other.m_value;
    //     this->m_value = other.m_value;
    //
    //     return *this;
    // }

    any::~any() {
        // delete m_value;
    }



    // template<typename T>
    // any_storage_derived<T>::~any_storage_derived() {
    //     delete m_ptr;
    // }

    // template<typename T>
    // void * any_storage_derived<T>::get_pointer() {
    //     return static_cast<void*>(this->m_ptr);
    // }

    any_storage_base::~any_storage_base(){}
}


// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
