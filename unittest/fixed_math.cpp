#include <gtest/gtest.h>

#include <realnumb/fixed_math.hpp>

using namespace realnumb;

constexpr auto pi = double{3.14159265358979323846264338327950288};

template <typename T>
class fixed_math_: public testing::Test {
public:
    using type = T;
};

using fixed_types = ::testing::Types<
    ::realnumb::fixed32
#ifdef REALNUMB_INT128
    , ::realnumb::fixed64
#endif
>;
TYPED_TEST_SUITE(fixed_math_, fixed_types);

TYPED_TEST(fixed_math_, isfinite)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(isfinite(type(0)));
    EXPECT_FALSE(isfinite( type::get_positive_infinity()));
    EXPECT_FALSE(isfinite(-type::get_positive_infinity()));
    EXPECT_FALSE(isfinite(type::get_nan()));
}

TYPED_TEST(fixed_math_, isnan)
{
    using type = typename TestFixture::type;

    EXPECT_FALSE(isnan(type( 0)));
    EXPECT_FALSE(isnan(type( 1)));
    EXPECT_FALSE(isnan(type(-1)));
    EXPECT_FALSE(isnan(type(10.0f)));
    EXPECT_FALSE(isnan(type(-10.0f)));
    EXPECT_FALSE(isnan( type::get_positive_infinity()));
    EXPECT_FALSE(isnan(-type::get_positive_infinity()));
    EXPECT_FALSE(isnan( type::get_negative_infinity()));
    EXPECT_FALSE(isnan(type::get_max()));
    EXPECT_FALSE(isnan(type::get_min()));
    EXPECT_FALSE(isnan(type::get_lowest()));

    EXPECT_TRUE(isnan(type::get_nan()));
    EXPECT_TRUE(isnan(type(std::numeric_limits<float>::quiet_NaN())));
    EXPECT_TRUE(isnan(type(std::numeric_limits<float>::signaling_NaN())));
    EXPECT_TRUE(isnan(type(std::numeric_limits<double>::quiet_NaN())));
    EXPECT_TRUE(isnan(type(std::numeric_limits<double>::signaling_NaN())));
    EXPECT_TRUE(isnan(type(std::numeric_limits<long double>::quiet_NaN())));
    EXPECT_TRUE(isnan(type(std::numeric_limits<long double>::signaling_NaN())));
    EXPECT_TRUE(isnan(type::get_positive_infinity() / type::get_positive_infinity()));
    EXPECT_TRUE(isnan(type::get_positive_infinity() - type::get_positive_infinity()));
    EXPECT_TRUE(isnan(-type::get_positive_infinity() - -type::get_positive_infinity()));
    EXPECT_TRUE(isnan(-type::get_positive_infinity() + type::get_positive_infinity()));
}

TYPED_TEST(fixed_math_, log_zero_notfinite)
{
    using type = typename TestFixture::type;
    ASSERT_FALSE(isfinite(static_cast<double>(log(0.0))));
    EXPECT_FALSE(isfinite(static_cast<double>(log(type(0)))));
}

TYPED_TEST(fixed_math_, log_minus_one_isnan)
{
    using type = typename TestFixture::type;
    ASSERT_TRUE(isnan(static_cast<double>(log(-1.0))));
    EXPECT_TRUE(isnan(static_cast<double>(log(type(-1)))));
}

TEST(fixed_math, log_fixed32)
{
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

    EXPECT_EQ(static_cast<double>(log(fixed32::get_positive_infinity())),
              log(std::numeric_limits<double>::infinity()));
}

TEST(fixed_math, exp_fixed32)
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

TEST(fixed_math, intpow_fixed32)
{
    ASSERT_NEAR(static_cast<double>(pow(0.0, 0)), 1.0, 0.0);
    ASSERT_NEAR(static_cast<double>(pow(0.0, +1)), 0.0, 0.0);
    ASSERT_FALSE(isfinite(static_cast<double>(pow(0.0, -1))));

    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), 0)), 1.0, 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), +1)), 0.0, 0.0);
    EXPECT_FALSE(isfinite(static_cast<double>(pow(fixed32(0), -1))));

    EXPECT_EQ(static_cast<double>(pow(fixed32::get_negative_infinity(), -1)),
              pow(-std::numeric_limits<double>::infinity(), -1));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_negative_infinity(), +1)),
              pow(-std::numeric_limits<double>::infinity(), +1));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_negative_infinity(), +2)),
              pow(-std::numeric_limits<double>::infinity(), +2));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_positive_infinity(), +2)),
              pow(std::numeric_limits<double>::infinity(), +2));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_positive_infinity(), -2)),
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

TEST(fixed_math, regpow_fixed32)
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

TEST(fixed_math, sqrt_fixed32)
{
    const auto tolerance = static_cast<double>(fixed32::get_min());

    EXPECT_TRUE(isnan(sqrt(fixed32{-1})));
    EXPECT_TRUE(isnan(sqrt(fixed32::get_nan())));
    EXPECT_EQ(sqrt(fixed32{0}), fixed32{0});
    EXPECT_EQ(sqrt(fixed32::get_positive_infinity()), fixed32::get_positive_infinity());
    EXPECT_EQ(sqrt(fixed32::get_min()), fixed32(0.046875));
    EXPECT_EQ((sqrt(fixed32::get_min()) * sqrt(fixed32::get_min())), fixed32::get_min());
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
        EXPECT_LE(abs((sqrt(fixedv) * sqrt(fixedv)) - fixedv), fixed32::get_min());
        EXPECT_NEAR(static_cast<double>(sqrt(fixedv)), sqrt(static_cast<double>(fixedv)), tolerance * 5);
    }
    const auto maxV = static_cast<double>(std::numeric_limits<fixed32>::max());
    for (auto v = 1.0; v < maxV; v *= 1.1)
    {
        const auto fixedv = fixed32{v};
        EXPECT_NEAR(static_cast<double>(sqrt(fixedv)), sqrt(v), tolerance);
        //EXPECT_LE(abs(Square(sqrt(fixedv)) - fixedv), fixed32::get_min() * 5);
    }
}

TYPED_TEST(fixed_math_, hypot)
{
    using type = typename TestFixture::type;
    constexpr auto tolerance = (sizeof(type) > 4u)? 0.001: 0.01;
    for (auto i = 0; i < 100; ++i)
    {
        for (auto j = 0; i < 100; ++i)
        {
            EXPECT_NEAR(static_cast<double>(hypot(type(i), type(j))),
                        hypot(double(i), double(j)), tolerance);
        }
    }
}

TEST(fixed_math, sin_fixed32)
{
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
TEST(fixed_math, sin_fixed64)
{
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

TEST(fixed_math, cos_fixed32)
{
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
TEST(fixed_math, cos_fixed64)
{
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

TYPED_TEST(fixed_math_, atan)
{
    using type = typename TestFixture::type;
    EXPECT_NEAR(static_cast<double>(atan(type::get_positive_infinity())),
                atan(std::numeric_limits<double>::infinity()), 0.001);
    EXPECT_NEAR(static_cast<double>(atan(type::get_negative_infinity())),
                atan(-std::numeric_limits<double>::infinity()), 0.001);
}

TEST(fixed_math, atan2_specials_fixed32)
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

TEST(fixed_math, atan2_angles_fixed32)
{
    for (auto angleInDegs = -90; angleInDegs < +90; ++angleInDegs)
    {
        const auto angle = angleInDegs * pi / 180;
        const auto s = sin(angle);
        const auto c = cos(angle);
        EXPECT_NEAR(static_cast<double>(atan2(fixed32(s), fixed32(c))), angle, 0.05);
    }
}

TYPED_TEST(fixed_math_, InfinityDividedByInfinityIsNaN)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(isnan(type::get_positive_infinity() / type::get_positive_infinity()));
}

TYPED_TEST(fixed_math_, InfinityTimesZeroIsNaN)
{
    using type = typename TestFixture::type;
   EXPECT_TRUE(isnan(type::get_positive_infinity() * 0));
}

TEST(fixed_math, BiggerValsIdenticallyInaccurate_fixed32)
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

TEST(fixed_math, nextafter)
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
