#pragma once

#include <string_view>

namespace minidrive {

inline constexpr std::string_view version() noexcept {
    return "0.1.0";
}

const char* resolved_version();

} // namespace minidrive
