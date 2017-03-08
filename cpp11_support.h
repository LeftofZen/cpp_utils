#pragma once

#include "compiler_helpers_gcc.h"

// From here: https://gcc.gnu.org/projects/cxx-status.html#cxx11
// Makes the attribute evaluate to a no-op if not supported
#if defined(__cpp_attributes) && __cpp_attributes >= 200809
#define CPP_11_ATTRIBUTE(x) x
#pragma message "C++11 attributes are supported by your compiler; you can remove usage of this macro"
#else
#define CPP_11_ATTRIBUTE(x)
#endif
