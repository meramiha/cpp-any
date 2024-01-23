#include <string>
#include "any.h"

namespace utils {
    [[nodiscard]] const char *bad_any_cast::what() const noexcept {
        return "";
    }


    [[nodiscard]] std::string any::to_string() const noexcept {
        return m_type;
    }

    [[nodiscard]] bool any::empty() const noexcept {
        return this->m_value == nullptr;
    }
}
