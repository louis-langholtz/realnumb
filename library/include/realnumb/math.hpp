#ifndef REALNUMB_MATH_HPP
#define REALNUMB_MATH_HPP

/// @file
/// @brief Generic math functions.

namespace realnumb {

/// @brief Computes the absolute value of the value given.
/// @see https://en.cppreference.com/w/cpp/numeric/math/fabs
template <class T>
constexpr auto abs(T arg) -> decltype(((arg < T())? -arg: arg), T{})
{
    return (arg < T())? -arg: arg;
}

/// @brief Computes the factorial.
/// @see https://en.wikipedia.org/wiki/Factorial.
template <class T>
constexpr auto factorial(T n) -> decltype(--n, n > 1, n *= n, T{})
{
    // n! = n·(n – 1)·(n – 2) · · · 3·2·1
    auto res = n;
    for (--n; n > 1; --n)
    {
        res *= n;
    }
    return res;
}

}

#endif /* REALNUMB_MATH_HPP */
