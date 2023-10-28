#ifndef REALNUMB_TAYLOR_SERIES_HPP
#define REALNUMB_TAYLOR_SERIES_HPP

/// @file
/// @brief Taylor series functions.
/// @see https://en.wikipedia.org/wiki/Taylor_series

#include <cassert> // for assert

#include <realnumb/numbers.hpp>
#include <realnumb/is_arithmetic.hpp> // for is_arithmetic_v
#include <realnumb/math.hpp> // for realnumb::abs

namespace realnumb::taylor_series {

/// @brief Computes Euler's number raised to the given power argument.
/// @note Uses Maclaurin approximation.
/// @see https://en.cppreference.com/w/cpp/numeric/math/exp
/// @see https://en.wikipedia.org/wiki/Exponentiation
/// @see https://en.wikipedia.org/wiki/Exponential_function
template <int N, class T>
constexpr auto exp(T arg) -> std::enable_if_t<is_arithmetic_v<T>, T>
{
    const auto doReciprocal = (arg < 0);
    if (doReciprocal)
    {
        arg = -arg;
    }
    // Maclaurin series approximation...
    // e^x = sum(x^n/n!) for n =0 to infinity.
    // e^x = 1 + x + x^2/2! + x^3/3! + ...
    // Note: e^(x+y) = e^x * e^y.
    // Note: convergence is slower for arg > 2 and overflow happens by i == 9
    auto res = arg + 1;
    auto last = arg;
    for (auto i = 2; (last != 0) && (i < N); ++i)
    {
        // have to avoid unnecessarily overflowing...
        last /= i; // for factorial
        last *= arg;
        res += last;
    }
    return doReciprocal? 1 / res: res;
}

/// @brief Computes the sine of the given angle via Maclaurin series approximation.
/// @param arg Angle in radians.
/// @note Most accurate for angles between −π/2 and π/2.
template <int N, class T>
constexpr auto sin(T arg) -> std::enable_if_t<is_arithmetic_v<T>, T>
{
    // Maclaurin series approximation...
    // sin x = sum((-1^n)*(x^(2n+1))/((2n+1)!))
    // sin(2) = 0.90929742682
    // x - (x^3)/6 + (x^5)/120 - (x^7)/5040 + (x^9)/362880 + (x^11)/39916800
    // 2 - 8/6 = 0.666
    // 2 - 8/6 + 32/120 = 0.9333
    // 2 - 8/6 + 32/120 - 128/5040 = 0.90793650793
    // 2 - 8/6 + 32/120 - 128/5040 + 512/362880 = 0.90934744268
    auto res = arg;
    auto sgn = -1;
    constexpr auto last = 2 * N + 1;
    auto pt = arg;
    auto ft = 1;
#ifndef NDEBUG
    auto last_ft = ft;
#endif
    auto last_term = T(7); // more than 2 Pi radian
    for (auto i = 3; i <= last; i += 2)
    {
        ft *= (i - 1) * i;
#ifndef NDEBUG
        assert(ft > last_ft); // confirm no overflow of ft
        last_ft = ft;
#endif
        pt *= arg * arg;
        const auto term = pt / ft;
        if (abs(term) >= abs(last_term)) {
            break;
        }
        last_term = term;
        res += sgn * term;
        sgn = -sgn;
    }
#ifndef NDEBUG
    assert(res >= T(-1));
    assert(res <= T(+1));
#endif
    return res;
}

/// @brief Computes the cosine of the given argument via Maclaurin series approximation.
/// @param arg Angle in radians.
template <int N, class T>
constexpr auto cos(T arg) -> std::enable_if_t<is_arithmetic_v<T>, T>
{
    // Maclaurin series approximation...
    // cos x = sum((-1^n)*(x^(2n))/(2n)!)
    // cos(2) = -0.41614683654
    // 1 - 2^2/2 = -1
    // 1 - 2^2/2 + 2^4/24 = -0.3333
    // 1 - 2^2/2 + 2^4/24 - 2^6/720 = -0.422
    auto res = T(1);
    auto sgn = -1;
    constexpr auto last = 2 * N;
    auto ft = 1;
    auto pt = T(1);
    auto last_term = T(7);
    for (auto i = 2; i <= last; i += 2)
    {
        ft *= (i - 1) * i;
        pt *= arg * arg;
        const auto term = pt / ft;
        if (abs(term) >= abs(last_term)) {
            break;
        }
        last_term = term;
        res += sgn * term;
        sgn = -sgn;
    }
#ifndef NDEBUG
    assert(res >= T(-1));
    assert(res <= T(+1));
#endif
    return res;
}

/// @brief Computes the arctangent of the given argument via Maclaurin series approximation.
/// @see https://en.cppreference.com/w/cpp/numeric/math/atan
template <int N, class T>
constexpr auto atan(T arg) -> std::enable_if_t<is_arithmetic_v<T>, T>
{
    // Note: if (x > 0) then arctan(x) ==  Pi/2 - arctan(1/x)
    //       if (x < 0) then arctan(x) == -Pi/2 - arctan(1/x).
    const auto doReciprocal = (abs(arg) > T(1));
    if (doReciprocal)
    {
        arg = 1 / arg;
    }
    // Maclaurin series approximation...
    // For |arg| <= 1, arg != +/- i
    // If |arg| > 1 the result is too wrong which is why the reciprocal is done then.
    auto res = arg;
    auto sgn = -1;
    const auto last = 2 * N + 1;
    auto pt = arg;
    for (auto i = 3; i <= last; i += 2)
    {
        pt *= arg * arg;
        const auto term = pt / i;
        res += sgn * term;
        sgn = -sgn;
    }
    if (doReciprocal)
    {
        return (arg > 0) // newline!
            ? +T(realnumb::numbers::pi) / 2 - res // newline!
            : -T(realnumb::numbers::pi) / 2 - res;
    }
    return res;
}

}

#endif /* REALNUMB_TAYLOR_SERIES_HPP */
