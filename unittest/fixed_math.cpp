#include <gtest/gtest.h>

#include <cmath> // for std::isnan, etc.

#include <realnumb/fixed_math.hpp>

using namespace realnumb;

constexpr auto pi = double{3.14159265358979323846264338327950288};

namespace {

struct value_expect {
    double value;
    double tolerance; // actual time tolerance gives absolute
};

template <int N, class T>
auto next_after(T from, T to) -> decltype(nextafter(from, to))
{
    for (auto i = 0; i < N; ++i) {
        from = nextafter(from, to);
    }
    return from;
}

}

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

TYPED_TEST(fixed_math_, round_trip_pi)
{
    using type = typename TestFixture::type;
    // check within 1 ULP...
    EXPECT_GE(double(type(pi) + type::get_min()), pi);
    EXPECT_LE(double(type(pi) - type::get_min()), pi);
}

TYPED_TEST(fixed_math_, angular_normalize)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(detail::angular_normalize(+type( 0) * 1.0), +type( 0) * 1.0);
    EXPECT_EQ(detail::angular_normalize(+type(pi) * 1.0), +type(pi) * 1.0);
    EXPECT_EQ(detail::angular_normalize(+type(pi) * 1.5), -type(pi) * 0.5);
    EXPECT_EQ(detail::angular_normalize(+type(pi) * 2.0), +type( 0) * 1.0);
    EXPECT_EQ(detail::angular_normalize(+type(pi) * 3.0), +type(pi) * 1.0);
    EXPECT_EQ(detail::angular_normalize(-type(pi) * 1.0), -type(pi) * 1.0);
    EXPECT_EQ(detail::angular_normalize(-type(pi) * 1.5), +type(pi) * 0.5);
    EXPECT_EQ(detail::angular_normalize(-type(pi) * 2.0), -type( 0) * 1.0);
    EXPECT_EQ(detail::angular_normalize(-type(pi) * 3.0), -type(pi) * 1.0);
}

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

TYPED_TEST(fixed_math_, log_error_handling_same_as_double)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(isnan(log(type::get_nan())), std::isnan(std::log(std::numeric_limits<double>::quiet_NaN())));
    EXPECT_EQ(isnan(log(type(-42.0))), std::isnan(std::log(-42.0)));
    EXPECT_EQ(log(type(1.0)), std::log(1.0));
    EXPECT_EQ(log(type(0.0)), std::log(0.0));
    EXPECT_EQ(log(type::get_positive_infinity()), std::log(std::numeric_limits<double>::infinity()));
}

TYPED_TEST(fixed_math_, log_error_handling_as_specified)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(isnan(log(type::get_nan())));
    EXPECT_TRUE(isnan(log(type(-42.0))));
    EXPECT_EQ(log(type(1)), type(0));
    EXPECT_EQ(log(type(0)), type::get_negative_infinity());
    EXPECT_EQ(log(type::get_positive_infinity()), type::get_positive_infinity());
}

TEST(fixed_math, log_fixed32)
{
    ASSERT_DOUBLE_EQ(std::log(0.1), -2.3025850929940455);
    EXPECT_NEAR(static_cast<double>(log(fixed32(0.1))), std::log(0.1),
                double(fixed32::get_min() * 1));

    ASSERT_DOUBLE_EQ(std::log(0.5), -0.69314718055994529);
    EXPECT_NEAR(static_cast<double>(log(fixed32(0.5))), std::log(0.5),
                double(fixed32::get_min() * 1));

    ASSERT_DOUBLE_EQ(std::log(1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(log(fixed32(1.0))), 0.0,
                double(fixed32::get_min() * 0));

    ASSERT_DOUBLE_EQ(std::log(1.5), 0.40546510810816438);
    EXPECT_NEAR(static_cast<double>(log(fixed32(1.5))), std::log(1.5),
                double(fixed32::get_min() * 1));

    ASSERT_DOUBLE_EQ(std::log(2.0),  0.69314718055994529);
    EXPECT_NEAR(static_cast<double>(log(fixed32(2.0))), std::log(2.0),
                double(fixed32::get_min() * 9));

    ASSERT_DOUBLE_EQ(std::log(2.1), 0.74193734472937733);
    EXPECT_NEAR(static_cast<double>(log(fixed32(2.1))), std::log(2.1),
                double(fixed32::get_min() * 1));

    ASSERT_DOUBLE_EQ(std::log(2.75), 1.0116009116784799);
    EXPECT_NEAR(static_cast<double>(log(fixed32(2.75))), std::log(2.75),
                double(fixed32::get_min() * 2));

    ASSERT_DOUBLE_EQ(std::log(4.5), 1.5040773967762742);
    EXPECT_NEAR(static_cast<double>(log(fixed32(4.5))), std::log(4.5),
                double(fixed32::get_min() * 2));

    ASSERT_DOUBLE_EQ(std::log(31.21), 3.440738556282688);
    EXPECT_NEAR(static_cast<double>(log(fixed32(31.21))), std::log(31.21),
                double(fixed32::get_min() * 10));

    ASSERT_DOUBLE_EQ(std::log(491.721), 6.1979114824747752);
    EXPECT_NEAR(static_cast<double>(log(fixed32(491.721))), std::log(491.721),
                1.517);
}

TEST(fixed_math, exp_fixed32)
{
    const auto terms = std::initializer_list<value_expect>{
        {0.0, 0.001},
        {0.4, 0.005},
        {0.9, 0.008},
        {1.0, 0.003},
        {1.34, 0.005},
        {2.5, 0.003},
        {3.15, 0.004},
        {4.8, 0.003},
        {7.1, 0.002},
        {8.9, 0.002},
        {10.1, 0.001},
        {12.5, 0.003},
        {-1.0, 0.004},
        {-2.0, 0.02},
        {-4.0, 0.041}
    };
    for (const auto& term: terms) {
        const auto expected = std::exp(term.value);
        const auto abs_err = std::abs(expected) * term.tolerance;
        std::ostringstream os;
        os << "for value of ";
        os << term.value;
        os << ", with absolute error tolerance ";
        os << abs_err;
        SCOPED_TRACE(os.str());
        EXPECT_NEAR(double(exp(fixed32(term.value))), expected, abs_err);
    }
}

TEST(fixed_math, intpow_fixed32)
{
    ASSERT_NEAR(static_cast<double>(std::pow(0.0, 0)), 1.0, 0.0);
    ASSERT_NEAR(static_cast<double>(std::pow(0.0, +1)), 0.0, 0.0);
    ASSERT_FALSE(std::isfinite(static_cast<double>(std::pow(0.0, -1))));

    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), 0)), 1.0, 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), +1)), 0.0, 0.0);
    EXPECT_FALSE(std::isfinite(static_cast<double>(pow(fixed32(0), -1))));

    EXPECT_EQ(static_cast<double>(pow(fixed32::get_negative_infinity(), -1)),
              std::pow(-std::numeric_limits<double>::infinity(), -1));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_negative_infinity(), +1)),
              std::pow(-std::numeric_limits<double>::infinity(), +1));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_negative_infinity(), +2)),
              std::pow(-std::numeric_limits<double>::infinity(), +2));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_positive_infinity(), +2)),
              std::pow(std::numeric_limits<double>::infinity(), +2));
    EXPECT_EQ(static_cast<double>(pow(fixed32::get_positive_infinity(), -2)),
              std::pow(std::numeric_limits<double>::infinity(), -2));

    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), 1)), std::pow(0.0, 1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), 0)), std::pow(0.0, 0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), 0)), std::pow(1.0, 0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), +44)), std::pow(1.0, +44), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), -44)), std::pow(1.0, -44), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), +1)), std::pow(+1.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), +1)), std::pow(-1.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), -1)), std::pow(+1.0, -1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), -1)), std::pow(-1.0, -1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+2), +1)), std::pow(+2.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), +1)), std::pow(+3.0, +1), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), +2)), std::pow(+3.0, +2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), +3)), std::pow(+3.0, +3), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), +2)), std::pow(+9.0, +2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), -1)), std::pow(+9.0, -1), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), -1)), std::pow(+3.0, -1), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), -2)), std::pow(+9.0, -2), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), -2)), std::pow(+3.0, -2), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+10), -2)), std::pow(+10.0, -2), 0.01);
}

TEST(fixed_math, regpow_fixed32)
{
    ASSERT_NEAR(std::pow(0.0, 0.0), 1.0, 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(0), fixed32(0))), std::pow(0.0, 0.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), fixed32(0))), std::pow(1.0, 0.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), fixed32(+44.2))), std::pow(1.0, +44.2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(1), fixed32(-44.2))), std::pow(1.0, -44.2), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), fixed32(+1))), std::pow(+1.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), fixed32(+1))), std::pow(-1.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+1), fixed32(-1))), std::pow(+1.0, -1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-1), fixed32(-1))), std::pow(-1.0, -1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+2), fixed32(+1))), std::pow(+2.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(+1))), std::pow(+3.0, +1.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(+2))), std::pow(+3.0, +2.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(+3))), std::pow(+3.0, +3.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), fixed32(+2))), std::pow(+9.0, +2.0), 0.0);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), fixed32(-1))), std::pow(+9.0, -1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(-1))), std::pow(+3.0, -1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+9), fixed32(-2))), std::pow(+9.0, -2.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3), fixed32(-2))), std::pow(+3.0, -2.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+10), fixed32(-2))), std::pow(+10.0, -2.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(-10), fixed32(-2))), std::pow(-10.0, -2.0), 0.01);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+4), fixed32(+2.3))), std::pow(+4.0, +2.3), 0.97);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+4), fixed32(-2.3))), std::pow(+4.0, -2.3), 0.1);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(+2.3))), std::pow(+3.1, +2.3), 0.75);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(-2.3))), std::pow(+3.1, -2.3), 0.1);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(+4.3))), std::pow(+3.1, +4.3), 12.3);
    EXPECT_NEAR(static_cast<double>(pow(fixed32(+3.1), fixed32(-4.3))), std::pow(+3.1, -4.3), 0.3);
    EXPECT_EQ(std::isnan(static_cast<double>(pow(fixed32(-4), fixed32(+2.3)))),
              std::isnan(std::pow(-4.0, +2.3)));
    EXPECT_EQ(std::isnan(static_cast<double>(pow(fixed32(-4), fixed32(-2.3)))),
              std::isnan(std::pow(-4.0, -2.3)));
}

TYPED_TEST(fixed_math_, sqrt_error_handling)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(isnan(sqrt(type::get_lowest())));
    EXPECT_TRUE(isnan(sqrt(type(-1))));
    EXPECT_EQ(sqrt(type(0)), type(0));
    EXPECT_EQ(sqrt(type::get_positive_infinity()), type::get_positive_infinity());
    EXPECT_TRUE(isnan(sqrt(type::get_nan())));
}

TYPED_TEST(fixed_math_, sqrt_basics)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(sqrt(type(1)), type(1));
    EXPECT_EQ((sqrt(type::get_min()) * sqrt(type::get_min())), type::get_min());
    EXPECT_NEAR(double(sqrt(type::get_min())), std::sqrt(double(type::get_min())),
                double(type::get_min() * 6));
}

TYPED_TEST(fixed_math_, sqrt_0_to_10000_within_1ulps)
{
    using type = typename TestFixture::type;
    for (auto i = 0; i < 10000; ++i) {
        std::ostringstream os;
        os << "for value of " << i;
        SCOPED_TRACE(os.str());
        EXPECT_NEAR(double(sqrt(type(i))), std::sqrt(double(i)), double(type::get_min()));
    }
}

TEST(fixed_math, sqrt_fixed32)
{
    const auto tolerance = static_cast<double>(fixed32::get_min());
    EXPECT_NEAR(static_cast<double>(sqrt(fixed32{0.25})), 0.5, 0.0);
    EXPECT_NEAR(static_cast<double>(sqrt(fixed32{0.0625})), 0.25, tolerance);
    for (auto v = 1.0; v > 0.0; v /= 1.1)
    {
        const auto fixedv = fixed32{v};
        if (fixedv == fixed32{0}) {
            break;
        }
        std::ostringstream os;
        os << "for double value of " << v;
        SCOPED_TRACE(os.str());
        EXPECT_LE(abs((sqrt(fixedv) * sqrt(fixedv)) - fixedv), fixed32::get_min());
        EXPECT_NEAR(double(sqrt(fixedv)), std::sqrt(double(fixedv)),
                    double(fixed32::get_min() * 6));
    }
    const auto maxV = static_cast<double>(std::numeric_limits<fixed32>::max());
    for (auto v = 1.0; v < maxV; v *= 1.1)
    {
        const auto fixedv = fixed32{v};
        EXPECT_NEAR(static_cast<double>(sqrt(fixedv)), std::sqrt(v), tolerance);
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
                        std::hypot(double(i), double(j)), tolerance);
        }
    }
}

TYPED_TEST(fixed_math_, sin_error_handling)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(sin(type(0)), type(0));
    EXPECT_TRUE(isnan(sin(type::get_positive_infinity())));
    EXPECT_TRUE(isnan(sin(type::get_negative_infinity())));
}

TYPED_TEST(fixed_math_, cos_error_handling)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(cos(type(0)), type(1));
    EXPECT_TRUE(isnan(cos(type::get_positive_infinity())));
    EXPECT_TRUE(isnan(cos(type::get_negative_infinity())));
}

TYPED_TEST(fixed_math_, sin_basic_shape)
{
    using type = typename TestFixture::type;
    auto last = type(0);
    const auto trace_msg = [](int i, double angle_in_radians) -> std::string {
        std::ostringstream os;
        os << "for angle of " << i << " degrees, or " << angle_in_radians << " radians";
        return os.str();
    };
    for (auto i = 1; i <= 90; ++i) {
        const auto angle_in_radians = (double(i) * pi) / 180.0;
        SCOPED_TRACE(trace_msg(i, angle_in_radians));
        const auto result = sin(type(angle_in_radians));
        EXPECT_GE(result, last);
        last = result;
    }
    for (auto i = 91; i <= 180; ++i) {
        const auto angle_in_radians = (double(i) * pi) / 180.0;
        SCOPED_TRACE(trace_msg(i, angle_in_radians));
        const auto result = sin(type(angle_in_radians));
        EXPECT_LE(result, last);
        last = result;
    }
    for (auto i = 181; i <= 270; ++i) {
        const auto angle_in_radians = (double(i) * pi) / 180.0;
        SCOPED_TRACE(trace_msg(i, angle_in_radians));
        const auto result = sin(type(angle_in_radians));
        EXPECT_LE(result, last);
        last = result;
    }
    for (auto i = 271; i <= 360; ++i) {
        const auto angle_in_radians = (double(i) * pi) / 180.0;
        SCOPED_TRACE(trace_msg(i, angle_in_radians));
        const auto result = sin(type(angle_in_radians));
        EXPECT_GE(result, last);
        last = result;
    }
}

TYPED_TEST(fixed_math_, sin_ulps)
{
    using type = typename TestFixture::type;
    constexpr auto ulps = 2;
    for (auto i = -180; i <= 180; ++i) {
        const auto angle_in_radians = (type(i) * pi) / 180;
        const auto expected = type(std::sin(double(angle_in_radians)));
        const auto expected_hi = next_after<ulps>(expected, type(+2.0));
        const auto expected_lo = next_after<ulps>(expected, type(-2.0));
        const auto result = sin(angle_in_radians);
        std::ostringstream os;
        os << "for angle of " << i << " degrees, or " << angle_in_radians << " radians";
        os << ", expected=" << expected;
        SCOPED_TRACE(os.str());
        EXPECT_LE(result, type(+1));
        EXPECT_GE(result, type(-1));
        const auto result_hi = next_after<ulps>(result, type(+2));
        const auto result_lo = next_after<ulps>(result, type(-2));
        EXPECT_GE(result_hi, expected_lo);
        EXPECT_LE(result_lo, expected_hi);
    }
}

TYPED_TEST(fixed_math_, sin)
{
    using type = typename TestFixture::type;
    for (auto i = 0; i < 90; ++i) {
        const auto angle_in_radians = (double(i) * pi) / 180.0;
        const auto expected = std::sin(angle_in_radians);
        const auto abs_err = expected * ((sizeof(type) > 4u)? 0.001: 0.4);
        std::ostringstream os;
        os << "for angle of " << i << " degrees, or " << angle_in_radians << " radians";
        SCOPED_TRACE(os.str());
        const auto result = sin(type(angle_in_radians));
        EXPECT_NEAR(double(result), expected, abs_err);
        EXPECT_LE(double(result), +1.0);
        EXPECT_GE(double(result), -1.0);
    }
    for (auto i = 90; i < 180; ++i) {
        const auto angle_in_radians = (double(i) * pi) / 180.0;
        const auto expected = std::sin(angle_in_radians);
        const auto abs_err = expected * ((sizeof(type) > 4u)? 0.03: 0.8);
        std::ostringstream os;
        os << "for angle of " << i << " degrees, or " << angle_in_radians << " radians";
        SCOPED_TRACE(os.str());
        const auto result = sin(type(angle_in_radians));
        EXPECT_NEAR(double(result), expected, abs_err);
        EXPECT_LE(double(result), +1.0);
        EXPECT_GE(double(result), -1.0);
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
                std::atan(std::numeric_limits<double>::infinity()), 0.001);
    EXPECT_NEAR(static_cast<double>(atan(type::get_negative_infinity())),
                std::atan(-std::numeric_limits<double>::infinity()), 0.001);
}

TEST(fixed_math, atan2_specials_fixed32)
{
    EXPECT_TRUE(std::isnan(static_cast<double>(atan2(fixed32(0), fixed32(0)))));
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(+1), fixed32(0))), std::atan2(+1.0, 0.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(-1), fixed32(0))), std::atan2(-1.0, 0.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(0), fixed32(+1))), std::atan2(0.0, +1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(0), fixed32(-1))), std::atan2(0.0, -1.0), 0.01);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(+1), fixed32(+1))), std::atan2(+1.0, +1.0), 0.05);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(+1), fixed32(-1))), std::atan2(+1.0, -1.0), 0.05);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(-1), fixed32(+1))), std::atan2(-1.0, +1.0), 0.05);
    EXPECT_NEAR(static_cast<double>(atan2(fixed32(-1), fixed32(-1))), std::atan2(-1.0, -1.0), 0.05);
}

TEST(fixed_math, atan2_angles_fixed32)
{
    for (auto angleInDegs = -90; angleInDegs < +90; ++angleInDegs)
    {
        const auto angle = angleInDegs * pi / 180;
        const auto s = std::sin(angle);
        const auto c = std::cos(angle);
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

TYPED_TEST(fixed_math_, nextafter_upward_like_adding_min)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(nextafter(type(0), type(1)), type::get_min());
    EXPECT_EQ(nextafter(type::get_min(), type(1)), type::get_min() + type::get_min());
    EXPECT_EQ(nextafter(type(-2), type(1)), type(-2) + type::get_min());
}

TYPED_TEST(fixed_math_, nextafter_downward_like_subtracting_min)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(nextafter(type(0), type(-1)), -type::get_min());
    EXPECT_EQ(nextafter(-type::get_min(), -type(1)), -type::get_min() - type::get_min());
    EXPECT_EQ(nextafter(type(+2), type(1)), type(2) - type::get_min());
}

TYPED_TEST(fixed_math_, nextafter_to_higher_is_higher)
{
    using type = typename TestFixture::type;
    auto v = type::get_lowest();
    auto last = v;
    constexpr auto shift = (type::total_bits >= 32u)
        ? (type::total_bits / 3)
        : (type::total_bits / 2);
    constexpr auto max = typename type::value_type(1) << shift;
    for (auto i = 0; i < max; ++i) {
        v = nextafter(v, type::get_max());
        EXPECT_GT(v, last);
        last = v;
    }
}

TYPED_TEST(fixed_math_, nextafter_to_lower_is_lower)
{
    using type = typename TestFixture::type;
    auto v = type::get_max();
    auto last = v;
    constexpr auto shift = (type::total_bits >= 32u)
        ? (type::total_bits / 3)
        : (type::total_bits / 2);
    constexpr auto max = typename type::value_type(1) << shift;
    for (auto i = 0; i < max; ++i) {
        v = nextafter(v, type::get_lowest());
        EXPECT_LT(v, last);
        last = v;
    }
}

TYPED_TEST(fixed_math_, fpclassify)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(fpclassify(type::get_nan()), FP_NAN);
    EXPECT_EQ(fpclassify(type::get_positive_infinity()), FP_INFINITE);
    EXPECT_EQ(fpclassify(type::get_negative_infinity()), FP_INFINITE);
    EXPECT_EQ(fpclassify(type()), FP_ZERO);
    EXPECT_EQ(fpclassify(type(1)), FP_NORMAL);
}
