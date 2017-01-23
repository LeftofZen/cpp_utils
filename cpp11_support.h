#pragma once

#include "compiler_helpers_gcc.h"


// std::unique_ptr
#if defined(GCC_VERSION) && GCC_VERSION < 40900

namespace std {

// There exist 2 more overloads for make_unique, but we aren't using them so
// I won't bother adding them.
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}

#else
#message "GCC is now compliant with std::make_unique (and std::make_shared); you can ignore this warning"
#endif


// From here: https://gcc.gnu.org/projects/cxx-status.html#cxx11
+#if defined(__cpp_attributes) && __cpp_attributes >= 200809
#define Cpp11Attribute(x) x
#else
#define Cpp11Attribute(x)
#endif
