#pragma once

#include <algorithm>
#include <iterator>
#include <functional>

#pragma region Misc

// Modified from http://en.cppreference.com/w/cpp/algorithm/set_difference
// std::set_difference requires both input containers to be sorted and produces a sorted output container
// This is nice until you get an unsorted container where you want to maintain the ordering. ie you want a stable
// version of the algorithm. This algorithm is that; a stable std::set_difference
// Should be O(n*m).
template<class InputIt1, class InputIt2, class OutputIt>
inline OutputIt UnsortedSetDifference(
	InputIt1 first1, InputIt1 last1,
	InputIt2 first2, InputIt2 last2,
	OutputIt d_first)
{
	while (first1 != last1)
	{
		if (first2 == last2) return std::copy(first1, last1, d_first);

		// if *first1 isn't in [first2, last2]
		if (std::find(first2, last2, *first1) != last2)
		{
			// chuck it in the output and move the iterators
			*d_first++ = *first1++;
		}
		else
		{
			// can find it, so just move first2 along
			++first2;
		}
	}
	return d_first;
}


// Ranged version of the above (UnsortedSetDifference)
// However it's not ready for prime time since I don't know how to let the user
// specify an output iterator at the same time as defaulting it to std::back_inserter(outputContainer)

// Returns an iterator pointing to the end of outputContainer
template<typename TContainer, typename UContainer>
inline auto RangedUnsortedSetDifference(
	const TContainer& container1,
	const TContainer& container2,
	const UContainer& outputContainer
    /* pass in inserter/iterator here*/) -> decltype(typename UContainer::iterator)
{
	return = UnsortedSetDifference(
		std::begin(container1), std::end(container1),
		std::begin(container2), std::end(container2),
		std::back_inserter(outputContainer));
}

#pragma endregion
