
#pragma region FoldExpressions

// Until we get fold expressions in C++17 (http://en.cppreference.com/w/cpp/language/fold)
// we'll use a 'poor-mans fold expression' which is just variadic template recursion and expansion

// base case for the below
bool fold_and(bool b)
{
    return b;
}

// left fold over logical and (&&)
// uses variadic template recursion+expansion
template<typename... Args>
bool fold_logical_and_left(bool b, Args... args)
{
    return b && fold_and(args...);
}

// right fold over logical and (&&)
// uses variadic template recursion+expansion
template<typename... Args>
bool fold_logical_and_right(bool b, Args... args)
{
    return fold_and(args...) && b;
}

// base case for the below
template <typename T>
bool unary_fold_over(T t)
{ return t; }

// left fold over logical and (&&)
// uses variadic template recursion+expansion
template<typename Functor, typename T, typename... TArgs>
auto unary_fold_over(T t, TArgs... args) -> typename std::result_of<Functor(T, T)>::type
{
    return Functor()(t, unary_fold_over(args...));
}



#pragma endregion
