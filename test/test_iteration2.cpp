extern "C" {
#include "config.h"
#include "controller.h"
#include "executor.h"
}

#include <gtest/gtest.h>

struct MoveCase {
    Heading heading;
    int forward_x;
    int forward_y;
    int backward_x;
    int backward_y;
};

class MoveTest : public testing::TestWithParam<MoveCase> {
};

TEST_P(MoveTest, ForwardAndBackward)
{
    const MoveCase test = GetParam();
    Executor executor;
    int x;
    int y;
    Heading heading;

    executor_init(&executor, 10, 20, test.heading);
    controller_forward_mile(&executor, 3);
    controller_get_position(&executor, &x, &y, &heading);

    EXPECT_EQ(test.forward_x, x);
    EXPECT_EQ(test.forward_y, y);
    EXPECT_EQ(test.heading, heading);

    executor_init(&executor, 10, 20, test.heading);
    controller_backward_mile(&executor, 3);
    controller_get_position(&executor, &x, &y, &heading);

    EXPECT_EQ(test.backward_x, x);
    EXPECT_EQ(test.backward_y, y);
    EXPECT_EQ(test.heading, heading);
}

INSTANTIATE_TEST_SUITE_P(
    FourHeadings,
    MoveTest,
    testing::Values(
        MoveCase{NORTH, 10, 23, 10, 17},
        MoveCase{EAST, 13, 20, 7, 20},
        MoveCase{SOUTH, 10, 17, 10, 23},
        MoveCase{WEST, 7, 20, 13, 20}));

TEST(TurnRoundTest, TurnsToOppositeHeadingWithoutMoving)
{
    struct TurnCase {
        Heading before;
        Heading after;
    };
    const TurnCase cases[] = {
        {NORTH, SOUTH},
        {EAST, WEST},
        {SOUTH, NORTH},
        {WEST, EAST}
    };

    for (const TurnCase &test : cases) {
        Executor executor;
        int x;
        int y;
        Heading heading;

        executor_init(&executor, 10, 20, test.before);
        controller_turn_round(&executor);
        controller_get_position(&executor, &x, &y, &heading);

        EXPECT_EQ(10, x);
        EXPECT_EQ(20, y);
        EXPECT_EQ(test.after, heading);
    }
}
