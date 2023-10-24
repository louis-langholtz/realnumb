# realnumb

A `constexpr` enhanced, real numbers, C++ library with support for fixed point types satisfying the [LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType) named requirement.

## Why?

Why yet another fixed-point data type and math library?

Reasons:

- For a `constexpr` enhanced fixed-point type.
- For `constexpr` enhanced `cmath` functions at least for the fixed-point type.
- For that fixed-point type to satisfy the `LiteralType` named requirement.
- To get this code out from another project it had been in ([PlayRho](https://github.com/louis-langholtz/PlayRho)), and in to its own project.
- Having a project whose consumption by another project of mine could be more readily tested.

## Caveats

This is experimental code that's a work in progress.

## Usage

### Component

To use this within your own project...

#### CMake FetchContent Usage

Add the following to your CMake configuration where you replace `your_target` with your project's target name:

```cmake
include(FetchContent)
FetchContent_Declare(
    realnumb
    GIT_REPOSITORY https://github.com/louis-langholtz/realnumb
    GIT_TAG main # Hash or tag or branch name
)
FetchContent_MakeAvailable(realnumb)
target_link_libraries(your_target realnumb)
```

### Stand Alone

Like for development...

#### Via CMake

Configure and build:

```sh
cmake -S realnumb -B realnumb_build -DREALNUMB_BUILD_UNITTEST=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DREALNUMB_INSTALL=ON
cmake --build realnumb_build --config Release
```

Then, for a local install:

```sh
cmake --install realnumb_build --prefix realnumb_install
```

## Related

- [Fixed point arithmetic on WikiPedia](https://en.wikipedia.org/wiki/Fixed-point_arithmetic).
- [PlayRho](https://github.com/louis-langholtz/PlayRho) issue [No external fixed-point implementation supported](https://github.com/louis-langholtz/PlayRho/issues/295).
- [Compositional Numeric Library](https://github.com/johnmcfarlane/cnl), a C++ library of fixed-precision numeric classes.
- [libfixmath archived at code.google.com](https://code.google.com/archive/p/libfixmath/) and [libfixmath at github](https://github.com/PetteriAimonen/libfixmath). Neither of these appear to have `constexpr` support, nor support for anything but [Q16.16](https://en.wikipedia.org/wiki/Q_(number_format)) format, nor active maintenance.
