#include "any.h"
#include <string>

namespace utils {
[[nodiscard]] const char *bad_any_cast::what() const noexcept {
    return "Can't cast";
}

[[nodiscard]] bool any::empty() const noexcept {
    return this->m_value == nullptr;
}
}  // namespace utils
