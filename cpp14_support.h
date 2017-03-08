#pragma once

#include "compiler_helpers_gcc.h"

// std::make_unque
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

// there is a defect in the standard that doesn't specify enum hashers. it's remedied
// in gcc 5.0 and msvc 2015 (v1900)
// for this hash we'll just use the actual enum value itself
#if (defined(GCC_VERSION) && GCC_VERSION < 50000) || (defined(_MSC_VER) && _MSC_VER < 1900)
struct EnumHasher
{
	template <typename T>
	std::size_t operator()(const T& t) const
	{
		// same as misc.h::GetUnderlyingTypeFromEnum, but including misc.h introduces circular dependencies
		return static_cast<typename std::underlying_type<T>::type>(t);
	}
};

#else
#warning "GCC/MSVC is now compliant with C++14 enum hashing; you don't need to explicitly specify EnumHasher as your container hasher any more"
#endif
