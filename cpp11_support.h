#pragma once

#include "compiler_helpers_gcc.h"

//#include <memory>
//#include <type_traits>

// std::unique_ptr
#if defined(GCC_VERSION) && GCC_VERSION < 40900

namespace std {


template <typename T, typename... TArgs>
typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(TArgs&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<TArgs>(args)...));
}

template <typename T>
typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0, std::unique_ptr<T>>::type
make_unique(std::size_t size)
{
	using arrType = typename std::remove_extent<T>::type;
	return std::unique_ptr<T>(new arrType[size]());
}


template<typename T, typename... TArgs>
typename std::enable_if<std::extent<T>::value != 0, void>::type
make_unique(TArgs&&... args) = delete;

}

#else
#pragma message "GCC is now compliant with std::make_unique (and std::make_shared); you can ignore this warning"
#endif


// From here: https://gcc.gnu.org/projects/cxx-status.html#cxx11
// Makes the attribute evaluate to a no-op if not supported
#if defined(__cpp_attributes) && __cpp_attributes >= 200809
#define CPP_11_ATTRIBUTE(x) x
#pragma message "C++11 attributes are supported by your compiler; you can remove usage of this macro"
#else
#define CPP_11_ATTRIBUTE(x)
#endif
