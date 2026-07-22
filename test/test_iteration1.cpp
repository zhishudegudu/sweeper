extern "C" {
#include "config.h"
#include "controller.h"
}

#include <gtest/gtest.h>

TEST(Iteration1Test, UsesDefaultPositionAndHeading)
{
    Config config;
    Executor executor;
    int x;
    int y;
    Heading heading;

    config_default(&config);
    ASSERT_EQ(1, config_init_executor(&config, &executor));
    controller_get_position(&executor, &x, &y, &heading);

    EXPECT_EQ(0, x);
    EXPECT_EQ(0, y);
    EXPECT_EQ(NORTH, heading);
}

TEST(Iteration1Test, TurnsRightAndLeftWithoutMoving)
{
    Executor executor;
    int x;
    int y;
    Heading heading;

    executor_init(&executor, 10, 20, NORTH);
    controller_turn_right(&executor);
    controller_get_position(&executor, &x, &y, &heading);
    EXPECT_EQ(10, x);
    EXPECT_EQ(20, y);
    EXPECT_EQ(EAST, heading);

    controller_turn_left(&executor);
    controller_get_position(&executor, &x, &y, &heading);
    EXPECT_EQ(10, x);
    EXPECT_EQ(20, y);
    EXPECT_EQ(NORTH, heading);
}

TEST(Iteration1Test, ConfigInitializesSpecifiedPosition)
{
    Config config;
    Executor executor;
    int x;
    int y;
    Heading heading;

    config_default(&config);
    config.x = 3;
    config.y = -2;
    config.heading = SOUTH;
    ASSERT_EQ(1, config_init_executor(&config, &executor));
    controller_get_position(&executor, &x, &y, &heading);

    EXPECT_EQ(3, x);
    EXPECT_EQ(-2, y);
    EXPECT_EQ(SOUTH, heading);
}
