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

    // Config 未指定位置时，应使用题目要求的 (0, 0, 北)。
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

    // 右转只把北变成东，不能改变坐标。
    executor_init(&executor, 10, 20, NORTH);
    controller_turn_right(&executor);
    controller_get_position(&executor, &x, &y, &heading);
    EXPECT_EQ(10, x);
    EXPECT_EQ(20, y);
    EXPECT_EQ(EAST, heading);

    // 再左转应由东回到北，坐标仍保持不变。
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

    // Config 自定义的坐标与朝向应完整下发给 Executor。
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
