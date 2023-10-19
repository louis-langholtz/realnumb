#include <gtest/gtest.h>

#include "realnumb/fixed.hpp"
#include "realnumb/fixed_limits.hpp"
#include "realnumb/fixed_math.hpp"

#include <iostream>

using namespace realnumb;

TEST(fixed32, ByteSizeIs4)
{
    // Check size at test runtime instead of compile-time via static_assert to avoid stopping
    // builds and to report actual size rather than just reporting that expected size is wrong.
    EXPECT_EQ(sizeof(fixed32), std::size_t(4));
}

#ifdef REALNUMB_INT128

TEST(fixed64, ByteSizeIs8)
{
    EXPECT_EQ(sizeof(fixed64), std::size_t(8));
}

#endif

// Tests of fixed<T>::GetFromUnsignedInt(v)

#define DECL_GET_FROM_UNSIGNED_INT_TEST(type) \
TEST(type, GetFromUnsignedInt) \
{ \
    EXPECT_EQ(type::GetFromUnsignedInt(0u),  0 * type::scale_factor); \
    EXPECT_EQ(type::GetFromUnsignedInt(1u),  1 * type::scale_factor); \
    EXPECT_EQ(type::GetFromUnsignedInt(2u),  2 * type::scale_factor); \
}

DECL_GET_FROM_UNSIGNED_INT_TEST(fixed32)
#ifdef REALNUMB_INT128
DECL_GET_FROM_UNSIGNED_INT_TEST(fixed64)
#endif

// Tests of fixed<T>::GetFromSignedInt(v)

#define DECL_GET_FROM_SIGNED_INT_TEST(type) \
TEST(type, GetFromSignedInt) \
{ \
    EXPECT_EQ(type::GetFromSignedInt( 0),  0 * type::scale_factor); \
    EXPECT_EQ(type::GetFromSignedInt( 1),  1 * type::scale_factor); \
    EXPECT_EQ(type::GetFromSignedInt( 2),  2 * type::scale_factor); \
    EXPECT_EQ(type::GetFromSignedInt(-1), -1 * type::scale_factor); \
    EXPECT_EQ(type::GetFromSignedInt(-2), -2 * type::scale_factor); \
}

DECL_GET_FROM_SIGNED_INT_TEST(fixed32)
#ifdef REALNUMB_INT128
DECL_GET_FROM_SIGNED_INT_TEST(fixed64)
#endif

// Tests of fixed<T>::GetFromFloat(v)

#define DECL_GET_FROM_FLOAT_TEST(type) \
TEST(type, GetFromFloat) \
{ \
    EXPECT_EQ(type::GetFromFloat( 0.0),  0 * type::scale_factor); \
    EXPECT_EQ(type::GetFromFloat( 1.0),  1 * type::scale_factor); \
    EXPECT_EQ(type::GetFromFloat( 2.0),  2 * type::scale_factor); \
    EXPECT_EQ(type::GetFromFloat(-1.0), -1 * type::scale_factor); \
    EXPECT_EQ(type::GetFromFloat(-2.0), -2 * type::scale_factor); \
    EXPECT_EQ(type::GetFromFloat(-4.7), static_cast<type::value_type>(-4.7 * type::scale_factor)); \
    const auto long_double_max = std::numeric_limits<long double>::max(); \
    const auto fixed_infinity = type::GetInfinity(); \
    const auto fixed_infinity_as_vt = *reinterpret_cast<const type::value_type*>(&fixed_infinity); \
    EXPECT_EQ(type::GetFromFloat( long_double_max),  fixed_infinity_as_vt); \
    EXPECT_EQ(type::GetFromFloat(-long_double_max), -fixed_infinity_as_vt); \
    EXPECT_EQ(type::GetFromFloat( std::numeric_limits<float>::infinity()),  fixed_infinity_as_vt); \
    EXPECT_EQ(type::GetFromFloat(-std::numeric_limits<float>::infinity()), -fixed_infinity_as_vt); \
}

DECL_GET_FROM_FLOAT_TEST(fixed32)
#ifdef REALNUMB_INT128
DECL_GET_FROM_FLOAT_TEST(fixed64)
#endif

// Tests of fixed<T>::fixed(v) and comparisons

#define DECL_INT_CONSTRUCTION_AND_COMPARE_TEST(type) \
TEST(type, IntConstructionAndCompare) \
{ \
    EXPECT_EQ(type( 0), type( 0)); \
    EXPECT_LT(type( 0), type( 1)); \
    EXPECT_GT(type( 0), type(-1)); \
    EXPECT_EQ(type(-10), type(-10)); \
    EXPECT_LT(type(-10), type( -9)); \
    EXPECT_GT(type(-10), type(-11)); \
}

DECL_INT_CONSTRUCTION_AND_COMPARE_TEST(fixed32)
#ifdef REALNUMB_INT128
DECL_INT_CONSTRUCTION_AND_COMPARE_TEST(fixed64)
#endif

// Tests of isfinite(fixed<T>)

#define DECL_ISFINITE_TEST(type) \
TEST(type, isfinite) \
{ \
    EXPECT_TRUE(isfinite(type(0))); \
    EXPECT_FALSE(isfinite( type::GetInfinity())); \
    EXPECT_FALSE(isfinite(-type::GetInfinity())); \
    EXPECT_FALSE(isfinite(type::GetNaN())); \
}

DECL_ISFINITE_TEST(fixed32)
#ifdef REALNUMB_INT128
DECL_ISFINITE_TEST(fixed64)
#endif

// Tests of isnan(fixed<T>)

#define DECL_ISNAN_TEST(type) \
TEST(type, isnan) \
{ \
    EXPECT_FALSE(isnan(type( 0))); \
    EXPECT_FALSE(isnan(type( 1))); \
    EXPECT_FALSE(isnan(type(-1))); \
    EXPECT_FALSE(isnan( type::GetInfinity())); \
    EXPECT_FALSE(isnan(-type::GetInfinity())); \
    EXPECT_FALSE(isnan( type::GetNegativeInfinity())); \
    EXPECT_TRUE(isnan(type::GetNaN())); \
    EXPECT_TRUE(isnan(type(std::numeric_limits<float>::quiet_NaN()))); \
    EXPECT_TRUE(isnan(type(std::numeric_limits<float>::signaling_NaN()))); \
    EXPECT_TRUE(isnan(type(std::numeric_limits<double>::quiet_NaN()))); \
    EXPECT_TRUE(isnan(type(std::numeric_limits<double>::signaling_NaN()))); \
    EXPECT_TRUE(isnan(type(std::numeric_limits<long double>::quiet_NaN()))); \
    EXPECT_TRUE(isnan(type(std::numeric_limits<long double>::signaling_NaN()))); \
}

DECL_ISNAN_TEST(fixed32)
#ifdef REALNUMB_INT128
DECL_ISNAN_TEST(fixed64)
#endif

// Regular tests

TEST(fixed32, IntCast)
{
    EXPECT_EQ(static_cast<int>(fixed32( 0)),  0);
    EXPECT_EQ(static_cast<int>(fixed32(-1)), -1);
    EXPECT_EQ(static_cast<int>(fixed32(-2)), -2);
    EXPECT_EQ(static_cast<int>(fixed32(+1)), +1);
    EXPECT_EQ(static_cast<int>(fixed32(+2)), +2);
}

#ifdef REALNUMB_INT128
TEST(fixed64, IntCast)
{
    EXPECT_EQ(static_cast<int>(fixed64( 0)),  0);
    EXPECT_EQ(static_cast<int>(fixed64(-1)), -1);
    EXPECT_EQ(static_cast<int>(fixed64(-2)), -2);
    EXPECT_EQ(static_cast<int>(fixed64(+1)), +1);
    EXPECT_EQ(static_cast<int>(fixed64(+2)), +2);
}
#endif

TEST(fixed32, FloatCast)
{
    EXPECT_EQ(static_cast<float>(fixed32( 0)),  0.0f);
    EXPECT_EQ(static_cast<float>(fixed32(-1)), -1.0f);
    EXPECT_EQ(static_cast<float>(fixed32(+1)), +1.0f);
}

#ifdef REALNUMB_INT128
TEST(fixed64, FloatCast)
{
    EXPECT_EQ(static_cast<float>(fixed64( 0)),  0.0f);
    EXPECT_EQ(static_cast<float>(fixed64(-1)), -1.0f);
    EXPECT_EQ(static_cast<float>(fixed64(+1)), +1.0f);
}
#endif

TEST(fixed32, DoubleCast)
{
    EXPECT_EQ(static_cast<double>(fixed32( 0)),  0.0);
    EXPECT_EQ(static_cast<double>(fixed32(-1)), -1.0);
    EXPECT_EQ(static_cast<double>(fixed32(+1)), +1.0);
}

#ifdef REALNUMB_INT128
TEST(fixed64, DoubleCast)
{
    EXPECT_EQ(static_cast<double>(fixed64( 0)),  0.0);
    EXPECT_EQ(static_cast<double>(fixed64(-1)), -1.0);
    EXPECT_EQ(static_cast<double>(fixed64(+1)), +1.0);
}
#endif

TEST(fixed32, FloatConstruction)
{
    EXPECT_EQ(fixed32(0.0), 0.0);
    EXPECT_EQ(fixed32(-1.0), -1.0);
    EXPECT_EQ(fixed32(+1.0), +1.0);

    EXPECT_EQ(fixed32(std::numeric_limits<float>::infinity()), fixed32::GetInfinity());
    EXPECT_EQ(fixed32(-std::numeric_limits<float>::infinity()), -fixed32::GetInfinity());
    EXPECT_EQ(fixed32(-std::numeric_limits<float>::infinity()), fixed32::GetNegativeInfinity());
    EXPECT_TRUE(isnan(fixed32(std::numeric_limits<float>::quiet_NaN())));
    EXPECT_TRUE(isnan(fixed32(std::numeric_limits<float>::signaling_NaN())));

    const auto range = 30000;
    for (auto i = -range; i < range; ++i)
    {
        EXPECT_EQ(fixed32(static_cast<float>(i)), i);
        EXPECT_EQ(fixed32(float(i)), i);
        EXPECT_EQ(fixed32(float(i)), fixed32(i));
    }
}

TEST(fixed32, GetMin)
{
    EXPECT_NEAR(static_cast<double>(fixed32::GetMin()), 0.001953125, 0.00001);
}

TEST(fixed32, GetMax)
{
    EXPECT_NEAR(static_cast<double>(fixed32::GetMax()), 4194303.99609375,
                0.0001);
}

TEST(fixed32, limits)
{
    EXPECT_NEAR(static_cast<double>(std::numeric_limits<fixed32>::max()), 4194303.99609375,
                0.0);
    EXPECT_NEAR(static_cast<double>(std::numeric_limits<fixed32>::lowest()), -4194303.99609375,
                0.0);
    EXPECT_NEAR(static_cast<double>(std::numeric_limits<fixed32>::min()), 0.001953125,
                0.0);
}

#ifdef REALNUMB_INT128
TEST(fixed64, GetMin)
{
    EXPECT_NEAR(static_cast<double>(fixed64::GetMin()), 5.9604644775390625e-08, 0.0);
}

TEST(fixed64, GetMax)
{
    EXPECT_NEAR(static_cast<double>(fixed64::GetMax()), 549755813888, 0.0);
}
#endif

TEST(fixed32, Equals)
{
    EXPECT_TRUE(fixed32(12) == fixed32(12.0f));
    EXPECT_FALSE(std::numeric_limits<fixed32>::quiet_NaN() == std::numeric_limits<fixed32>::quiet_NaN());
}

TEST(fixed32, NotEquals)
{
    EXPECT_TRUE(fixed32(-302) != fixed32(12.0f));
    EXPECT_FALSE(fixed32(-302) != fixed32(-302));
    EXPECT_TRUE(std::numeric_limits<fixed32>::quiet_NaN() != std::numeric_limits<fixed32>::quiet_NaN());
}

TEST(fixed32, less)
{
    EXPECT_TRUE(fixed32(-302) < fixed32(12.0f));
    EXPECT_TRUE(fixed32(40) < fixed32(44));
    EXPECT_FALSE(fixed32(76) < fixed32(31));
    EXPECT_TRUE(fixed32(0.001) < fixed32(0.002));
    EXPECT_TRUE(fixed32(0.000) < fixed32(0.01));
}

TEST(fixed32, greater)
{
    EXPECT_FALSE(fixed32(-302) > fixed32(12.0f));
    EXPECT_FALSE(fixed32(40) > fixed32(44));
    EXPECT_TRUE(fixed32(76) > fixed32(31));
}

TEST(fixed32, Addition)
{
    for (auto val = 0; val < 100; ++val)
    {
        fixed32 a{val};
        fixed32 b{val};
        EXPECT_EQ(a + b, fixed32(val * 2));
    }
}

TEST(fixed32, InfinityPlusValidIsInfinity)
{
    EXPECT_EQ(fixed32::GetInfinity() + 0, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() + 1, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() + 100, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() + -1, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() + -100, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() + fixed32::GetInfinity(), fixed32::GetInfinity());
}

TEST(fixed32, EqualSubtraction)
{
    for (auto val = 0; val < 100; ++val)
    {
        fixed32 a{val};
        fixed32 b{val};
        EXPECT_EQ(a - b, fixed32(0));
    }
}

TEST(fixed32, OppositeSubtraction)
{
    for (auto val = 0; val < 100; ++val)
    {
        fixed32 a{-val};
        fixed32 b{val};
        EXPECT_EQ(a - b, fixed32(val * -2));
    }
}

TEST(fixed32, Multiplication)
{
    for (auto val = 0; val < 100; ++val)
    {
        fixed32 a{val};
        EXPECT_EQ(a * a, fixed32(val * val));
    }
    EXPECT_EQ(fixed32(9) * fixed32(3), fixed32(27));
    EXPECT_EQ(fixed32(-5) * fixed32(-4), fixed32(20));
    EXPECT_EQ(fixed32(0.5) * fixed32(0.5), fixed32(0.25));
    EXPECT_EQ(fixed32(-0.05) * fixed32(0.05), fixed32(-0.0025));
    EXPECT_EQ(fixed32(181) * fixed32(181), fixed32(32761));
}

TEST(fixed32, Division)
{
    for (auto val = 1; val < 100; ++val)
    {
        fixed32 a{val};
        EXPECT_EQ(a / a, fixed32(1));
    }
    EXPECT_EQ(fixed32(9) / fixed32(3), fixed32(3));
    EXPECT_EQ(fixed32(81) / fixed32(9), fixed32(9));
    EXPECT_EQ(fixed32(-10) / fixed32(2), fixed32(-5));
    EXPECT_EQ(fixed32(1) / fixed32(2), fixed32(0.5));
    EXPECT_EQ(fixed32(7) / fixed32(3), fixed32(7.0/3.0));

    // Confirm int divided by fixed32 gets promoted to fixed32 divided by fixed32
    EXPECT_EQ(1 / fixed32(2), fixed32(0.5));
    EXPECT_EQ(2 / fixed32(2), fixed32(1));
    EXPECT_EQ(3 / fixed32(2), fixed32(1.5));
}

TEST(fixed32, log)
{
    ASSERT_FALSE(isfinite(static_cast<double>(log(0.0))));
    EXPECT_FALSE(isfinite(static_cast<double>(log(fixed32(0)))));

    ASSERT_TRUE(isnan(static_cast<double>(log(-1.0))));
    EXPECT_TRUE(isnan(static_cast<double>(log(fixed32(-1)))));

    ASSERT_NEAR(static_cast<double>(log(0.1)), -2.3025850929940455, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(0.1))), -2.3025850929940455, 0.051);

    ASSERT_NEAR(static_cast<double>(log(0.5)), -0.69314718055994529, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(0.5))), -0.69314718055994529, 0.01);

    ASSERT_NEAR(static_cast<double>(log(1.0)), 0.0, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(1.0))), 0.0, 0.01);

    ASSERT_NEAR(static_cast<double>(log(1.5)), 0.40546510810816438, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(1.5))), 0.40546510810816438, 0.01);

    ASSERT_NEAR(static_cast<double>(log(2.0)), 0.69314718055994529, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(2.0))), 0.69314718055994529, 0.012);

    ASSERT_NEAR(static_cast<double>(log(2.1)), 0.74193734472937733, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(2.1))), 0.74193734472937733, 0.0096);

    ASSERT_NEAR(static_cast<double>(log(2.75)), 1.0116009116784799, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(2.75))), 0.994140625, 0.0001);

    ASSERT_NEAR(static_cast<double>(log(4.5)), 1.5040773967762742, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(4.5))), 1.5040773967762742, 0.028);

    ASSERT_NEAR(static_cast<double>(log(31.21)), 3.440738556282688, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(31.21))), log(31.21), 0.25);

    // Error gets pretty bad...
    ASSERT_NEAR(static_cast<double>(log(491.721)), 6.1979114824747752, 0.01);
    EXPECT_NEAR(static_cast<double>(log(fixed32(491.721))), log(491.721), 1.517);
    
    EXPECT_EQ(static_cast<double>(log(fixed32::GetInfinity())), log(std::numeric_limits<double>::infinity()));
}

TEST(fixed32, exp)
{
    EXPECT_NEAR(static_cast<double>(exp(fixed32(0))), exp(0.0), 0.01);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(0.4))), exp(0.4), 0.02);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(0.9))), exp(0.9), 0.02);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(1.0))), exp(1.0), 0.02);

    ASSERT_NEAR(static_cast<double>(exp(1.34)), 3.8190435053663361, 0.001);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(1.34))), exp(1.34), 0.019);

    ASSERT_NEAR(static_cast<double>(exp(2.5)), 12.182493960703473, 0.01);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(2.5))), exp(2.5), 0.04);

    ASSERT_NEAR(static_cast<double>(exp(3.15)), 23.336064580942711, 0.2);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(3.15))), 23.23828125, 0.01);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(3.15))), exp(3.15), 0.1);

    ASSERT_NEAR(static_cast<double>(exp(4.8)), 121.51041751873485, 0.2);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(4.8))), 121.19140625, 0.01);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(4.8))), exp(4.8), 0.4);
    
    ASSERT_NEAR(static_cast<double>(exp(7.1)), 1211.9670744925763, 0.2);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(7.1))), 1210.525390625, 0.01);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(7.1))), exp(7.1), 1.6);

    ASSERT_NEAR(static_cast<double>(exp(8.9)), 7331.9735391559952, 0.2);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(8.9))), 7318.447265625, 0.01);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(8.9))), exp(8.9), 13.55);

    ASSERT_NEAR(static_cast<double>(exp(10.1)), 24343.009424408381, 0.2);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(10.1))), 24322.119140625, 0.01);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(10.1))), exp(10.1), 22.0);

    ASSERT_NEAR(static_cast<double>(exp(12.5)), 268337.28652087448, 0.2);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(12.5))), 267662.830078125, 0.01);
    
    ASSERT_NEAR(static_cast<double>(exp(-1.0)), 0.36787944117144233, 0.0001);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(-1))), 0.36787944117144233, 0.001);
    
    ASSERT_NEAR(static_cast<double>(exp(-2.0)), 0.1353352832366127, 0.0001);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(-2))), 0.13671875, 0.001);

    ASSERT_NEAR(static_cast<double>(exp(-4.0)), 0.018315638888734179, 0.0001);
    EXPECT_NEAR(static_cast<double>(exp(fixed32(-4))), 0.021484375, 0.001);
}

TEST(fixed32, intpow)
{
    ASSERT_NEAR(static_cast<double>(pow(0.0, 0)), 1.0, 0.0);
    ASSERT_NEAR(static_cast<double>(pow(0.0, +1)), 0.0, 0.0);
    ASSERT_FALSE(isfinite(static_cast<double>(pow(0.0, -1))));

    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), 0)), 1.0, 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), +1)), 0.0, 0.0);
    EXPECT_FALSE(isfinite(static_cast<double>(pow(fixed32(0), -1))));

    EXPECT_EQ(static_cast<double>(pow(fixed32::GetNegativeInfinity(), -1)),
              pow(-std::numeric_limits<double>::infinity(), -1));
    EXPECT_EQ(static_cast<double>(pow(fixed32::GetNegativeInfinity(), +1)),
              pow(-std::numeric_limits<double>::infinity(), +1));
    EXPECT_EQ(static_cast<double>(pow(fixed32::GetNegativeInfinity(), +2)),
              pow(-std::numeric_limits<double>::infinity(), +2));
    EXPECT_EQ(static_cast<double>(pow(fixed32::GetInfinity(), +2)),
              pow(std::numeric_limits<double>::infinity(), +2));
    EXPECT_EQ(static_cast<double>(pow(fixed32::GetInfinity(), -2)),
              pow(std::numeric_limits<double>::infinity(), -2));

    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), 1)), pow(0.0, 1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), 0)), pow(0.0, 0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), 0)), pow(1.0, 0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), +44)), pow(1.0, +44), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), -44)), pow(1.0, -44), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), +1)), pow(+1.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), +1)), pow(-1.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), -1)), pow(+1.0, -1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), -1)), pow(-1.0, -1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+2), +1)), pow(+2.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), +1)), pow(+3.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), +2)), pow(+3.0, +2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), +3)), pow(+3.0, +3), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), +2)), pow(+9.0, +2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), -1)), pow(+9.0, -1), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), -1)), pow(+3.0, -1), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), -2)), pow(+9.0, -2), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), -2)), pow(+3.0, -2), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+10), -2)), pow(+10.0, -2), 0.01);
}

TEST(fixed32, regpow)
{
    ASSERT_NEAR(pow(0.0, 0.0), 1.0, 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), fixed32(0))), pow(0.0, 0.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), fixed32(0))), pow(1.0, 0.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), fixed32(+44.2))), pow(1.0, +44.2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), fixed32(-44.2))), pow(1.0, -44.2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), fixed32(+1))), pow(+1.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), fixed32(+1))), pow(-1.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), fixed32(-1))), pow(+1.0, -1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), fixed32(-1))), pow(-1.0, -1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+2), fixed32(+1))), pow(+2.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(+1))), pow(+3.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(+2))), pow(+3.0, +2.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(+3))), pow(+3.0, +3.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), fixed32(+2))), pow(+9.0, +2.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), fixed32(-1))), pow(+9.0, -1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(-1))), pow(+3.0, -1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), fixed32(-2))), pow(+9.0, -2.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(-2))), pow(+3.0, -2.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+10), fixed32(-2))), pow(+10.0, -2.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-10), fixed32(-2))), pow(-10.0, -2.0), 0.01);
    
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+4), fixed32(+2.3))), pow(+4.0, +2.3), 0.97);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+4), fixed32(-2.3))), pow(+4.0, -2.3), 0.1);

    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(+2.3))), pow(+3.1, +2.3), 0.75);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(-2.3))), pow(+3.1, -2.3), 0.1);

    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(+4.3))), pow(+3.1, +4.3), 12.3);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(-4.3))), pow(+3.1, -4.3), 0.3);

    EXPECT_EQ(isnan(static_cast<double>(pow(fixed32(-4), fixed32(+2.3)))), isnan(pow(-4.0, +2.3)));
    EXPECT_EQ(isnan(static_cast<double>(pow(fixed32(-4), fixed32(-2.3)))), isnan(pow(-4.0, -2.3)));
}

TEST(fixed32, sqrt)
{
    const auto tolerance = static_cast<double>(fixed32::GetMin());

    EXPECT_TRUE(isnan(sqrt(fixed32{-1})));
    EXPECT_TRUE(isnan(sqrt(fixed32::GetNaN())));
    EXPECT_EQ(sqrt(fixed32{0}), fixed32{0});
    EXPECT_EQ(sqrt(fixed32::GetInfinity()), fixed32::GetInfinity());
    EXPECT_EQ(sqrt(fixed32::GetMin()), fixed32(0.046875));
    EXPECT_EQ((sqrt(fixed32::GetMin()) * sqrt(fixed32::GetMin())), fixed32::GetMin());
    EXPECT_EQ(sqrt(fixed32{1}), fixed32{1});
    EXPECT_NEAR(static_cast<double>(sqrt(fixed32{0.25})), 0.5, 0.0);
    EXPECT_NEAR(static_cast<double>(sqrt(fixed32{0.0625})), 0.25, tolerance);

    for (auto i = 0; i < 10000; ++i)
    {
        EXPECT_NEAR(static_cast<double>(sqrt(fixed32(i))), sqrt(double(i)), 0.01);
    }

    for (auto v = 1.0; v > 0.0; v /= 1.1)
    {
        const auto fixedv = fixed32{v};
        if (fixedv == fixed32{0})
        {
            break;
        }
        EXPECT_LE(abs((sqrt(fixedv) * sqrt(fixedv)) - fixedv), fixed32::GetMin());
        EXPECT_NEAR(static_cast<double>(sqrt(fixedv)), sqrt(static_cast<double>(fixedv)), tolerance * 5);
    }
    const auto maxV = static_cast<double>(std::numeric_limits<fixed32>::max());
    for (auto v = 1.0; v < maxV; v *= 1.1)
    {
        const auto fixedv = fixed32{v};
        EXPECT_NEAR(static_cast<double>(sqrt(fixedv)), sqrt(v), tolerance);
        //EXPECT_LE(abs(Square(sqrt(fixedv)) - fixedv), fixed32::GetMin() * 5);
    }
}

TEST(fixed32, hypot)
{
    for (auto i = 0; i < 100; ++i)
    {
        for (auto j = 0; i < 100; ++i)
        {
            EXPECT_NEAR(static_cast<double>(hypot(fixed32(i), fixed32(j))),
                        hypot(double(i), double(j)), 0.01);
        }
    }
}

#ifdef REALNUMB_INT128
TEST(fixed64, hypot)
{
    for (auto i = 0; i < 100; ++i)
    {
        for (auto j = 0; i < 100; ++i)
        {
            EXPECT_NEAR(static_cast<double>(hypot(fixed64(i), fixed64(j))),
                        hypot(double(i), double(j)), 0.001);
        }
    }
}
#endif

TEST(fixed32, sin)
{
    constexpr auto pi = double{3.14159265358979323846264338327950288};
    EXPECT_NEAR(static_cast<double>(sin(fixed32(0))), 0.0, 0.005);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+pi/4))), std::sin(+pi/4), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-pi/4))), std::sin(-pi/4), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+1))), std::sin(+1), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-1))), std::sin(-1), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+pi/2))), std::sin(+pi/2), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-pi/2))), std::sin(-pi/2), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+2))), std::sin(+2), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-2))), std::sin(-2), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+3))), std::sin(+3), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-3))), std::sin(-3), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+pi))), std::sin(+pi), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-pi))), std::sin(-pi), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+4))), std::sin(+4), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-4))), std::sin(-4), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+5))), std::sin(+5), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-5))), std::sin(-5), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+pi*2))), std::sin(+pi*2), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-pi*2))), std::sin(-pi*2), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+8))), std::sin(+8), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-8))), std::sin(-8), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(+10))), std::sin(+10), 0.015);
    EXPECT_NEAR(static_cast<double>(sin(fixed32(-10))), std::sin(-10), 0.015);
}

#ifdef REALNUMB_INT128
TEST(fixed64, sin)
{
    constexpr auto pi = double{3.14159265358979323846264338327950288};
    EXPECT_NEAR(static_cast<double>(sin(fixed64(0))), 0.0, 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+pi/4))), std::sin(+pi/4), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-pi/4))), std::sin(-pi/4), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+1))), std::sin(+1), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-1))), std::sin(-1), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+pi/2))), std::sin(+pi/2), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-pi/2))), std::sin(-pi/2), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+2))), std::sin(+2), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-2))), std::sin(-2), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+3))), std::sin(+3), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-3))), std::sin(-3), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+pi))), std::sin(+pi), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-pi))), std::sin(-pi), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+4))), std::sin(+4), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-4))), std::sin(-4), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+5))), std::sin(+5), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-5))), std::sin(-5), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+pi*2))), std::sin(+pi*2), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-pi*2))), std::sin(-pi*2), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+8))), std::sin(+8), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-8))), std::sin(-8), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(+10))), std::sin(+10), 0.002);
    EXPECT_NEAR(static_cast<double>(sin(fixed64(-10))), std::sin(-10), 0.002);
}
#endif

TEST(fixed32, cos)
{
    constexpr auto pi = double{3.14159265358979323846264338327950288};
    EXPECT_NEAR(static_cast<double>(cos(fixed32(0))), 1.0, 0.01);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(+1))), std::cos(+1.0), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(-1))), std::cos(-1.0), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(+2))), std::cos(+2.0), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(-2))), std::cos(-2.0), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(+pi/2))), std::cos(+pi/2), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(-pi/2))), std::cos(-pi/2), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(+3))), std::cos(+3.0), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(-3))), std::cos(-3.0), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(+8))), std::cos(+8), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(-8))), std::cos(-8), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(+10))), std::cos(+10), 0.015);
    EXPECT_NEAR(static_cast<double>(cos(fixed32(-10))), std::cos(-10), 0.015);
}

#ifdef REALNUMB_INT128
TEST(fixed64, cos)
{
    constexpr auto pi = double{3.14159265358979323846264338327950288};
    EXPECT_NEAR(static_cast<double>(cos(fixed64(0))), 1.0, 0.01);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(+1))), std::cos(+1.0), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(-1))), std::cos(-1.0), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(+2))), std::cos(+2.0), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(-2))), std::cos(-2.0), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(+pi/2))), std::cos(+pi/2), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(-pi/2))), std::cos(-pi/2), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(+3))), std::cos(+3.0), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(-3))), std::cos(-3.0), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(+8))), std::cos(+8), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(-8))), std::cos(-8), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(+10))), std::cos(+10), 0.002);
    EXPECT_NEAR(static_cast<double>(cos(fixed64(-10))), std::cos(-10), 0.002);
}
#endif

TEST(fixed32, atan)
{
    EXPECT_NEAR(static_cast<double>(atan(fixed32::GetInfinity())),
                atan(std::numeric_limits<double>::infinity()), 0.001);
    EXPECT_NEAR(static_cast<double>(atan(fixed32::GetNegativeInfinity())),
                atan(-std::numeric_limits<double>::infinity()), 0.001);
}

TEST(fixed32, atan2_specials)
{
    EXPECT_TRUE(isnan(static_cast<double>(atan2(fixed32(0), fixed32(0)))));
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(+1), fixed32(0))), atan2(+1.0, 0.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(-1), fixed32(0))), atan2(-1.0, 0.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(0), fixed32(+1))), atan2(0.0, +1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(0), fixed32(-1))), atan2(0.0, -1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(+1), fixed32(+1))), atan2(+1.0, +1.0), 0.05);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(+1), fixed32(-1))), atan2(+1.0, -1.0), 0.05);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(-1), fixed32(+1))), atan2(-1.0, +1.0), 0.05);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(-1), fixed32(-1))), atan2(-1.0, -1.0), 0.05);
}

TEST(fixed32, atan2_angles)
{
    constexpr auto pi = double{3.14159265358979323846264338327950288};
    for (auto angleInDegs = -90; angleInDegs < +90; ++angleInDegs)
    {
        const auto angle = angleInDegs * pi / 180;
        const auto s = sin(angle);
        const auto c = cos(angle);
        EXPECT_NEAR(static_cast<double>(atan2(fixed32(s), fixed32(c))), angle, 0.05);
    }
}

TEST(fixed32, Max)
{
    const auto max_internal_val = std::numeric_limits<int32_t>::max() - 1;
    const auto max_fixed32 = *reinterpret_cast<const fixed32*>(&max_internal_val);
    
    EXPECT_EQ(fixed32::GetMax(), fixed32::GetMax());
    EXPECT_EQ(fixed32::GetMax(), max_fixed32);
    //EXPECT_EQ(static_cast<long double>(fixed32::GetMax()), 131071.99993896484375000000L);
    //std::cout << std::setprecision(22) << static_cast<long double>(fixed32::GetMax()) << std::endl;
    switch (fixed32::fraction_bits)
    {
        case  9:
            EXPECT_NEAR(static_cast<double>(fixed32::GetMax()), 4.1943e+06, 4.0);
            break;
        case 14:
            EXPECT_EQ(static_cast<double>(fixed32::GetMax()), 131071.9998779296875);
            break;
    }

    EXPECT_GT(fixed32::GetMax(), fixed32(0));
    EXPECT_GT(fixed32::GetMax(), fixed32::GetMin());
    EXPECT_GT(fixed32::GetMax(), fixed32::GetLowest());
    EXPECT_GT(fixed32::GetMax(), fixed32((1 << (31u - fixed32::fraction_bits)) - 1));
}

TEST(fixed32, Min)
{
    EXPECT_EQ(fixed32::GetMin(), fixed32::GetMin());
    EXPECT_EQ(fixed32::GetMin(), fixed32(0, 1));
    switch (fixed32::fraction_bits)
    {
        case  9:
            EXPECT_NEAR(static_cast<double>(fixed32::GetMin()), 0.00195312, 0.0000001);
            break;
        case 14:
            EXPECT_EQ(static_cast<double>(fixed32::GetMin()), 0.00006103515625000000);
            break;
    }

    EXPECT_LT(fixed32::GetMin(), fixed32::GetMax());
    
    EXPECT_GT(fixed32::GetMin(), fixed32(0));
    EXPECT_GT(fixed32::GetMin(), fixed32::GetLowest());
}

TEST(fixed32, Lowest)
{
    const auto lowest_internal_val = std::numeric_limits<int32_t>::min() + 2;
    const auto lowest_fixed32 = *reinterpret_cast<const fixed32*>(&lowest_internal_val);

    EXPECT_EQ(fixed32::GetLowest(), fixed32::GetLowest());
    EXPECT_EQ(fixed32::GetLowest(), lowest_fixed32);
    //EXPECT_EQ(static_cast<long double>(fixed32::GetLowest()), -131072.00000000000000000000L);
    //std::cout << std::setprecision(22) << static_cast<long double>(fixed32::GetLowest()) << std::endl;
    switch (fixed32::fraction_bits)
    {
        case  9:
            EXPECT_NEAR(static_cast<double>(fixed32::GetLowest()), -4.1943e+06, 4.0);
            break;
        case 14:
            EXPECT_EQ(static_cast<double>(fixed32::GetLowest()), -131071.9998779296875);
            break;
    }
    EXPECT_LT(fixed32::GetLowest(), fixed32(0));
    EXPECT_LT(fixed32::GetLowest(), fixed32(-((1 << (31u - fixed32::fraction_bits)) - 1), 0u));
    EXPECT_LT(fixed32::GetLowest(), fixed32(-((1 << (31u - fixed32::fraction_bits)) - 1), (1u << fixed32::fraction_bits) - 1u));
    EXPECT_EQ(fixed32::GetLowest(), -fixed32::GetMax());
}

TEST(fixed32, SubtractingFromLowestGetsNegativeInfinity)
{
    EXPECT_EQ(fixed32::GetLowest() - fixed32::GetMin(), fixed32::GetNegativeInfinity());
    EXPECT_EQ(fixed32::GetLowest() - 1, fixed32::GetNegativeInfinity());
}

TEST(fixed32, AddingToMaxGetsInfinity)
{
    EXPECT_EQ(fixed32::GetMax() + fixed32::GetMin(), fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetMax() + 1, fixed32::GetInfinity());
}

TEST(fixed32, MinusInfinityEqualsNegativeInfinity)
{
    EXPECT_EQ(-fixed32::GetInfinity(), fixed32::GetNegativeInfinity());
}

TEST(fixed32, InfinityEqualsMinusNegativeInfinity)
{
    EXPECT_EQ(fixed32::GetInfinity(), -fixed32::GetNegativeInfinity());
}

TEST(fixed32, InifnityTimesPositiveIsInfinity)
{
    EXPECT_EQ(fixed32::GetInfinity() * 1, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() * 2, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() * 0.5, fixed32::GetInfinity());
}

TEST(fixed32, InifnityDividedByPositiveIsInfinity)
{
    EXPECT_EQ(fixed32::GetInfinity() / 1, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() / 2, fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() / 0.5, fixed32::GetInfinity());
}

TEST(fixed32, InfinityDividedByInfinityIsNaN)
{
    EXPECT_TRUE(isnan(fixed32::GetInfinity() / fixed32::GetInfinity()));
}

TEST(fixed32, InifnityTimesNegativeIsNegativeInfinity)
{
    EXPECT_EQ(fixed32::GetInfinity() * -1, -fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() * -2, -fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() * -0.5, -fixed32::GetInfinity());
}

TEST(fixed32, InifnityDividedByNegativeIsNegativeInfinity)
{
    EXPECT_EQ(fixed32::GetInfinity() / -1, -fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() / -2, -fixed32::GetInfinity());
    EXPECT_EQ(fixed32::GetInfinity() / -0.5, -fixed32::GetInfinity());
}

TEST(fixed32, InfinityMinusNegativeInfinityIsInfinity)
{
    EXPECT_EQ(fixed32::GetInfinity() - -fixed32::GetInfinity(), fixed32::GetInfinity());
}

TEST(fixed32, NegativeInfinityMinusInfinityIsNegativeInfinity)
{
    EXPECT_EQ(-fixed32::GetInfinity() - fixed32::GetInfinity(), -fixed32::GetInfinity());
}

TEST(fixed32, NaN)
{
    EXPECT_TRUE(isnan(fixed32::GetNaN()));
    EXPECT_TRUE(isnan(fixed32::GetInfinity() / fixed32::GetInfinity()));
    EXPECT_TRUE(isnan(fixed32::GetInfinity() - fixed32::GetInfinity()));
    EXPECT_TRUE(isnan(-fixed32::GetInfinity() - -fixed32::GetInfinity()));
    EXPECT_TRUE(isnan(-fixed32::GetInfinity() + fixed32::GetInfinity()));

    EXPECT_FALSE(isnan(fixed32{0}));
    EXPECT_FALSE(isnan(fixed32{10.0f}));
    EXPECT_FALSE(isnan(fixed32{-10.0f}));
    EXPECT_FALSE(isnan(fixed32::GetInfinity()));
    EXPECT_FALSE(isnan(fixed32::GetNegativeInfinity()));
    EXPECT_FALSE(isnan(fixed32::GetMax()));
    EXPECT_FALSE(isnan(fixed32::GetMin()));
    EXPECT_FALSE(isnan(fixed32::GetLowest()));
}

TEST(fixed32, InfinityTimesZeroIsNaN)
{
    EXPECT_TRUE(isnan(fixed32::GetInfinity() * 0));
}

TEST(fixed32, Comparators)
{
    EXPECT_FALSE(fixed32::GetNaN() > 0.0f);
    EXPECT_FALSE(fixed32::GetNaN() < 0.0f);
    EXPECT_FALSE(fixed32::GetNaN() == 0.0f);
    EXPECT_TRUE(fixed32::GetNaN() != 0.0f);
    EXPECT_FALSE(fixed32::GetNaN() == fixed32::GetNaN());
}

TEST(fixed32, BiggerValsIdenticallyInaccurate)
{
    // Check that Real doesn't suffer from inconstent inaccuracy (like float has depending on
    // the float's value).
    auto last_delta = float(0);
    auto val = fixed32{1};
    const auto max = sizeof(fixed32) * 8 - fixed32::fraction_bits - 1;
    for (auto i = decltype(max){0}; i < max; ++i)
    {
        const auto next = nextafter(val, std::numeric_limits<fixed32>::max());
        const auto delta = static_cast<float>(next - val);
        ASSERT_EQ(val + (delta / 2.0f), val);
#if 0
        std::cout << std::hexfloat;
        std::cout << "For " << std::setw(7) << val << ", delta of next value is " << std::setw(7) << delta;
        std::cout << std::defaultfloat;
        std::cout << ": ie. at " << std::setw(6) << val;
        std::cout << std::fixed;
        std::cout << ", delta is " << delta;
        std::cout << std::endl;
#endif
        val *= 2;
        if (last_delta != 0)
        {
            ASSERT_EQ(delta, last_delta);
        }
        last_delta = delta;
    }
}

TEST(fixed32, AdditionAssignment)
{
    fixed32 foo;
    foo = 0;
    foo += fixed32::GetNegativeInfinity();
    EXPECT_EQ(foo, -std::numeric_limits<fixed32>::infinity());
    foo = std::numeric_limits<fixed32>::lowest();
    foo += -1;
    EXPECT_EQ(foo, fixed32::GetNegativeInfinity());
}

TEST(fixed32, SubtractionAssignment)
{
    fixed32 foo;
    foo = 0;
    foo -= 0;
    EXPECT_EQ(foo, fixed32{0});
    foo = 0;
    foo -= 1;
    EXPECT_EQ(foo, fixed32{-1});
    foo = std::numeric_limits<fixed32>::max();
    foo -= fixed32{-2};
    EXPECT_EQ(foo, fixed32::GetInfinity());
}

TEST(fixed32, MultiplicationAssignment)
{
    fixed32 foo;
    foo = fixed32::GetNaN();
    foo *= fixed32{0};
    EXPECT_TRUE(foo.isnan());
    foo = 0;
    foo *= fixed32::GetNaN();
    EXPECT_TRUE(foo.isnan());
    foo = std::numeric_limits<fixed32>::min();
    foo *= std::numeric_limits<fixed32>::min();
    EXPECT_EQ(foo, fixed32(0));
    foo = std::numeric_limits<fixed32>::lowest();
    foo *= 2;
    EXPECT_EQ(foo, fixed32::GetNegativeInfinity());
}

TEST(fixed32, DivisionAssignment)
{
    fixed32 foo;
    foo = fixed32::GetNaN();
    foo /= fixed32{1};
    EXPECT_TRUE(foo.isnan());
    foo = 0;
    foo /= fixed32::GetNaN();
    EXPECT_TRUE(foo.isnan());
    foo = 1;
    foo /= fixed32::GetInfinity();
    EXPECT_EQ(foo, fixed32(0));
    foo = std::numeric_limits<fixed32>::max();
    ASSERT_EQ(foo, std::numeric_limits<fixed32>::max());
    foo /= fixed32(0.5f);
    EXPECT_EQ(foo, fixed32::GetInfinity());
    foo = std::numeric_limits<fixed32>::lowest();
    ASSERT_TRUE(foo.isfinite());
    foo /= fixed32(0.5);
    EXPECT_EQ(foo, fixed32::GetNegativeInfinity());
}

TEST(fixed32, GetSign)
{
    fixed32 foo;
    foo = 0;
    EXPECT_GT(foo.getsign(), 0);
    foo = fixed32(-32.412);
    EXPECT_LT(foo.getsign(), 0);
}

TEST(fixed32, StreamOut)
{
    std::ostringstream os;
    os << fixed32(2.2f);
    EXPECT_STREQ(os.str().c_str(), "2.19922");
}

#ifdef REALNUMB_INT128
TEST(fixed64, StreamOut)
{
    std::ostringstream os;
    os << fixed64(2.2f);
    EXPECT_STREQ(os.str().c_str(), "2.2");
}
#endif

TEST(fixed, Int32TypeAnd0bits)
{
    using fixed = fixed<std::int32_t, 0>;
    
    const auto zero = fixed(0);
    EXPECT_TRUE(zero == zero);
    EXPECT_EQ(zero, zero);

    const auto one = fixed(1);
    EXPECT_TRUE(one == one);
    EXPECT_EQ(one, one);

    EXPECT_NE(one, zero);
    EXPECT_NE(zero, one);
    EXPECT_GT(one, zero);
    EXPECT_GE(one, zero);
    EXPECT_GE(one, one);
    EXPECT_LT(zero, one);
    EXPECT_LE(zero, one);
    
    const auto two = one + one;
    EXPECT_NE(one, two);
    EXPECT_GT(two, one);
    EXPECT_GT(two, zero);
    
    EXPECT_EQ(one * one, one);
    EXPECT_EQ(one * two, two);
    EXPECT_EQ(two / two, one);
    EXPECT_EQ(two - two, zero);
}

TEST(fixed, less)
{
    using fixed_32_0 = fixed<std::int32_t, 0>;
    EXPECT_LT(fixed_32_0(0), fixed_32_0(1));
}

TEST(fixed, nextafter)
{
    using fixed_32_0 = fixed<std::int32_t, 0>;
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_0(0), fixed_32_0( 0))),  0.0);
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_0(0), fixed_32_0(+1))), +1.0);
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_0(0), fixed_32_0(-1))), -1.0);

    using fixed_32_1 = fixed<std::int32_t, 1>;
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_1(0), fixed_32_1( 0))),  0.0);
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_1(0), fixed_32_1(+1))), +0.5);
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_1(0), fixed_32_1(-1))), -0.5);

    using fixed_32_2 = fixed<std::int32_t, 2>;
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_2(0), fixed_32_2( 0))),  0.0);
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_2(0), fixed_32_2(+1))), +0.25);
    EXPECT_EQ(static_cast<double>(nextafter(fixed_32_2(0), fixed_32_2(-1))), -0.25);
}
