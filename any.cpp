#include "any.h"
#include <boost/core/demangle.hpp>
#include <boost/format.hpp>
#include <string>
#include <typeinfo>
namespace utils {
[[nodiscard]] const char *bad_any_cast::what() const noexcept {
    return message.c_str();
}
// NOLINTNEXTLINE (bugprone-easily-swappable-parameters)
bad_any_cast::bad_any_cast(const std::type_info &from_type,
                           const std::type_info &to_type)
    : message((boost::format("bad any_cast: can't cast %1% to %2%") %
               boost::core::demangle(from_type.name()) %
               boost::core::demangle(to_type.name()))
                  .str()) {
}

[[nodiscard]] bool any::empty() const noexcept {
    return this->m_value == nullptr;
}
}  // namespace utils
