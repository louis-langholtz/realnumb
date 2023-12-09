#ifndef REALNUMB_FIXED_HPP
#define REALNUMB_FIXED_HPP

/// @file
/// @brief Definition of the @c fixed class and closely related code.

#include <cstdint> // for types like std::int32_t
#include <limits> // for std::numeric_limits
#include <cassert> // for assert macro
#include <type_traits> // for std::decay_t and more
#include <iostream>
#include <utility> // for std::forward

namespace realnumb {
namespace detail {

/// @brief Defaults for the <code>fixed</code> class template.
/// @see fixed.
template <typename BaseType>
struct fixed_default {};

/// @brief Defaults for 32-bit integer instantiation of the <code>fixed</code> class template.
template <>
struct fixed_default<std::int32_t>
{
    /// @brief Fraction bits for the type this is specialized for.
    static constexpr auto fraction_bits = 9u;
};

/// @brief Defaults for 64-bit integer instantiation of the <code>fixed</code> class template.
template <>
struct fixed_default<std::int64_t>
{
    /// @brief Fraction bits for the type this is specialized for.
    static constexpr auto fraction_bits = 24u;
};

/// @brief wider data type obtainer.
/// @details Widens a data type to the data type that's twice its original size.
template <typename T>
struct wider {
};

/// @brief Specialization of the wider trait for signed 8-bit integers.
template <>
struct wider<std::int8_t> {
    using type = std::int16_t; ///< wider type.
};

/// @brief Specialization of the wider trait for unsigned 8-bit integers.
template <>
struct wider<std::uint8_t> {
    using type = std::uint16_t; ///< wider type.
};

/// @brief Specialization of the wider trait for signed 16-bit integers.
template <>
struct wider<std::int16_t> {
    using type = std::int32_t; ///< wider type.
};

/// @brief Specialization of the wider trait for unsigned 16-bit integers.
template <>
struct wider<std::uint16_t> {
    using type = std::uint32_t; ///< wider type.
};

/// @brief Specialization of the wider trait for signed 32-bit integers.
template <>
struct wider<std::int32_t> {
    using type = std::int64_t; ///< wider type.
};

/// @brief Specialization of the wider trait for unsigned 32-bit integers.
template <>
struct wider<std::uint32_t> {
    using type = std::uint64_t; ///< wider type.
};

/// @brief Specialization of the wider trait for float.
template <>
struct wider<float> {
    using type = double; ///< wider type.
};

/// @brief Specialization of the wider trait for double.
template <>
struct wider<double> {
    using type = long double; ///< wider type.
};

#ifdef __SIZEOF_INT128__
#define REALNUMB_INT128 __int128_t
#define REALNUMB_UINT128 __uint128_t
#endif

#ifdef REALNUMB_INT128
/// @brief Specialization of the wider trait for signed 64-bit integers.
template <>
struct wider<std::int64_t> {
    using type = REALNUMB_INT128; ///< wider type.
};
#endif

#ifdef REALNUMB_UINT128
/// @brief Specialization of the wider trait for unsigned 64-bit integers.
template <>
struct wider<std::uint64_t> {
    using type = REALNUMB_UINT128; ///< wider type.
};
#endif

} // namespace detail

/// @brief compare result enumeration - a partial ordering result.
enum class ordering
{
    unordered,
    equivalent,
    less,
    greater
};

/// @brief Template class for fixed-point real-like numbers.
/// @details This is a fixed point type template for a given base type using a given number
///   of fraction bits that satisfies the <code>LiteralType</code> named requirement.
/// @see https://en.wikipedia.org/wiki/Fixed-point_arithmetic
/// @see https://en.cppreference.com/w/cpp/named_req/LiteralType
template <typename BaseType, unsigned int FractionBits>
class fixed
{
public:

    /// @brief Value type.
    using value_type = BaseType;

    /// @brief Bits per byte.
    static constexpr auto bits_per_byte = 8u;

    /// @brief Total number of bits.
    static constexpr auto total_bits = static_cast<unsigned int>(sizeof(BaseType) * bits_per_byte);

    /// @brief Fraction bits.
    static constexpr auto fraction_bits = FractionBits;

    /// @brief Whole value bits.
    static constexpr auto whole_bits = total_bits - fraction_bits;

    /// @brief Scale factor.
    static constexpr auto scale_factor = static_cast<value_type>(1u << fraction_bits);

    /// @brief Gets the min value this type is capable of expressing.
    static constexpr auto get_min() noexcept -> fixed
    {
        return fixed{1, scalar_type{1}};
    }

    /// @brief Gets an infinite value for this type.
    static constexpr auto get_positive_infinity() noexcept -> fixed
    {
        return fixed{numeric_limits::max(), scalar_type{1}};
    }

    /// @brief Gets the max value this type is capable of expressing.
    static constexpr auto get_max() noexcept -> fixed
    {
        // max reserved for +inf
        return fixed{numeric_limits::max() - 1, scalar_type{1}};
    }

    /// @brief Gets a NaN value for this type.
    static constexpr auto get_nan() noexcept -> fixed
    {
        return fixed{numeric_limits::lowest(), scalar_type{1}};
    }

    /// @brief Gets the negative infinity value for this type.
    static constexpr auto get_negative_infinity() noexcept -> fixed
    {
        // lowest reserved for NaN
        return fixed{numeric_limits::lowest() + 1, scalar_type{1}};
    }

    /// @brief Gets the lowest value this type is capable of expressing.
    static constexpr auto get_lowest() noexcept -> fixed
    {
        // lowest reserved for NaN
        // lowest + 1 reserved for -inf
        return fixed{numeric_limits::lowest() + 2, scalar_type{1}};
    }

    /// @brief Gets the value from a floating point value.
    template <typename T>
    static constexpr auto to_value(T val) noexcept
        -> std::enable_if_t<std::is_floating_point_v<T>, value_type>
    {
        // Note: std::isnan(val) *NOT* constant expression, so can't use here!
        return !(val <= 0 || val >= 0) // NOLINT(misc-redundant-expression)
            ? get_nan().m_value // newline!
            : (static_cast<long double>(val) > static_cast<long double>(get_max())) // newline!
                ? get_positive_infinity().m_value // newline!
                : (static_cast<long double>(val) < static_cast<long double>(get_lowest())) // newline!
                    ? get_negative_infinity().m_value // newline!
                    : static_cast<value_type>(val * scale_factor);
    }

    /// @brief Gets the value from a signed integral value.
    template <typename T>
    static constexpr auto to_value(T val) noexcept
        -> std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, value_type>
    {
        return (val > (get_max().m_value / scale_factor))? get_positive_infinity().m_value:
            (val < (get_lowest().m_value / scale_factor))? get_negative_infinity().m_value:
            static_cast<value_type>(val * scale_factor);
    }

    /// @brief Gets the value from an unsigned integral value.
    template <typename T>
    static constexpr auto to_value(T val) noexcept
        -> std::enable_if_t<std::is_integral_v<T> && !std::is_signed_v<T>, value_type>
    {
        const auto max = static_cast<unsigned_wider_type>(get_max().m_value / scale_factor);
        return (val > max)? get_positive_infinity().m_value: static_cast<value_type>(val) * scale_factor;
    }

    fixed() = default;

    /// @brief Initializing constructor from any arithmetic type.
    template <class U, std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>, int> = 0>
    constexpr fixed(U&& val) noexcept:
        m_value{to_value(std::forward<U>(val))}
    {
        // Intentionally empty
    }

    /// @brief Initializing constructor.
    constexpr fixed(value_type val, unsigned int fraction) noexcept:
        m_value{static_cast<value_type>(static_cast<std::uint32_t>(val * scale_factor) | fraction)}
    {
        // Intentionally empty.
    }

    /// @brief Copy constructor for copying from any fixed type.
    template <typename BT, unsigned int FB>
    constexpr fixed(const fixed<BT, FB> val) noexcept:
        fixed(static_cast<long double>(val))
    {
        // Intentionally empty
    }

    // Methods

    /// @brief Converts the value to the expressed type.
    template <typename T>
    constexpr auto to_type() const noexcept -> std::enable_if_t<std::is_floating_point_v<T>, T>
    {
        return isnan() // newline!
            ? std::numeric_limits<T>::signaling_NaN() // newline!
            : !isfinite() // newline!
                ? std::numeric_limits<T>::infinity() * static_cast<T>(getsign()) // newline!
                : static_cast<T>(m_value) / static_cast<T>(scale_factor);
    }

    /// @brief Compares this value to the given one.
    constexpr auto compare(const fixed other) const noexcept -> ordering
    {
        if (isnan() || other.isnan())
        {
            return ordering::unordered;
        }
        if (m_value < other.m_value)
        {
            return ordering::less;
        }
        if (m_value > other.m_value)
        {
            return ordering::greater;
        }
        return ordering::equivalent;
    }

    // Unary operations

    /// @brief To floating point type operator.
    template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    explicit constexpr operator T() const noexcept
    {
        return to_type<T>();
    }

    /// @brief Long long operator.
    explicit constexpr operator long long() const noexcept
    {
        return m_value / scale_factor;
    }

    /// @brief Long operator.
    explicit constexpr operator long() const noexcept
    {
        return m_value / scale_factor;
    }

    /// @brief Unsigned long long operator.
    explicit constexpr operator unsigned long long() const noexcept
    {
        return static_cast<unsigned long long>(m_value / scale_factor);
    }

    /// @brief Unsigned long operator.
    explicit constexpr operator unsigned long() const noexcept
    {
        return static_cast<unsigned long>(m_value / scale_factor);
    }

    /// @brief Unsigned int operator.
    explicit constexpr operator unsigned int() const noexcept
    {
        return static_cast<unsigned int>(m_value / scale_factor);
    }

    /// @brief int operator.
    explicit constexpr operator int() const noexcept
    {
        return static_cast<int>(m_value / scale_factor);
    }

    /// @brief short operator.
    explicit constexpr operator short() const noexcept
    {
        return static_cast<short>(m_value / scale_factor);
    }

    /// @brief Negation operator.
    constexpr auto operator- () const noexcept -> fixed
    {
        return (isnan())? *this: fixed{-m_value, scalar_type{1}};
    }

    /// @brief Positive operator.
    constexpr auto operator+ () const noexcept -> fixed
    {
        return *this;
    }

    /// @brief Boolean operator.
    explicit constexpr operator bool() const noexcept
    {
        return m_value != 0;
    }

    /// @brief Logical not operator.
    constexpr bool operator! () const noexcept
    {
        return m_value == 0;
    }

    /// @brief Addition assignment operator.
    constexpr auto operator+= (fixed val) noexcept -> fixed&
    {
        if (isnan() || val.isnan() // newline!
            || ((*this == get_positive_infinity()) && (val == get_negative_infinity()))
            || ((*this == get_negative_infinity()) && (val == get_positive_infinity()))
            )
        {
            *this = get_nan();
        }
        else if (val == get_positive_infinity())
        {
            *this = get_positive_infinity();
        }
        else if (val == get_negative_infinity())
        {
            *this = get_negative_infinity();
        }
        else if (isfinite() && val.isfinite())
        {
            m_value = ((m_value > 0) && (val.m_value > get_max().m_value - m_value)) // newline!
                ? get_positive_infinity().m_value // overflow
                : ((m_value < 0) && (val.m_value < get_lowest().m_value - m_value)) // newline!
                    ? get_negative_infinity().m_value // underflow
                    : m_value + val.m_value;
        }
        return *this;
    }

    /// @brief Subtraction assignment operator.
    constexpr auto operator-= (fixed val) noexcept -> fixed&
    {
        if (isnan() || val.isnan() // newline!
            || ((*this == get_positive_infinity()) && (val == get_positive_infinity()))
            || ((*this == get_negative_infinity()) && (val == get_negative_infinity()))
        )
        {
            *this = get_nan();
        }
        else if (val.m_value == get_positive_infinity().m_value)
        {
            *this = get_negative_infinity();
        }
        else if (val.m_value == get_negative_infinity().m_value)
        {
            *this = get_positive_infinity();
        }
        else if (isfinite() && val.isfinite())
        {
            m_value = ((m_value > 0) && (val.m_value < get_lowest().m_value + m_value)) // newline!
                ? get_positive_infinity().m_value // overflow
                : ((m_value < 0) && (val.m_value > get_max().m_value + m_value)) // newline!
                    ? get_negative_infinity().m_value // underflow
                    : m_value - val.m_value;
        }
        return *this;
    }

    /// @brief Multiplication assignment operator.
    constexpr auto operator*= (fixed val) noexcept -> fixed&
    {
        if (isnan() || val.isnan())
        {
            *this = get_nan();
        }
        else if (!isfinite() || !val.isfinite())
        {
            if ((m_value == 0) || (val.m_value == 0))
            {
                *this = get_nan();
            }
            else
            {
                *this = ((m_value > 0) != (val.m_value > 0)) // newline!
                    ? -get_positive_infinity(): get_positive_infinity();
            }
        }
        else
        {
            const auto product = wider_type{m_value} * wider_type{val.m_value};
            const auto offset = (((product < 0) == (scale_factor < 0)) ? scale_factor : -scale_factor) / 2;
            const auto result = (product + offset) / scale_factor;
            m_value = (product != 0 && result == 0) // newline!
                ? static_cast<value_type>(result) // newline!
                : (result > get_max().m_value) // newline!
                    ? get_positive_infinity().m_value // newline!
                    : (result < get_lowest().m_value) // newline!
                        ? get_negative_infinity().m_value // newline!
                        : static_cast<value_type>(result);
        }
        return *this;
    }

    /// @brief Division assignment operator.
    constexpr auto operator/= (fixed val) noexcept -> fixed&
    {
        if (isnan() || val.isnan())
        {
            *this = get_nan();
        }
        else if (!isfinite() && !val.isfinite())
        {
            *this = get_nan();
        }
        else if (!isfinite())
        {
            *this = ((m_value > 0) != (val.m_value > 0)) // newline!
                ? -get_positive_infinity(): get_positive_infinity();
        }
        else if (!val.isfinite())
        {
            *this = 0;
        }
        else
        {
            const auto product = wider_type{m_value} * scale_factor;
            const auto offset = (((product < 0) == (val.m_value < 0)) ? val.m_value : -val.m_value) / 2;
            const auto result = (product + offset) / val.m_value;
            m_value = (product != 0 && result == 0) // newline!
                ? static_cast<value_type>(result) // newline!
                : (result > get_max().m_value) // newline!
                    ? get_positive_infinity().m_value // newline!
                    : (result < get_lowest().m_value) // newline!
                        ? get_negative_infinity().m_value // newline!
                        : static_cast<value_type>(result);
        }
        return *this;
    }

    /// @brief Modulo operator.
    constexpr auto operator%= (fixed val) noexcept -> fixed&
    {
        assert(!isnan());
        assert(!val.isnan());
        m_value %= val.m_value;
        return *this;
    }

    /// @brief Is finite.
    constexpr auto isfinite() const noexcept -> bool
    {
        return (*this > get_negative_infinity()) && (*this < get_positive_infinity());
    }

    /// @brief Is NaN.
    constexpr auto isnan() const noexcept -> bool
    {
        return m_value == get_nan().m_value;
    }

    /// @brief Gets this value's sign.
    constexpr auto getsign() const noexcept -> int
    {
        return (m_value >= 0)? +1: -1;
    }

private:

    /// @brief Widened type alias.
    using wider_type = typename detail::wider<value_type>::type;

    /// @brief Unsigned widened type alias.
    using unsigned_wider_type = std::make_unsigned_t<wider_type>;

    /// @brief Scalar type.
    struct scalar_type
    {
        value_type value = 1; ///< Value.
    };

    /// @brief Numeric limits type alias.
    using numeric_limits = std::numeric_limits<value_type>;

    /// @brief Initializing constructor.
    constexpr fixed(value_type val, scalar_type scalar) noexcept:
        m_value{val * scalar.value}
    {
        // Intentionally empty.
    }

    value_type m_value; ///< Value in internal form.
};

/// @brief Equality operator.
template <typename BT, unsigned int FB>
constexpr bool operator== (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    return lhs.compare(rhs) == ordering::equivalent;
}

/// @brief Inequality operator.
template <typename BT, unsigned int FB>
constexpr bool operator!= (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    return lhs.compare(rhs) != ordering::equivalent;
}

/// @brief Less-than operator.
template <typename BT, unsigned int FB>
constexpr bool operator< (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    return lhs.compare(rhs) == ordering::less;
}

/// @brief Greater-than operator.
template <typename BT, unsigned int FB>
constexpr bool operator> (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    return lhs.compare(rhs) == ordering::greater;
}

/// @brief Less-than or equal-to operator.
template <typename BT, unsigned int FB>
constexpr bool operator<= (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return result == ordering::less || result == ordering::equivalent;
}

/// @brief Greater-than or equal-to operator.
template <typename BT, unsigned int FB>
constexpr bool operator>= (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return result == ordering::greater || result == ordering::equivalent;
}

/// @brief Addition operator.
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> operator+ (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    lhs += rhs;
    return lhs;
}

/// @brief Subtraction operator.
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> operator- (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    lhs -= rhs;
    return lhs;
}

/// @brief Multiplication operator.
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> operator* (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    lhs *= rhs;
    return lhs;
}

/// @brief Division operator.
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> operator/ (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    lhs /= rhs;
    return lhs;
}

/// @brief Modulo operator.
template <typename BT, unsigned int FB>
constexpr fixed<BT, FB> operator% (fixed<BT, FB> lhs, fixed<BT, FB> rhs) noexcept
{
    lhs %= rhs;
    return lhs;
}

/// @brief Output stream operator.
template <typename BT, unsigned int FB>
inline ::std::ostream& operator<<(::std::ostream& os, const fixed<BT, FB>& value)
{
    return os << static_cast<double>(value);
}

/// @brief 32-bit fixed precision type.
/// @details This is a 32-bit fixed precision type with a Q number-format of
///   <code>Q23.9</code>.
/// @warning The available numeric fidelity of any 32-bit fixed point type is very limited.
///   Using a 32-bit fixed point type for Real should only be considered for simulations
///   where it's been found to work and where the dynamics won't be changing between runs.
/// @note Maximum value (with 9 fraction bits) is approximately 4194303.99609375.
/// @note Minimum value (with 9 fraction bits) is approximately 0.001953125.
/// @see fixed, Real
/// @see https://en.wikipedia.org/wiki/Q_(number_format)
using fixed32 = fixed<std::int32_t, detail::fixed_default<std::int32_t>::fraction_bits>;

// Assert basic type traits...
static_assert(std::is_trivially_copyable_v<fixed32>); // can safely copy with std::memcpy!
static_assert(std::is_trivial_v<fixed32>); // trivially copyable & trivial default ctor

// fixed32 free functions.

/// @brief Addition operator.
constexpr fixed32 operator+ (fixed32 lhs, fixed32 rhs) noexcept
{
    lhs += rhs;
    return lhs;
}

/// @brief Subtraction operator.
constexpr fixed32 operator- (fixed32 lhs, fixed32 rhs) noexcept
{
    lhs -= rhs;
    return lhs;
}

/// @brief Multiplication operator.
constexpr fixed32 operator* (fixed32 lhs, fixed32 rhs) noexcept
{
    lhs *= rhs;
    return lhs;
}

/// @brief Division operator.
constexpr fixed32 operator/ (fixed32 lhs, fixed32 rhs) noexcept
{
    lhs /= rhs;
    return lhs;
}

/// @brief Modulo operator.
constexpr fixed32 operator% (fixed32 lhs, fixed32 rhs) noexcept
{
    lhs %= rhs;
    return lhs;
}

/// @brief Equality operator.
constexpr bool operator== (fixed32 lhs, fixed32 rhs) noexcept
{
    return lhs.compare(rhs) == ordering::equivalent;
}

/// @brief Inequality operator.
constexpr bool operator!= (fixed32 lhs, fixed32 rhs) noexcept
{
    return lhs.compare(rhs) != ordering::equivalent;
}

/// @brief Less-than or equal-to operator.
constexpr bool operator <= (fixed32 lhs, fixed32 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return (result == ordering::less) || (result == ordering::equivalent);
}

/// @brief Greater-than or equal-to operator.
constexpr bool operator >= (fixed32 lhs, fixed32 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return (result == ordering::greater) || (result == ordering::equivalent);
}

/// @brief Less-than operator.
constexpr bool operator < (fixed32 lhs, fixed32 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return result == ordering::less;
}

/// @brief Greater-than operator.
constexpr bool operator > (fixed32 lhs, fixed32 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return result == ordering::greater;
}

#ifdef REALNUMB_INT128
// fixed64 free functions.

/// @brief 64-bit fixed precision type.
/// @details This is a 64-bit fixed precision type with a Q number-format of
///   <code>Q40.24</code>.
///
/// @note Minimum value (with 24 fraction bits) is approximately
///   <code>5.9604644775390625e-08</code>.
/// @note Maximum value (with 24 fraction bits) is approximately
///   <code>549755813888</code>.
///
/// @see fixed, Real
/// @see https://en.wikipedia.org/wiki/Q_(number_format)
///
using fixed64 = fixed<std::int64_t,detail::fixed_default<std::int64_t>::fraction_bits>;

// Assert basic type traits...
static_assert(std::is_trivially_copyable_v<fixed64>); // can safely copy with std::memcpy!
static_assert(std::is_trivial_v<fixed64>); // trivially copyable & trivial default ctor

/// @brief Addition operator.
constexpr fixed64 operator+ (fixed64 lhs, fixed64 rhs) noexcept
{
    lhs += rhs;
    return lhs;
}

/// @brief Subtraction operator.
constexpr fixed64 operator- (fixed64 lhs, fixed64 rhs) noexcept
{
    lhs -= rhs;
    return lhs;
}

/// @brief Multiplication operator.
constexpr fixed64 operator* (fixed64 lhs, fixed64 rhs) noexcept
{
    lhs *= rhs;
    return lhs;
}

/// @brief Division operator.
constexpr fixed64 operator/ (fixed64 lhs, fixed64 rhs) noexcept
{
    lhs /= rhs;
    return lhs;
}

constexpr fixed64 operator% (fixed64 lhs, fixed64 rhs) noexcept
{
    lhs %= rhs;
    return lhs;
}

/// @brief Equality operator.
constexpr bool operator== (fixed64 lhs, fixed64 rhs) noexcept
{
    return lhs.compare(rhs) == ordering::equivalent;
}

/// @brief Inequality operator.
constexpr bool operator!= (fixed64 lhs, fixed64 rhs) noexcept
{
    return lhs.compare(rhs) != ordering::equivalent;
}

constexpr bool operator <= (fixed64 lhs, fixed64 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return (result == ordering::less) || (result == ordering::equivalent);
}

constexpr bool operator >= (fixed64 lhs, fixed64 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return (result == ordering::greater) || (result == ordering::equivalent);
}

constexpr bool operator < (fixed64 lhs, fixed64 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return result == ordering::less;
}

constexpr bool operator > (fixed64 lhs, fixed64 rhs) noexcept
{
    const auto result = lhs.compare(rhs);
    return result == ordering::greater;
}

/// @brief Specialization of the wider trait for the <code>fixed32</code> type.
template<> struct detail::wider<fixed32> {
    using type = fixed64; ///< wider type.
};

#endif /* REALNUMB_INT128 */

} // namespace realnumb

#endif // REALNUMB_FIXED_HPP
