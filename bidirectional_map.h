#pragma once

#include "ranged_container_utils.h"
#include <set>
#include <utlity>

template <typename T, typename U>
struct BiDirectionalMap
{
public:
	BiDirectionalMap(std::initializer_list<std::pair<T, U>> init)
	{
		for (const auto& item : init)
		{
			insert(item._First, item.second);
		}
	}

private:
	// internal
	struct RightNode;

	struct LeftNode
	{
		T& data;
		RightNode* mapping;
	};

	struct RightNode
	{
		U& data;
		LeftNode* mapping;
	};

	std::set<LeftNode> left;
	std::set<RightNode> right;

public:
	bool insert(T& t, U& u)
	{
		// if data already exists
		if (RangedFind(left, t) || RangedFind(right, u))
		{ return false; }

		// set up mapping between sets
		LeftNode lNode { t, nullptr };
		RightNode rNode {u, &lNode };
		lNode.mapping = &rNode;

		// add the nodes to the sets
		left.insert(lNode);
		right.insert(rNode);
	}

	U* Find(const T& t) const
	{
		if (RangedFind(left, t))
		{
			return &(left[t].mapping->data);
		}
		return nullptr;
	}

	T* Find(const U& u) const
	{
		if (RangedFind(right, u))
		{
			return &(right[u].mapping->data);
		}
		return nullptr;
	}

};
