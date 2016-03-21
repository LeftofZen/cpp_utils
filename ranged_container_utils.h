
#include <algorithm>
#include <functional>
#include <vector>

#pragma region RangedForEach

/*
Templated over container type itself rather than the internal type
This method is much cleaner than the above.
*/

// Vectors, sets, deques, maps by key-pair
template <typename T>
void RangedForEach(const T container, std::function<void(typename T::value_type&)> fn)
{
	std::for_each(container.begin(), container.end(), fn);
}

// Maps - values only
template <typename T>
void RangedForEachValue(const T container, std::function<void(typename T::mapped_type&)> fn)
{
	std::for_each(container.begin(), container.end(), [fn] (typename T::mapped_type& v) { fn(v.second); });
}

#pragma endregion

#pragma region RangedFind

/*
Templated over container type itself rather than the internal type
This method is much cleaner than the above.
*/

// T should be an iterable container. This simplifies searching for a value in a
// container by hiding the iterators from the user.
// Not sure how to simplify this further (ie use one function instead of 2)
// This is for maps, where value_type is a pair<key_type, mapped_type> so we need to use mapped_type instead
// We also need to use std::find_if because std::find expects a T::value_type, not T::mapped_type
// This is for maps, where value_type is a pair<key_type, mapped_type> so we need to use mapped_type instead
template <typename T>
bool RangedFindValue(const T& container, const typename T::mapped_type& val)
{
	return std::find_if
	(
		std::begin(container),
		std::end(container),
		[val] (const typename T::value_type& m) { return val == m.second; }
	)
	!= std::end(container);
}

// This is for maps by key
template <typename T>
bool RangedFindKey(const T& container, const typename T::key_type& val)
{
	return std::find_if
	(
		std::begin(container),
		std::end(container),
		[val] (const typename T::value_type& m) { return val == m.first; }
	)
	!= std::end(container);
}

// This is for things like queues, sets and vectors
template <typename T>
bool RangedFind(const T& container, const typename T::value_type& val)
{
	return std::find(std::begin(container), std::end(container), val) != std::end(container);
}

#pragma endregion
