#pragma once

#include "compiler_helpers_gcc.h"

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
