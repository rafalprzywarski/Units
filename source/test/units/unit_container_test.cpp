#include <units/unit_container.hpp>
#include <gtest/gtest.h>
#include <boost/optional/optional_io.hpp>

namespace units
{

namespace
{

struct simple_unit
{
    unsigned id{};
    vec2f position{};
    float radius{};
};

}

struct unit_container_test : testing::Test
{
    using container = unit_container<simple_unit>;
};

TEST_F(unit_container_test, should_check_for_collisions_with_a_single_unit)
{
    container c;
    c.put(simple_unit{1, {10, 10}, 5});

    EXPECT_TRUE(c.collides_with({10, 10}, 2));
    EXPECT_TRUE(c.collides_with({5, 10}, 2));

    EXPECT_TRUE(c.collides_with({4, 10}, 1.01f));
    EXPECT_TRUE(c.collides_with({3, 10}, 2.01f));
    EXPECT_FALSE(c.collides_with({4, 10}, 0.99f));
    EXPECT_FALSE(c.collides_with({3, 10}, 1.99f));

    EXPECT_TRUE(c.collides_with({10, 4}, 1.01f));
    EXPECT_FALSE(c.collides_with({10, 4}, 0.99f));

    EXPECT_TRUE(c.collides_with({10, 16}, 1.01f));
    EXPECT_FALSE(c.collides_with({10, 16}, 0.99f));

    EXPECT_TRUE(c.collides_with({14.2426, 14.2426}, 1.01f));
    EXPECT_FALSE(c.collides_with({14.2426, 14.2426}, 0.99f));
}

}
