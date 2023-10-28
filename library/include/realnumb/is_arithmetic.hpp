#ifndef REALNUMB_IS_ARITHMETIC_HPP
#define REALNUMB_IS_ARITHMETIC_HPP

#include <type_traits>

namespace realnumb {

/// @brief Template for determining if the given type is an "arithmetic" type.
/// @note In the context of this library, "arithmetic" types are types which
///   have +, -, *, / arithmetic operator support.
template <class T, class = void>
struct is_arithmetic : std::false_type {
};

/// @brief Template specialization for basic qualities for valid/acceptable
///   "arithmetic" types.
template <class T>
struct is_arithmetic<T,
    std::void_t<decltype(+T{}), decltype(-T{}),
                decltype(T{} + T{}), decltype(T{} - T{}),
                decltype(T{} * T{}), decltype(T{} / T{})>
> : std::true_type {
};

/// @brief Determines whether the given type is an arithmetic type.
template< class T >
constexpr auto is_arithmetic_v = is_arithmetic<T>::value;

}

#endif /* REALNUMB_IS_ARITHMETIC_HPP */
