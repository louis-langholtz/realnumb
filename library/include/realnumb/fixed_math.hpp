#ifndef REALNUMB_FIXEDMATH_HPP
#define REALNUMB_FIXEDMATH_HPP

/// @file
/// @brief Conventional math functions for the @c fixed class template.

#include <cmath>

#include <realnumb/fixed.hpp>

namespace realnumb {

namespace detail {

/// @brief fixed point pi value.
template <typename BT, unsigned int FB>
constexpr auto FixedPi = fixed<BT, FB>{3.14159265358979323846264338327950288};

/// @brief Default iterations for @c exp function.
constexpr auto DefaultExpIterations = 6;

/// @brief Default iterations for @c log function.
constexpr auto DefaultLogIterations = 6;

/// @brief Default iterations for @c sin function.
constexpr auto DefaultSinIterations = 5;

/// @brief Default iterations for @c cos function.
constexpr auto DefaultCosIterations = 5;

/// @brief Default iterations for @c atan function.
constexpr auto DefaultAtanIterations = 5;

/// @brief Computes the factorial.
constexpr auto factorial(std::int64_t n)
{
    // n! = n·(n – 1)·(n – 2) · · · 3·2·1
    auto res = n;
    for (--n; n > 1; --n)
    {
        res *= n;
    }
    return res;
}

/// @brief Computes Euler's number raised to the given power argument.
/// @note Uses Maclaurin series approximation.
/// @see https://en.cppreference.com/w/cpp/numeric/math/exp
/// @see https://en.wikipedia.org/wiki/Taylor_series
/// @see https://en.wikipedia.org/wiki/Exponentiation
/// @see https://en.wikipedia.org/wiki/Exponential_function
template <typename BT, unsigned int FB, int N = DefaultExpIterations>
constexpr fixed<BT, FB> exp(fixed<BT, FB> arg)
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
    auto pt = arg;
    auto res = pt + 1;
    auto ft = 1;
    auto last = pt / ft;
    for (auto i = 2; i < N; ++i)
    {
        // have to avoid unnecessarily overflowing...
        last /= i;
        last *= arg;
        res += last;
    }
    return doReciprocal? 1 / res: res;
}

/// @brief Computes the natural logarithm.
/// @note A better method may be explained in https://math.stackexchange.com/a/61236/408405
/// @see https://en.cppreference.com/w/cpp/numeric/math/log
/// @see https://en.wikipedia.org/wiki/Natural_logarithm
template <typename BT, unsigned int FB, int N = DefaultLogIterations>
fixed<BT, FB> log(fixed<BT, FB> arg)
{
    if (arg.isnan() || (arg < 0))
    {
        return fixed<BT, FB>::GetNaN();
    }
    if (arg == 0)
    {
        return fixed<BT, FB>::GetNegativeInfinity();
    }
    if (arg == 1)
    {
        return fixed<BT, FB>{0};
    }
    if (arg == fixed<BT, FB>::GetInfinity())
    {
        return fixed<BT, FB>::GetInfinity();
    }
    if (arg <= 2)
    {
        // ln(x) = sum((-1)^(n + 1) * (x - 1)^n / n) from n = 1 to infinity
        // ln(x) = (x - 1) - (x - 1)^2/2 + (x - 1)^3/3 - (x - 1)^4/4 ....
        arg -= 1;
        auto res = arg;
        auto sign = -1;
        auto pt = arg;
        for (auto i = 2; i < N; ++i)
        {
            pt *= arg;
            res += sign * pt / i;
            sign = -sign;
        }
        return res;
    }
    // The following algorithm isn't as accurate as desired.
    // Is there a better one?
    // ln(x) = ((x - 1) / x) + ((x - 1) / x)^2/2 + ((x - 1) / x)^3/3 + ...
    arg = (arg - 1) / arg;
    auto pt = arg;
    auto res = pt;
    for (auto i = 2; i < N; ++i)
    {
        pt *= arg;
        res += pt / i;
    }
    return res;
}

/// @brief Computes the sine of the given argument via Maclaurin series approximation.
/// @see https://en.wikipedia.org/wiki/Taylor_series
template <typename BT, unsigned int FB, int N = DefaultSinIterations>
constexpr fixed<BT, FB> sin(fixed<BT, FB> arg)
{
    // Maclaurin series approximation...
    // sin x = sum((-1^n)*(x^(2n+1))/(2n+1)!)
    // sin(2) = 0.90929742682
    // x - x^3/6 + x^5/120 - x^7/5040 + x^9/
    // 2 - 8/6 = 0.666
    // 2 - 8/6 + 32/120 = 0.9333
    // 2 - 8/6 + 32/120 - 128/5040 = 0.90793650793
    // 2 - 8/6 + 32/120 - 128/5040 + 512/362880 = 0.90934744268
    auto res = arg;
    auto sgn = -1;
    constexpr auto last = 2 * N + 1;
    auto pt = arg;
    auto ft = 1;
    for (auto i = 3; i <= last; i += 2)
    {
        ft *= (i - 1) * i;
        pt *= arg * arg;
        const auto term = pt / ft;
        res += sgn * term;
        sgn = -sgn;
    }
    return res;
}

/// @brief Computes the cosine of the given argument via Maclaurin series approximation.
/// @see https://en.wikipedia.org/wiki/Taylor_series
template <typename BT, unsigned int FB, int N = DefaultCosIterations>
constexpr fixed<BT, FB> cos(fixed<BT, FB> arg)
{
    // Maclaurin series approximation...
    // cos x = sum((-1^n)*(x^(2n))/(2n)!)
    // cos(2) = -0.41614683654
    // 1 - 2^2/2 = -1
    // 1 - 2^2/2 + 2^4/24 = -0.3333
    // 1 - 2^2/2 + 2^4/24 - 2^6/720 = -0.422
    auto res = fixed<BT, FB>{1};
    auto sgn = -1;
    constexpr auto last = 2 * N;
    auto ft = 1;
    auto pt = fixed<BT, FB>{1};
    for (auto i = 2; i <= last; i += 2)
    {
        ft *= (i - 1) * i;
        pt *= arg * arg;
        const auto term = pt / ft;
        res += sgn * term;
        sgn = -sgn;
    }
    return res;
}

/// @brief Computes the arctangent of the given argument via Maclaurin series approximation.
/// @see https://en.cppreference.com/w/cpp/numeric/math/atan
/// @see https://en.wikipedia.org/wiki/Taylor_series
template <typename BT, unsigned int FB, int N = DefaultAtanIterations>
constexpr fixed<BT, FB> atan(fixed<BT, FB> arg)
{
    // Note: if (x > 0) then arctan(x) ==  Pi/2 - arctan(1/x)
    //       if (x < 0) then arctan(x) == -Pi/2 - arctan(1/x).
    const auto doReciprocal = (abs(arg) > 1);
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
        return (arg > 0)? FixedPi<BT, FB> / 2 - res: -FixedPi<BT, FB> / 2 - res;
    }
    return res;
}

/// @brief Computes the square root of a non-negative value.
/// @see https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
template <typename BT, unsigned int FB>
constexpr auto ComputeSqrt(fixed<BT, FB> arg)
{
    auto temp = fixed<BT, FB>{1};
    auto tempSquared = temp * temp;
    const auto greaterThanOne = arg > 1;
    auto lower = greaterThanOne? fixed<BT, FB>{1}: arg;
    auto upper = greaterThanOne? arg: fixed<BT, FB>{1};
    while (arg != tempSquared)
    {
        const auto mid = (lower + upper) / 2;
        if (temp == mid)
        {
            break;
        }
        temp = mid;
        tempSquared = temp * temp;
        if (tempSquared > arg)
        {
            upper = temp;
        }
        else if (tempSquared < arg)
        {
            lower = temp;
        }
    }
    return temp;
}

/// @brief Normalizes the given angular argument.
template <typename BT, unsigned int FB>
inline auto AngularNormalize(fixed<BT, FB> angleInRadians)
{
    constexpr auto oneRotationInRadians = 2 * FixedPi<BT, FB>;
    angleInRadians = fmod(angleInRadians, oneRotationInRadians);
    if (angleInRadians > FixedPi<BT, FB>)
    {
        // 190_deg becomes 190_deg - 360_deg = -170_deg
        angleInRadians -= oneRotationInRadians;
    }
    else if (angleInRadians < -FixedPi<BT, FB>)
    {
        // -200_deg becomes -200_deg + 360_deg = 100_deg
        angleInRadians += oneRotationInRadians;
    }
    return angleInRadians;
}

} // namespace detail

/// @defgroup FixedMath Math Functions For fixed Types
/// @brief Common Mathematical Functions For fixed Types.
/// @note These functions directly compute their respective results. They don't convert
///   their inputs to a floating point type to use the standard math functions and then
///   convert those results back to the fixed point type. This has pros and cons. Some
///   pros are that: this won't suffer from the "non-determinism" inherent with different
///   hardware platforms potentially having different floating point or math library
///   implementations; this implementation won't suffer any overhead of converting between
///   the underlying type and a floating point type. On the con side however: this
///   implementation is unlikely to be anywhere near as tested as standard C++ math library
///   functions likely are; this implementation is unlikely to have anywhere near as much
///   performance tuning as standard library functions have had.
/// @see fixed
/// @see https://en.cppreference.com/w/cpp/numeric/math
/// @{

/// @brief Computes the absolute value.
/// @see https://en.cppreference.com/w/cpp/numeric/math/fabs
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> abs(fixed<BT, FB> arg)
{
    return (arg >= 0)? arg: -arg;
}

/// @brief Computes the value of the given number raised to the given power.
/// @note This implementation is for raising a given value to an integer power.
///   This may have significantly different performance than raising a value to a
///   non-integer power.
/// @see https://en.cppreference.com/w/cpp/numeric/math/pow
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> pow(fixed<BT, FB> value, int n)
{
    if (!n)
    {
        return fixed<BT, FB>{1};
    }
    if (value == 0)
    {
        if (n > 0)
        {
            return fixed<BT, FB>{0};
        }
        return fixed<BT, FB>::GetInfinity();
    }
    if (value == 1)
    {
        return fixed<BT, FB>{1};
    }
    if (value == fixed<BT, FB>::GetNegativeInfinity())
    {
        if (n > 0)
        {
            if (n % 2 == 0)
            {
                return fixed<BT, FB>::GetInfinity();
            }
            return fixed<BT, FB>::GetNegativeInfinity();
        }
        return fixed<BT, FB>{0};
    }
    if (value == fixed<BT, FB>::GetInfinity())
    {
        return (n < 0)? fixed<BT, FB>{0}: fixed<BT, FB>::GetInfinity();
    }
    const auto doReciprocal = (n < 0);
    if (doReciprocal)
    {
        n = -n;
    }
    auto res = value;
    for (; n > 1; --n)
    {
        res *= value;
    }
    return (doReciprocal)? 1 / res: res;
}

/// @brief Truncates the given value.
/// @see https://en.cppreference.com/w/c/numeric/math/trunc
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> trunc(fixed<BT, FB> arg)
{
    return static_cast<fixed<BT, FB>>(static_cast<long long>(arg));
}

/// @brief Next after function for fixed types.
/// @see https://en.cppreference.com/w/cpp/numeric/math/nextafter
template <typename BT, unsigned int FB>
inline fixed<BT, FB> nextafter(fixed<BT, FB> from, fixed<BT, FB> to) noexcept
{
    if (from < to)
    {
        return static_cast<fixed<BT, FB>>(from + fixed<BT,FB>::GetMin());
    }
    if (from > to)
    {
        return static_cast<fixed<BT, FB>>(from - fixed<BT,FB>::GetMin());
    }
    return static_cast<fixed<BT, FB>>(to);
}

/// @brief Computes the remainder of the division of the given dividend by the given divisor.
/// @see https://en.cppreference.com/w/cpp/numeric/math/fmod
template <typename BT, unsigned int FB>
inline fixed<BT, FB> fmod(fixed<BT, FB> dividend, fixed<BT, FB> divisor) noexcept
{
    const auto quotient = dividend / divisor;
    const auto integer = trunc(quotient);
    const auto remainder = quotient - integer;
    return remainder * divisor;
}

/// @brief Square root's the given value.
/// @note This implementation isn't meant to be fast, only correct enough.
/// @note The IEEE standard (presumably IEC 60559), requires <code>std::sqrt</code> to be exact
///   to within half of a ULP for floating-point types (float, double). That sets a precedence
///   that puts a high expectation on this implementation for fixed-point types.
/// @note "Domain error" occurs if <code>arg</code> is less than zero.
/// @return Mathematical square root value of the given value or the <code>NaN</code> value.
/// @see https://en.cppreference.com/w/cpp/numeric/math/sqrt
template <typename BT, unsigned int FB>
inline auto sqrt(fixed<BT, FB> arg)
{
    if ((arg == fixed<BT, FB>{1}) || (arg == fixed<BT, FB>{0}))
    {
        return arg;
    }
    if (arg > fixed<BT, FB>{0})
    {
        return detail::ComputeSqrt(arg);
    }
    // else arg < 0 or NaN...
    return fixed<BT, FB>::GetNaN();
}

/// @brief Gets whether the given value is normal - i.e. not 0 nor infinite.
/// @see https://en.cppreference.com/w/cpp/numeric/math/isnormal
template <typename BT, unsigned int FB>
inline bool isnormal(fixed<BT, FB> arg)
{
    return arg != fixed<BT, FB>{0} && arg.isfinite();
}

/// @brief Computes the sine of the argument for fixed types.
/// @see https://en.cppreference.com/w/cpp/numeric/math/sin
template <typename BT, unsigned int FB>
inline fixed<BT, FB> sin(fixed<BT, FB> arg)
{
    arg = detail::AngularNormalize(arg);
    return detail::sin<BT, FB>(arg);
}

/// @brief Computes the cosine of the argument for fixed types.
/// @see https://en.cppreference.com/w/cpp/numeric/math/cos
template <typename BT, unsigned int FB>
inline fixed<BT, FB> cos(fixed<BT, FB> arg)
{
    arg = detail::AngularNormalize(arg);
    return detail::cos<BT, FB>(arg);
}

/// @brief Computes the arc tangent.
/// @see https://en.cppreference.com/w/cpp/numeric/math/atan
/// @return Value between <code>-Pi / 2</code> and <code>Pi / 2</code>.
template <typename BT, unsigned int FB>
inline fixed<BT, FB> atan(fixed<BT, FB> arg)
{
    if (arg.isnan() || (arg == 0))
    {
        return arg;
    }
    if (arg == fixed<BT, FB>::GetInfinity())
    {
        return detail::FixedPi<BT, FB> / 2;
    }
    if (arg == fixed<BT, FB>::GetNegativeInfinity())
    {
        return -detail::FixedPi<BT, FB> / 2;
    }
    return detail::atan<BT, FB>(arg);
}

/// @brief Computes the multi-valued inverse tangent.
/// @see https://en.cppreference.com/w/cpp/numeric/math/atan2
/// @return Value between <code>-Pi</code> and <code>+Pi</code> inclusive.
template <typename BT, unsigned int FB>
inline fixed<BT, FB> atan2(fixed<BT, FB> y, fixed<BT, FB> x)
{
    // See https://en.wikipedia.org/wiki/Atan2
    // See https://en.wikipedia.org/wiki/Taylor_series
    if (x > 0)
    {
        return atan(y / x);
    }
    if (x < 0)
    {
        return atan(y / x) + ((y >= 0)? +1: -1) * detail::FixedPi<BT, FB>;
    }
    if (y > 0)
    {
        return +detail::FixedPi<BT, FB> / 2;
    }
    if (y < 0)
    {
        return -detail::FixedPi<BT, FB> / 2;
    }
    return fixed<BT, FB>::GetNaN();
}

/// @brief Computes the natural logarithm of the given argument.
/// @see https://en.cppreference.com/w/cpp/numeric/math/log
template <typename BT, unsigned int FB>
inline fixed<BT, FB> log(fixed<BT, FB> arg)
{
    static constexpr auto MaxForLowerIterations = 8;
    static constexpr auto IterationsForSmaller = 36;
    static constexpr auto IterationsForLarger = 96;
    return (arg < MaxForLowerIterations)
        ? detail::log<BT, FB, IterationsForSmaller>(arg)
        : detail::log<BT, FB, IterationsForLarger>(arg);
}

/// @brief Computes the Euler number raised to the power of the given argument.
/// @see https://en.cppreference.com/w/cpp/numeric/math/exp
template <typename BT, unsigned int FB>
inline fixed<BT, FB> exp(fixed<BT, FB> arg)
{
    static constexpr auto MaxForLowerIterations = 2;
    static constexpr auto IterationsForLarger = 24;
    return (arg <= MaxForLowerIterations)
        ? detail::exp<BT, FB, detail::DefaultExpIterations>(arg)
        : detail::exp<BT, FB, IterationsForLarger>(arg);
}

/// @brief Computes the value of the base number raised to the power of the exponent.
/// @see https://en.cppreference.com/w/cpp/numeric/math/pow
template <typename BT, unsigned int FB>
inline fixed<BT, FB> pow(fixed<BT, FB> base, fixed<BT, FB> exponent)
{
    if (exponent.isfinite())
    {
        const auto intExp = static_cast<int>(exponent);
        if (intExp == exponent)
        {
            // fall back to integer version...
            return pow(base, intExp);
        }
    }
    if (base < 0)
    {
        return fixed<BT, FB>::GetNaN();
    }
    const auto lnResult = log(base);
    const auto expResult = exp(lnResult * exponent);
    return expResult;
}

/// @brief Computes the square root of the sum of the squares.
/// @see https://en.cppreference.com/w/cpp/numeric/math/hypot
template <typename BT, unsigned int FB>
inline fixed<BT, FB> hypot(fixed<BT, FB> x, fixed<BT, FB> y)
{
    return sqrt(x * x + y * y);
}

/// @brief Rounds the given value.
/// @see https://en.cppreference.com/w/cpp/numeric/math/round
template <typename BT, unsigned int FB>
inline fixed<BT, FB> round(fixed<BT, FB> value) noexcept
{
    const auto tmp = value + (fixed<BT, FB>{1} / fixed<BT, FB>{2});
    const auto truncated = static_cast<typename fixed<BT, FB>::value_type>(tmp);
    return fixed<BT, FB>{truncated, 0};
}

/// @brief Determines whether the given value is negative.
/// @see https://en.cppreference.com/w/cpp/numeric/math/signbit
template <typename BT, unsigned int FB>
inline bool signbit(fixed<BT, FB> value) noexcept
{
    return value.getsign() < 0;
}

/// @brief Gets whether the given value is not-a-number.
/// @see https://en.cppreference.com/w/cpp/numeric/math/isnan
template <typename BT, unsigned int FB>
constexpr bool isnan(fixed<BT, FB> value) noexcept
{
    return value.Compare(0) == fixed<BT, FB>::ordering::unordered;
}

/// @brief Gets whether the given value is finite.
/// @see https://en.cppreference.com/w/cpp/numeric/math/isfinite
template <typename BT, unsigned int FB>
inline bool isfinite(fixed<BT, FB> value) noexcept
{
    return (value > fixed<BT, FB>::GetNegativeInfinity())
    && (value < fixed<BT, FB>::GetInfinity());
}

/// @brief Gets wether the given value is a positive or negative infinity.
/// @see https://en.cppreference.com/w/cpp/numeric/math/isinf
template <typename BT, unsigned int FB>
auto isinf(const fixed<BT, FB>& value) noexcept
{
    return (value == fixed<BT, FB>::GetInfinity()) // force newline
        || (value == fixed<BT, FB>::GetNegativeInfinity());
}

/// @brief Gets the largest integer value not greater than the given value.
/// @see https://en.cppreference.com/w/cpp/numeric/math/floor.
template <typename BT, unsigned int FB>
auto floor(const fixed<BT, FB>& value)
{
    const auto tmp = trunc(value);
    return (tmp > value)? (value - 1): tmp;
}

/// @}

} // namespace realnumb

#endif // REALNUMB_FIXEDMATH_HPP
