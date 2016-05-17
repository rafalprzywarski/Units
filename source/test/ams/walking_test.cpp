#include <sim/walking_unit.hpp>
#include <gmock/gmock.h>

using namespace testing;

namespace ams
{

MATCHER_P2(IsCloseTo, p, d, std::string(negation ? "isn't" : "is") + " within " + PrintToString(d) + " to " + PrintToString(p))
{
    return length(arg - p) <= d;
}

struct walking_test : testing::Test
{
    void update_until_stops(walking_unit& u)
    {
        update_unit(u);

        for (int limit = 1000; u.logical.state != walking_unit_state::standing && limit > 0; --limit)
            update_unit(u);

        if (u.logical.state != walking_unit_state::standing)
            FAIL() << "never stopped";
    }
};

TEST_F(walking_test, walking_forward)
{
    const vec2f position{0, 0};
    walking_unit unit{{1, 0.25}, position};

    const vec2f target_position{0, 20};
    set_target_position(unit, target_position);

    update_until_stops(unit);

    ASSERT_THAT(get_position(unit), IsCloseTo(target_position, 0.01));
}

TEST_F(walking_test, turning_right)
{
    const vec2f position{0, 0};
    walking_unit unit{{1, 0.25}, position};
    ASSERT_THAT(get_direction(unit), IsCloseTo(vec2f{0, 1}, 0.001));

    const vec2f target_position{2, 0};
    set_target_position(unit, target_position);

    update_until_stops(unit);

    ASSERT_THAT(get_position(unit), IsCloseTo(target_position, 0.01));
    ASSERT_THAT(get_direction(unit), IsCloseTo(vec2f{1, 0}, 0.001));
}

}
