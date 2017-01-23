#pragma once

#include <algorithm>
#include <functional>
#include <vector>

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
	RangedForEach(container, [fn] (typename T::mapped_type& v) { fn(v.second); });
}

// Maps - keys only
template <typename T>
void RangedForEachKey(const T container, std::function<void(typename T::key_type&)> fn)
{
	RangedForEach(container, [fn] (typename T::mapped_type& v) { fn(v.first); });
}


/**
 * Templated over container type itself rather than the internal type (which results in much more complicated code
 * T should be an iterable container. This simplifies searching for a value in a
 * container by hiding the iterators from the user.
 **/

// This is for maps, where value_type is a pair<key_type, mapped_type> so we need to use mapped_type instead
// We also need to use std::find_if because std::find expects a T::value_type, not T::mapped_type
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


// This is for things like queues, sets and vectors
// It tries to find an item 'val'
template <typename T>
const typename T::mapped_type& RangedFindValueWithDefault(
	const T& container,
	const typename T::key_type& val,
	const typename T::mapped_type& to_return_if_val_not_found)
{
	auto find_result = container.find(val);
	return ((find_result == std::end(container)) ? to_return_if_val_not_found : find_result->second);
}


// This is a predicated find for things like queues, sets and vectors
template <typename T, typename UnaryPredicate>
bool RangedFindIf(const T& container, UnaryPredicate pred)
{
	return std::find_if(std::begin(container), std::end(container), pred) != std::end(container);
}

// TODO: add other predicated finds
