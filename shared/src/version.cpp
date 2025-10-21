#include "minidrive/version.hpp"

namespace minidrive {

const char* resolved_version() {
    return version().data();
}

} // namespace minidrive
