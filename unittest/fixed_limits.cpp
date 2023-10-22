#include <gtest/gtest.h>

#include <realnumb/fixed.hpp>
#include <realnumb/fixed_limits.hpp>

#include <iostream>

using namespace realnumb;

template <typename T>
class fixed_limits_: public testing::Test {
public:
    using type = T;
};

using fixed_types = ::testing::Types<
    ::realnumb::fixed32
#ifdef REALNUMB_INT128
    , ::realnumb::fixed64
#endif
>;
TYPED_TEST_SUITE(fixed_limits_, fixed_types);

TYPED_TEST(fixed_limits_, FloatConstruction)
{
    using type = typename TestFixture::type;
    EXPECT_EQ(std::numeric_limits<type>::infinity(), type::get_positive_infinity());
    EXPECT_EQ(-std::numeric_limits<type>::infinity(), -type::get_positive_infinity());
    EXPECT_EQ(-std::numeric_limits<type>::infinity(), type::get_negative_infinity());
    EXPECT_EQ(std::numeric_limits<type>::max(), type::get_max());
    EXPECT_EQ(std::numeric_limits<type>::min(), type::get_min());
    EXPECT_EQ(std::numeric_limits<type>::lowest(), type::get_lowest());
}

TYPED_TEST(fixed_limits_, Equals)
{
    using type = typename TestFixture::type;
    EXPECT_FALSE(std::numeric_limits<type>::quiet_NaN() == std::numeric_limits<type>::quiet_NaN());
}

TYPED_TEST(fixed_limits_, NotEquals)
{
    using type = typename TestFixture::type;
    EXPECT_TRUE(std::numeric_limits<type>::quiet_NaN() != std::numeric_limits<type>::quiet_NaN());
}

TYPED_TEST(fixed_limits_, AdditionAssignment)
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

TYPED_TEST(fixed_limits_, SubtractionAssignment)
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

TYPED_TEST(fixed_limits_, MultiplicationAssignment)
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

TYPED_TEST(fixed_limits_, DivisionAssignment)
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
