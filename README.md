# realnumb
A `constexpr` enhanced real numbers library with support for fixed point types that satisfy the [LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType) named requirement.

See also:

- [Fixed point arithmetic on WikiPedia](https://en.wikipedia.org/wiki/Fixed-point_arithmetic).
- [PlayRho](https://github.com/louis-langholtz/PlayRho) issue [No external fixed-point implementation supported](https://github.com/louis-langholtz/PlayRho/issues/295).
- [Compositional Numeric Library](https://github.com/johnmcfarlane/cnl), a C++ library of fixed-precision numeric classes.
- [libfixmath archived at code.google.com](https://code.google.com/archive/p/libfixmath/) and [libfixmath at github](https://github.com/PetteriAimonen/libfixmath). Neither of these appear to have `constexpr` support, nor support for anything but [Q16.16](https://en.wikipedia.org/wiki/Q_(number_format)) format, nor active maintenance.
