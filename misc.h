#pragma once

#include <algorithm>
#include <iterator>
#include <functional>
#include <vector>
#include <utility>
#include <memory>
#include <type_traits>

// Unfortunately until we get proper reflection in C++ we have to make do with horrible
// macros. MEMBER_CHECKER will define structs for whether a given member 'member'
// exists. We can call the defined structs with HAS_MEMBER(class, struct)
#define MEMBER_CHECKER(member)                                                         \
	template<typename T, typename = void>                                              \
	struct has_ ## member                                                              \
		: std::false_type { };                                                         \
	                                                                                   \
	template<typename T>                                                               \
	struct has_ ## member <T, decltype(std::declval<T>(). ## member, void())>          \
		: std::true_type { };

#define HAS_MEMBER(classs, member) has_ ## member<classs>::value

// Define any members we want to check for below.
//MEMBER_CHECKER(Y);
//MEMBER_CHECKER(x);


// Given an enum class value, this returns the underlying value of that enum value.
template <typename T>
constexpr auto GetUnderlyingValueFromEnum(const T& t) -> typename std::underlying_type<T>::type
{
	return static_cast<typename std::underlying_type<T>::type>(t);
}


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
	/* pass in inserter/iterator here*/)
	-> typename UContainer::iterator&
{
	return UnsortedSetDifference(
		std::begin(container1), std::end(container1),
		std::begin(container2), std::end(container2),
		std::back_inserter(outputContainer));
}

// Given a list of T, generate all pairs of <T, T>
template <typename T>
constexpr std::vector<std::pair<T, T>> GeneratePairwisePermutations(std::initializer_list<T> initList)
{
	std::vector<std::pair<T, T>> permutations;

	for (const auto& a : initList)
	{
		for (const auto& b : initList)
		{
			permutations.emplace_back(a, b);
		}
	}

	return permutations;
}


// std::hash specialisation for std::pair<A,B> since C++ doesn't specialise it,
// and I don't want to include boost for this sole use case. I've copied out
// boost's hash_combine which chains hashes together. It assumes there is a
// std::hash<A> and std::hash<B> defined somewhere - otherwise a compiler error
// is generated.

template <typename T, typename U>
inline size_t HashCombine(const T& t, const U& u)
{
	std::hash<T> tHasher;
	std::hash<U> uHasher;
	return (tHasher(t) ^ uHasher(u)) + 0x9e3779b9 + (tHasher(t) << 6) + (tHasher(t) >> 2);
}

inline size_t HashCombine(std::size_t hash1, std::size_t hash2)
{
	return (hash1 ^ hash2) + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
}

namespace std
{
	// for std::pair<T, U>
	template <typename T, typename U>
	struct hash<std::pair<T, U>>
	{
		size_t operator()(const std::pair<T, U>& tu) const
		{
			//std::hash<T> tHasher;
			//std::hash<U> uHasher;
			// '0' is the seed, and its fine to hardcode it to 0
			//return HashCombine(tHasher(tu.first), uHasher(tu.second));
			return HashCombine(tu.first, tu.second);
		}
	};
}

// constexpr pow function.
// beware of overflow of size_t - compiler won't warn about it
constexpr size_t ipow(size_t num, size_t pow)
{
	return pow == 0 ? 1 : num * ipow(num, pow - 1);
}

// templated constexpr pow function
//template <typename T>
//constexpr T ipow(T num, T pow)
//{
//	static_assert(std::is_integral<T>::value &&  std::is_arithmetic<T>::value, "T isn't integral and arithmetic");
//	return pow == 0 ? 1 : num * ipow(num, pow - 1);
//}
