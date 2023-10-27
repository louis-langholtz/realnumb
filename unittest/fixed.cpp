#include <gtest/gtest.h>

#include <iostream>
#include <limits> // for std::numeric_limits

#include <realnumb/fixed.hpp>

using namespace realnumb;

template <typename T>
class fixed_: public testing::Test {
public:
    using type = T;
};

using fixed_types = ::testing::Types<
    ::realnumb::fixed32
#ifdef REALNUMB_INT128
    , ::realnumb::fixed64
#endif
>;
TYPED_TEST_SUITE(fixed_, fixed_types);

TYPED_TEST(fixed_, to_value_from_unsigned)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::to_value(0u),  0 * type::scale_factor);
    EXPECT_EQ(type::to_value(1u),  1 * type::scale_factor);
    EXPECT_EQ(type::to_value(2u),  2 * type::scale_factor);
}

TYPED_TEST(fixed_, to_value_from_signed)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::to_value( 0),  0 * type::scale_factor);
    EXPECT_EQ(type::to_value( 1),  1 * type::scale_factor);
    EXPECT_EQ(type::to_value( 2),  2 * type::scale_factor);
    EXPECT_EQ(type::to_value(-1), -1 * type::scale_factor);
    EXPECT_EQ(type::to_value(-2), -2 * type::scale_factor);
}

TYPED_TEST(fixed_, to_value_from_floating)
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

TYPED_TEST(fixed_, int_construction_and_compare)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type( 0), type( 0));
    EXPECT_LT(type( 0), type( 1));
    EXPECT_GT(type( 0), type(-1));
    EXPECT_EQ(type(-10), type(-10));
    EXPECT_LT(type(-10), type( -9));
    EXPECT_GT(type(-10), type(-11));
}

TYPED_TEST(fixed_, IntCast)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(static_cast<int>(type( 0)),  0);
    EXPECT_EQ(static_cast<int>(type(-1)), -1);
    EXPECT_EQ(static_cast<int>(type(-2)), -2);
    EXPECT_EQ(static_cast<int>(type(+1)), +1);
    EXPECT_EQ(static_cast<int>(type(+2)), +2);
}

TYPED_TEST(fixed_, FloatCast)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(static_cast<float>(type( 0)),  0.0f);
    EXPECT_EQ(static_cast<float>(type(-1)), -1.0f);
    EXPECT_EQ(static_cast<float>(type(+1)), +1.0f);
}

TYPED_TEST(fixed_, DoubleCast)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(static_cast<double>(type( 0)),  0.0);
    EXPECT_EQ(static_cast<double>(type(-1)), -1.0);
    EXPECT_EQ(static_cast<double>(type(+1)), +1.0);
}

TYPED_TEST(fixed_, FloatConstruction)
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
    EXPECT_GT(fixed32::get_min(), fixed32(0));
    EXPECT_LT(fixed32::get_min(), fixed32(1));
    EXPECT_EQ(fixed32::get_min(), fixed32(0, 1u));
    EXPECT_NEAR(static_cast<double>(fixed32::get_min()), 0.001953125, 0.00001);
#ifdef REALNUMB_INT128
    EXPECT_GT(fixed64::get_min(), fixed64(0));
    EXPECT_LT(fixed64::get_min(), fixed64(1));
    EXPECT_EQ(fixed64::get_min(), fixed64(0, 1u));
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

TEST(fixed, get_lowest)
{
    EXPECT_EQ(static_cast<double>(fixed32::get_lowest()), -4194303.99609375);
#ifdef REALNUMB_INT128
    EXPECT_EQ(static_cast<double>(fixed64::get_lowest()), -549755813888);
#endif
}

TYPED_TEST(fixed_, Equals)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(type(12) == type(12.0f));
}

TYPED_TEST(fixed_, NotEquals)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(type(-302) != type(12.0f));
    EXPECT_FALSE(type(-302) != type(-302));
}

TYPED_TEST(fixed_, less)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(type(-302) < type(12.0f));
    EXPECT_TRUE(type(40) < type(44));
    EXPECT_FALSE(type(76) < type(31));
    EXPECT_TRUE(type(0.001) < type(0.002));
    EXPECT_TRUE(type(0.000) < type(0.01));
}

TYPED_TEST(fixed_, greater)
{
    using type = typename TestFixture::type;
    EXPECT_FALSE(type(-302) > type(12.0f));
    EXPECT_FALSE(type(40) > type(44));
    EXPECT_TRUE(type(76) > type(31));
}

TYPED_TEST(fixed_, Addition)
{
    using type = typename TestFixture::type;
    for (auto val = 0; val < 100; ++val)
    {
        type a{val};
        type b{val};
        EXPECT_EQ(a + b, type(val * 2));
    }
}

TYPED_TEST(fixed_, InfinityPlusValidIsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() + 0, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() + 1, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() + 100, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() + -1, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() + -100, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() + type::get_positive_infinity(), type::get_positive_infinity());
}

TYPED_TEST(fixed_, EqualSubtraction)
{
    using type = typename TestFixture::type;
    for (auto val = 0; val < 100; ++val)
    {
        type a{val};
        type b{val};
        EXPECT_EQ(a - b, type(0));
    }
}

TYPED_TEST(fixed_, OppositeSubtraction)
{
    using type = typename TestFixture::type;
    for (auto val = 0; val < 100; ++val)
    {
        type a{-val};
        type b{val};
        EXPECT_EQ(a - b, type(val * -2));
    }
}

TYPED_TEST(fixed_, Multiplication)
{
    using type = typename TestFixture::type;
    for (auto val = 0; val < 100; ++val)
    {
        type a{val};
        EXPECT_EQ(a * a, type(val * val));
    }
    EXPECT_EQ(type(9) * type(3), type(27));
    EXPECT_EQ(type(-5) * type(-4), type(20));
    EXPECT_EQ(type(181) * type(181), type(32761));
    EXPECT_EQ(type(0.5) * type(0.5), type(0.25));
    EXPECT_NEAR(double(type(-0.05) * type(0.05)), double(type(-0.0025)), 0.0001);
}

TYPED_TEST(fixed_, Division)
{
    using type = typename TestFixture::type;
    for (auto val = 1; val < 100; ++val)
    {
        type a{val};
        EXPECT_EQ(a / a, type(1));
    }
    EXPECT_EQ(type(9) / type(3), type(3));
    EXPECT_EQ(type(81) / type(9), type(9));
    EXPECT_EQ(type(+10) / type(2), type(+5));
    EXPECT_EQ(type(-10) / type(2), type(-5));
    EXPECT_EQ(type(1) / type(2), type(0.5));
    {
        const auto n = 7.0;
        const auto d = 3.0;
        const auto result = type(n) / type(d);
        const auto expected = type(n / d);
        const auto expected_hi = expected + type::get_min();
        const auto expected_lo = expected - type::get_min();
        EXPECT_LE(result, expected_hi);
        EXPECT_GE(result, expected_lo);
    }
    EXPECT_EQ(type(7) / type(2), type(7.0/2.0));
    EXPECT_EQ(type(0, 63u) / type(2), type(0, 32u));

    // Confirm int divided by fixed32 gets promoted to fixed32 divided by fixed32
    EXPECT_EQ(1 / type(2), type(0.5));
    EXPECT_EQ(2 / type(2), type(1));
    EXPECT_EQ(3 / type(2), type(1.5));
}

TEST(fixed, Max_fixed32)
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

TEST(fixed, Min_fixed32)
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

TEST(fixed, Lowest_fixed32)
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

TYPED_TEST(fixed_, SubtractingFromLowestGetsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_lowest() - type::get_min(), type::get_negative_infinity());
    EXPECT_EQ(type::get_lowest() - 1, type::get_negative_infinity());
}

TYPED_TEST(fixed_, AddingToMaxGetsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_max() + type::get_min(), type::get_positive_infinity());
    EXPECT_EQ(type::get_max() + 1, type::get_positive_infinity());
}

TYPED_TEST(fixed_, MinusInfinityEqualsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(-type::get_positive_infinity(), type::get_negative_infinity());
}

TYPED_TEST(fixed_, InfinityEqualsMinusNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity(), -type::get_negative_infinity());
}

TYPED_TEST(fixed_, InifnityTimesPositiveIsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() * 1, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * 2, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * 0.5, type::get_positive_infinity());
}

TYPED_TEST(fixed_, InifnityDividedByPositiveIsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() / 1, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / 2, type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / 0.5, type::get_positive_infinity());
}

TYPED_TEST(fixed_, InifnityTimesNegativeIsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() * -1, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * -2, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() * -0.5, -type::get_positive_infinity());
}

TYPED_TEST(fixed_, InifnityDividedByNegativeIsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() / -1, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / -2, -type::get_positive_infinity());
    EXPECT_EQ(type::get_positive_infinity() / -0.5, -type::get_positive_infinity());
}

TYPED_TEST(fixed_, InfinityMinusNegativeInfinityIsInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(type::get_positive_infinity() - -type::get_positive_infinity(), type::get_positive_infinity());
}

TYPED_TEST(fixed_, NegativeInfinityMinusInfinityIsNegativeInfinity)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(-type::get_positive_infinity() - type::get_positive_infinity(), -type::get_positive_infinity());
}

TYPED_TEST(fixed_, Comparators)
{
    using type = typename TestFixture::type;
    EXPECT_FALSE(type::get_nan() > 0.0f);
    EXPECT_FALSE(type::get_nan() < 0.0f);
    EXPECT_FALSE(type::get_nan() == 0.0f);
    EXPECT_TRUE(type::get_nan() != 0.0f);
    EXPECT_FALSE(type::get_nan() == type::get_nan());
}

TYPED_TEST(fixed_, AdditionAssignment)
{
    using type = typename TestFixture::type;
    type foo;
    foo = 0;
    foo += type::get_negative_infinity();
    EXPECT_EQ(foo, type::get_negative_infinity());
    foo = type::get_lowest();
    foo += -1;
    EXPECT_EQ(foo, type::get_negative_infinity());
}

TYPED_TEST(fixed_, SubtractionAssignment)
{
    using type = typename TestFixture::type;
    type foo;
    foo = 0;
    foo -= 0;
    EXPECT_EQ(foo, type{0});
    foo = 0;
    foo -= 1;
    EXPECT_EQ(foo, type{-1});
    foo = type::get_max();
    foo -= type{-2};
    EXPECT_EQ(foo, type::get_positive_infinity());
}

TYPED_TEST(fixed_, MultiplicationAssignment)
{
    using type = typename TestFixture::type;
    type foo;
    foo = type::get_nan();
    foo *= type{0};
    EXPECT_TRUE(foo.isnan());
    foo = 0;
    foo *= type::get_nan();
    EXPECT_TRUE(foo.isnan());
    foo = type::get_min();
    foo *= type::get_min();
    EXPECT_EQ(foo, type(0));
    foo = type::get_lowest();
    foo *= 2;
    EXPECT_EQ(foo, type::get_negative_infinity());
}

TYPED_TEST(fixed_, DivisionAssignment)
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
    foo = type::get_max();
    ASSERT_EQ(foo, type::get_max());
    foo /= type(0.5f);
    EXPECT_EQ(foo, type::get_positive_infinity());
    foo = type::get_lowest();
    ASSERT_TRUE(foo.isfinite());
    foo /= type(0.5);
    EXPECT_EQ(foo, type::get_negative_infinity());
}

TYPED_TEST(fixed_, GetSign)
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
