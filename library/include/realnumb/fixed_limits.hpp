#ifndef REALNUMB_FIXEDLIMITS_HPP
#define REALNUMB_FIXEDLIMITS_HPP

/// @file
/// @brief Specialization of @c std::numeric_limits for @c fixed.

#include <limits> // for std::numeric_limits

#include <realnumb/fixed.hpp>

/// @brief Template specialization of numeric limits for fixed types.
/// @see https://en.cppreference.com/w/cpp/types/numeric_limits.
/// @see https://en.wikipedia.org/wiki/IEEE_754.
template <typename BT, unsigned int FB>
class std::numeric_limits<realnumb::fixed<BT,FB>>
{
public:
    static constexpr bool is_specialized = true; ///< Type is specialized.

    /// @brief Gets the min value available for the type.
    static constexpr realnumb::fixed<BT,FB> min() noexcept { return realnumb::fixed<BT,FB>::get_min(); }

    /// @brief Gets the max value available for the type.
    static constexpr realnumb::fixed<BT,FB> max() noexcept    { return realnumb::fixed<BT,FB>::get_max(); }

    /// @brief Gets the lowest value available for the type.
    static constexpr realnumb::fixed<BT,FB> lowest() noexcept { return realnumb::fixed<BT,FB>::get_lowest(); }

    /// @brief Number of radix digits that can be represented.
    static constexpr int digits = realnumb::fixed<BT,FB>::whole_bits - 1;

    /// @brief Number of decimal digits that can be represented.
    static constexpr int digits10 = realnumb::fixed<BT,FB>::whole_bits - 1;

    /// @brief Number of decimal digits necessary to differentiate all values.
    static constexpr int max_digits10 = 5; // TODO(lou): check this

    static constexpr bool is_signed = true; ///< Identifies signed types.
    static constexpr bool is_integer = false; ///< Identifies integer types.
    static constexpr bool is_exact = true; ///< Identifies exact type.
    static constexpr int radix = 0; ///< Radix used by the type.

    /// @brief Gets the epsilon value for the type.
    static constexpr realnumb::fixed32 epsilon() noexcept { return realnumb::fixed<BT,FB>{0}; } // TODO(lou)

    /// @brief Gets the round error value for the type.
    static constexpr realnumb::fixed32 round_error() noexcept { return realnumb::fixed<BT,FB>{0}; } // TODO(lou)

    /// @brief One more than smallest negative power of the radix that's a valid
    ///    normalized floating-point value.
    static constexpr int min_exponent = 0;

    /// @brief Smallest negative power of ten that's a valid normalized floating-point value.
    static constexpr int min_exponent10 = 0;

    /// @brief One more than largest integer power of radix that's a valid finite
    ///   floating-point value.
    static constexpr int max_exponent = 0;

    /// @brief Largest integer power of 10 that's a valid finite floating-point value.
    static constexpr int max_exponent10 = 0;

    static constexpr bool has_infinity = true; ///< Whether can represent infinity.
    static constexpr bool has_quiet_NaN = true; ///< Whether can represent quiet-NaN.
    static constexpr bool has_signaling_NaN = false; ///< Whether can represent signaling-NaN.
    static constexpr float_denorm_style has_denorm = denorm_absent; ///< <code>Denorm</code> style used.
    static constexpr bool has_denorm_loss = false; ///< Has <code>denorm</code> loss amount.

    /// @brief Gets the infinite value for the type.
    static constexpr realnumb::fixed<BT,FB> infinity() noexcept { return realnumb::fixed<BT,FB>::get_positive_infinity(); }

    /// @brief Gets the quiet NaN value for the type.
    static constexpr realnumb::fixed<BT,FB> quiet_NaN() noexcept { return realnumb::fixed<BT,FB>::get_nan(); }

    /// @brief Gets the signaling NaN value for the type.
    static constexpr realnumb::fixed<BT,FB> signaling_NaN() noexcept { return realnumb::fixed<BT,FB>{0}; }

    /// @brief Gets the <code>denorm</code> value for the type.
    static constexpr realnumb::fixed<BT,FB> denorm_min() noexcept { return realnumb::fixed<BT,FB>{0}; }

    static constexpr bool is_iec559 = false; ///< @brief Not an IEEE 754 floating-point type.
    static constexpr bool is_bounded = true; ///< Type bounded: has limited precision.
    static constexpr bool is_modulo = false; ///< Doesn't modulo arithmetic overflows.

    static constexpr bool traps = false; ///< Doesn't do traps.
    static constexpr bool tinyness_before = false; ///< Doesn't detect <code>tinyness</code> before rounding.
    static constexpr float_round_style round_style = round_to_nearest; ///< Rounding like IEEE 754
};

// Assert basic constants of numeric_limits for fixed32...
static_assert(!std::numeric_limits<realnumb::fixed32>::is_integer);
static_assert(!std::numeric_limits<realnumb::fixed32>::is_modulo);
static_assert(std::numeric_limits<realnumb::fixed32>::is_specialized);
static_assert(std::numeric_limits<realnumb::fixed32>::is_signed);
static_assert(std::numeric_limits<realnumb::fixed32>::is_bounded);
static_assert(std::numeric_limits<realnumb::fixed32>::is_exact);
static_assert(std::numeric_limits<realnumb::fixed32>::has_infinity);
static_assert(std::numeric_limits<realnumb::fixed32>::has_quiet_NaN);

#endif // REALNUMB_FIXEDLIMITS_HPP
