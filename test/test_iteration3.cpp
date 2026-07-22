extern "C" {
#include "config.h"
#include "controller.h"
}

#include <gtest/gtest.h>

namespace {
int alert_count;
AlertType last_type;
int last_x;
int last_y;

void RecordAlert(AlertType type, int x, int y)
{
    ++alert_count;
    last_type = type;
    last_x = x;
    last_y = y;
}
}

TEST(Iteration3Test, RejectsInitialAndDuplicateDangerPoints)
{
    Config config;
    config_default(&config);

    EXPECT_EQ(0, config_add_danger_point(&config, 0, 0));
    EXPECT_EQ(1, config_add_danger_point(&config, 0, 3));
    EXPECT_EQ(0, config_add_danger_point(&config, 0, 3));
    EXPECT_EQ(1, config.danger_count);
}

TEST(Iteration3Test, AlertsAtMultipleDangerPoints)
{
    Config config;
    Executor executor;

    alert_count = 0;
    config_default(&config);
    ASSERT_EQ(1, config_add_danger_point(&config, 0, 3));
    ASSERT_EQ(1, config_add_danger_point(&config, 2, 3));
    ASSERT_EQ(1, config_init_executor(&config, &executor));
    executor_set_alert_handler(&executor, RecordAlert);

    controller_forward_mile(&executor, 3);
    EXPECT_EQ(1, alert_count);
    EXPECT_EQ(IN_DANGEROUS, last_type);
    EXPECT_EQ(0, last_x);
    EXPECT_EQ(3, last_y);

    controller_turn_right(&executor);
    EXPECT_EQ(2, alert_count);

    controller_forward_mile(&executor, 2);
    EXPECT_EQ(3, alert_count);
    EXPECT_EQ(2, last_x);
    EXPECT_EQ(3, last_y);

    controller_turn_round(&executor);
    EXPECT_EQ(4, alert_count);

    controller_forward_mile(&executor, 1);
    EXPECT_EQ(4, alert_count);
}
