#pragma once

#include <type_traits>

// is_specialisation checks if a type T is a specialisation of a template

// take in any type and any templated type
// this is the catch-all part
template<typename, template<typename...> class>
struct is_specialisation : std::false_type {};

// partial template specialisation
// if anything matches is_specialisation<T<TArgs...>, T> then this is valid
template<template<typename...> class T, typename... TArgs>
struct is_specialisation<T<TArgs...>, T> : std::true_type {};

