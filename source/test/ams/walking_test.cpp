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
        do update_unit(u);
        while (u.is_walking);
    }
};

TEST_F(walking_test, walking_forward)
{
    const vec2f position{0, 0};
    walking_unit unit{1, 0.25, position};

    const vec2f target_position{20, 0};
    set_target_position(unit, target_position);

    update_until_stops(unit);

    ASSERT_THAT(get_position(unit), IsCloseTo(target_position, 0.01));
}

}
