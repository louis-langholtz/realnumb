#include <gtest/gtest.h>

#include <realnumb/fixed.hpp>
#include <realnumb/fixed_limits.hpp>

#include <iostream>

using namespace realnumb;

template <typename T>
class fixed_test: public testing::Test {
public:
    using type = T;
};

using fixed_types = ::testing::Types<
    ::realnumb::fixed32
#ifdef REALNUMB_INT128
    , ::realnumb::fixed64
#endif
>;
TYPED_TEST_SUITE(fixed_test, fixed_types);

TYPED_TEST(fixed_test, to_value_from_unsigned)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::to_value(0u),  0 * type::scale_factor);
    EXPECT_EQ(type::to_value(1u),  1 * type::scale_factor);
    EXPECT_EQ(type::to_value(2u),  2 * type::scale_factor);
}

TYPED_TEST(fixed_test, to_value_from_signed)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::to_value( 0),  0 * type::scale_factor);
    EXPECT_EQ(type::to_value( 1),  1 * type::scale_factor);
    EXPECT_EQ(type::to_value( 2),  2 * type::scale_factor);
    EXPECT_EQ(type::to_value(-1), -1 * type::scale_factor);
    EXPECT_EQ(type::to_value(-2), -2 * type::scale_factor);
}

TYPED_TEST(fixed_test, to_value_from_floating)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::to_value( 0.0),  0 * type::scale_factor);
    EXPECT_EQ(type::to_value( 1.0),  1 * type::scale_factor);
    EXPECT_EQ(type::to_value( 2.0),  2 * type::scale_factor);
    EXPECT_EQ(type::to_value(-1.0), -1 * type::scale_factor);
    EXPECT_EQ(type::to_value(-2.0), -2 * type::scale_factor);
    EXPECT_EQ(type::to_value(-4.7),
              static_cast<typename type::value_type>(-4.7 * type::scale_factor));
    const auto long_double_max = std::numeric_limits<long double>::max();
    const auto fixed_infinity = type::get_positive_infinity();
    const auto fixed_infinity_as_vt = *reinterpret_cast<const typename type::value_type*>(&fixed_infinity);
    EXPECT_EQ(type::to_value( long_double_max),  fixed_infinity_as_vt);
    EXPECT_EQ(type::to_value(-long_double_max), -fixed_infinity_as_vt);
    EXPECT_EQ(type::to_value( std::numeric_limits<float>::infinity()),  fixed_infinity_as_vt);
    EXPECT_EQ(type::to_value(-std::numeric_limits<float>::infinity()), -fixed_infinity_as_vt);
}

TYPED_TEST(fixed_test, int_construction_and_compare)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type( 0), type( 0));
    EXPECT_LT(type( 0), type( 1));
    EXPECT_GT(type( 0), type(-1));
    EXPECT_EQ(type(-10), type(-10));
    EXPECT_LT(type(-10), type( -9));
    EXPECT_GT(type(-10), type(-11));
}

TYPED_TEST(fixed_test, IntCast)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(static_cast<int>(type( 0)),  0);
    EXPECT_EQ(static_cast<int>(type(-1)), -1);
    EXPECT_EQ(static_cast<int>(type(-2)), -2);
    EXPECT_EQ(static_cast<int>(type(+1)), +1);
    EXPECT_EQ(static_cast<int>(type(+2)), +2);
}

TYPED_TEST(fixed_test, FloatCast)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(static_cast<float>(type( 0)),  0.0f);
    EXPECT_EQ(static_cast<float>(type(-1)), -1.0f);
    EXPECT_EQ(static_cast<float>(type(+1)), +1.0f);
}

TYPED_TEST(fixed_test, DoubleCast)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(static_cast<double>(type( 0)),  0.0);
    EXPECT_EQ(static_cast<double>(type(-1)), -1.0);
    EXPECT_EQ(static_cast<double>(type(+1)), +1.0);
}

TYPED_TEST(fixed_test, FloatConstruction)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type(0.0), 0.0);
    EXPECT_EQ(type(-1.0), -1.0);
    EXPECT_EQ(type(+1.0), +1.0);
    EXPECT_EQ(type(std::numeric_limits<float>::infinity()), type::get_positive_infinity());
    EXPECT_EQ(type(-std::numeric_limits<float>::infinity()), -type::get_positive_infinity());
    EXPECT_EQ(type(-std::numeric_limits<float>::infinity()), type::get_negative_infinity());
    const auto range = 30000;
    for (auto i = -range; i < range; ++i)
    {
        EXPECT_EQ(type(static_cast<float>(i)), i);
        EXPECT_EQ(type(float(i)), i);
        EXPECT_EQ(type(float(i)), type(i));
    }
}

TEST(fixed, get_min)
{
    EXPECT_NEAR(static_cast<double>(fixed32::get_min()), 0.001953125, 0.00001);
#ifdef REALNUMB_INT128
    EXPECT_NEAR(static_cast<double>(fixed64::get_min()), 5.9604644775390625e-08, 0.0);
#endif
}

TEST(fixed, get_max)
{
    EXPECT_NEAR(static_cast<double>(fixed32::get_max()), 4194303.99609375, 0.0001);
#ifdef REALNUMB_INT128
    EXPECT_NEAR(static_cast<double>(fixed64::get_max()), 549755813888, 0.0);
#endif
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
    EXPECT_EQ(fixed32::get_positive_infinity() + 0, fixed32::get_positive_infinity());
    EXPECT_EQ(fixed32::get_positive_infinity() + 1, fixed32::get_positive_infinity());
    EXPECT_EQ(fixed32::get_positive_infinity() + 100, fixed32::get_positive_infinity());
    EXPECT_EQ(fixed32::get_positive_infinity() + -1, fixed32::get_positive_infinity());
    EXPECT_EQ(fixed32::get_positive_infinity() + -100, fixed32::get_positive_infinity());
    EXPECT_EQ(fixed32::get_positive_infinity() + fixed32::get_positive_infinity(), fixed32::get_positive_infinity());
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

TEST(fixed32, Max)
{
    const auto max_internal_val = std::numeric_limits<int32_t>::max() - 1;
    const auto max_fixed32 = *reinterpret_cast<const fixed32*>(&max_internal_val);
    
    EXPECT_EQ(fixed32::get_max(), fixed32::get_max());
    EXPECT_EQ(fixed32::get_max(), max_fixed32);
    //EXPECT_EQ(static_cast<long double>(fixed32::get_max()), 131071.99993896484375000000L);
    //std::cout << std::setprecision(22) << static_cast<long double>(fixed32::get_max()) << std::endl;
    switch (fixed32::fraction_bits)
    {
        case  9:
            EXPECT_NEAR(static_cast<double>(fixed32::get_max()), 4.1943e+06, 4.0);
            break;
        case 14:
            EXPECT_EQ(static_cast<double>(fixed32::get_max()), 131071.9998779296875);
            break;
    }

    EXPECT_GT(fixed32::get_max(), fixed32(0));
    EXPECT_GT(fixed32::get_max(), fixed32::get_min());
    EXPECT_GT(fixed32::get_max(), fixed32::get_lowest());
    EXPECT_GT(fixed32::get_max(), fixed32((1 << (31u - fixed32::fraction_bits)) - 1));
}

TEST(fixed32, Min)
{
    EXPECT_EQ(fixed32::get_min(), fixed32::get_min());
    EXPECT_EQ(fixed32::get_min(), fixed32(0, 1));
    switch (fixed32::fraction_bits)
    {
        case  9:
            EXPECT_NEAR(static_cast<double>(fixed32::get_min()), 0.00195312, 0.0000001);
            break;
        case 14:
            EXPECT_EQ(static_cast<double>(fixed32::get_min()), 0.00006103515625000000);
            break;
    }
    EXPECT_LT(fixed32::get_min(), fixed32::get_max());
    EXPECT_GT(fixed32::get_min(), fixed32(0));
    EXPECT_GT(fixed32::get_min(), fixed32::get_lowest());
}

TEST(fixed32, Lowest)
{
    const auto lowest_internal_val = std::numeric_limits<int32_t>::min() + 2;
    const auto lowest_fixed32 = *reinterpret_cast<const fixed32*>(&lowest_internal_val);

    EXPECT_EQ(fixed32::get_lowest(), fixed32::get_lowest());
    EXPECT_EQ(fixed32::get_lowest(), lowest_fixed32);
    //EXPECT_EQ(static_cast<long double>(fixed32::get_lowest()), -131072.00000000000000000000L);
    //std::cout << std::setprecision(22) << static_cast<long double>(fixed32::get_lowest()) << std::endl;
    switch (fixed32::fraction_bits)
    {
        case  9:
            EXPECT_NEAR(static_cast<double>(fixed32::get_lowest()), -4.1943e+06, 4.0);
            break;
        case 14:
            EXPECT_EQ(static_cast<double>(fixed32::get_lowest()), -131071.9998779296875);
            break;
    }
    EXPECT_LT(fixed32::get_lowest(), fixed32(0));
    EXPECT_LT(fixed32::get_lowest(), fixed32(-((1 << (31u - fixed32::fraction_bits)) - 1), 0u));
    EXPECT_LT(fixed32::get_lowest(), fixed32(-((1 << (31u - fixed32::fraction_bits)) - 1), (1u << fixed32::fraction_bits) - 1u));
    EXPECT_EQ(fixed32::get_lowest(), -fixed32::get_max());
}

TYPED_TEST(fixed_test, SubtractingFromLowestGetsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_lowest() - type::get_min(), type::get_negative_infinity());
    EXPECT_EQ(type::get_lowest() - 1, type::get_negative_infinity());
}

TYPED_TEST(fixed_test, AddingToMaxGetsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_max() + type::get_min(), type::get_positive_infinity());
    EXPECT_EQ(type::get_max() + 1, type::get_positive_infinity());
}

TYPED_TEST(fixed_test, MinusInfinityEqualsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(-type::get_positive_infinity(), type::get_negative_infinity());
}

TYPED_TEST(fixed_test, InfinityEqualsMinusNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity(), -type::get_negative_infinity());
}

TYPED_TEST(fixed_test, InifnityTimesPositiveIsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() * 1, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * 2, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * 0.5, type::get_positive_infinity());
}

TYPED_TEST(fixed_test, InifnityDividedByPositiveIsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() / 1, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / 2, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / 0.5, type::get_positive_infinity());
}

TYPED_TEST(fixed_test, InifnityTimesNegativeIsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() * -1, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * -2, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * -0.5, -type::get_positive_infinity());
}

TYPED_TEST(fixed_test, InifnityDividedByNegativeIsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() / -1, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / -2, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / -0.5, -type::get_positive_infinity());
}

TYPED_TEST(fixed_test, InfinityMinusNegativeInfinityIsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() - -type::get_positive_infinity(), type::get_positive_infinity());
}

TYPED_TEST(fixed_test, NegativeInfinityMinusInfinityIsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(-type::get_positive_infinity() - type::get_positive_infinity(), -type::get_positive_infinity());
}

TYPED_TEST(fixed_test, Comparators)
{
    using type = typename TestFixture::type;
    EXPECT_FALSE(type::get_nan() > 0.0f);
    EXPECT_FALSE(type::get_nan() < 0.0f);
    EXPECT_FALSE(type::get_nan() == 0.0f);
    EXPECT_TRUE(type::get_nan() != 0.0f);
    EXPECT_FALSE(type::get_nan() == type::get_nan());
}

TYPED_TEST(fixed_test, AdditionAssignment)
{
    using type = typename TestFixture::type;
    type foo;
    foo = 0;
    foo += type::get_negative_infinity();
    EXPECT_EQ(foo, -std::numeric_limits<type>::infinity());
    foo = std::numeric_limits<type>::lowest();
    foo += -1;
    EXPECT_EQ(foo, type::get_negative_infinity());
}

TYPED_TEST(fixed_test, SubtractionAssignment)
{
    using type = typename TestFixture::type;
    type foo;
    foo = 0;
    foo -= 0;
    EXPECT_EQ(foo, type{0});
    foo = 0;
    foo -= 1;
    EXPECT_EQ(foo, type{-1});
    foo = std::numeric_limits<type>::max();
    foo -= type{-2};
    EXPECT_EQ(foo, type::get_positive_infinity());
}

TYPED_TEST(fixed_test, MultiplicationAssignment)
{
    using type = typename TestFixture::type;
    type foo;
    foo = type::get_nan();
    foo *= type{0};
    EXPECT_TRUE(foo.isnan());
    foo = 0;
    foo *= type::get_nan();
    EXPECT_TRUE(foo.isnan());
    foo = std::numeric_limits<type>::min();
    foo *= std::numeric_limits<type>::min();
    EXPECT_EQ(foo, type(0));
    foo = std::numeric_limits<type>::lowest();
    foo *= 2;
    EXPECT_EQ(foo, type::get_negative_infinity());
}

TYPED_TEST(fixed_test, DivisionAssignment)
{
    using type = typename TestFixture::type;
    type foo;
    foo = type::get_nan();
    foo /= type{1};
    EXPECT_TRUE(foo.isnan());
    foo = 0;
    foo /= type::get_nan();
    EXPECT_TRUE(foo.isnan());
    foo = 1;
    foo /= type::get_positive_infinity();
    EXPECT_EQ(foo, type(0));
    foo = std::numeric_limits<type>::max();
    ASSERT_EQ(foo, std::numeric_limits<type>::max());
    foo /= type(0.5f);
    EXPECT_EQ(foo, type::get_positive_infinity());
    foo = std::numeric_limits<type>::lowest();
    ASSERT_TRUE(foo.isfinite());
    foo /= type(0.5);
    EXPECT_EQ(foo, type::get_negative_infinity());
}

TYPED_TEST(fixed_test, GetSign)
{
    using type = typename TestFixture::type;
    type foo;
    foo = 0;
    EXPECT_GT(foo.getsign(), 0);
    foo = type(-32.412);
    EXPECT_LT(foo.getsign(), 0);
}

TEST(fixed, StreamOut_fixed32)
{
    std::ostringstream os;
    os << fixed32(2.2f);
    EXPECT_STREQ(os.str().c_str(), "2.19922");
}

#ifdef REALNUMB_INT128
TEST(fixed, StreamOut_fixed64)
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
