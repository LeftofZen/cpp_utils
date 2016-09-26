
#include <functional>

#pragma region FoldExpressions

struct fold_base
{
	template <typename T>
	static T fold(T arg)
	{
		return arg;
	};
};

template <typename UnaryOp>
struct fold_left_trait : public fold_base
{
	// bring in the recursive base case from the parent
	using fold_base::fold;

	template <typename T, typename... TArgs>
	static T fold(T arg, TArgs... args)
	{
		static UnaryOp sUnaryOp;
		return sUnaryOp(fold(args...), arg);
	}
};

template <typename UnaryOp>
struct fold_right_trait : public fold_base
{
	// bring in the recursive base case from the parent
	using fold_base::fold;

	template <typename T, typename... TArgs>
	static T fold(T arg, TArgs... args)
	{
		static UnaryOp sUnaryOp;
		return sUnaryOp(arg, fold(args...));
	}
};

template <typename TFoldDirection, typename T, typename... TArgs>
T fold(T arg, TArgs... args)
{
	return TFoldDirection::fold(arg, args...);
}

#pragma endregion

template <typename T, typename... TArgs>
T all(T arg, TArgs... args)
{
	return fold<fold_left_trait<std::logical_and<bool>>>(arg, args...);
}


//int main(int argc, char** argv)
//{
//	//bool result = fold<fold_left_trait<std::logical_and<bool>>>(true, true, false, true);
//	bool result = all(true, true, false, true);
//
//	//std::cout << std::boolalpha << result << std::endl;
//
//	return 0;
//}
