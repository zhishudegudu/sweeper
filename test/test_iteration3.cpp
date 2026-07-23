extern "C" {
#include "config.h"
#include "controller.h"
}

#include "gtest/gtest.h"

namespace {
int alert_count;
AlertType last_type;
int last_x;
int last_y;

void RecordAlert(AlertType type, int x, int y)
{
    // 测试替身：不执行真实告警，只记录次数和最后一次参数。
    ++alert_count;
    last_type = type;
    last_x = x;
    last_y = y;
}

class Iteration3Test : public testing::Test {
protected:
    void SetUp() override
    {
        // 每个用例开始前清空共享记录，保证测试之间互不影响。
        alert_count = 0;
        last_type = IN_DANGEROUS;
        last_x = 0;
        last_y = 0;
    }
};
}

TEST_F(Iteration3Test, RejectsInitialAndDuplicateDangerPoints)
{
    Config config;
    config_default(&config);

    // 初始位置不能设为危险点；合法点可添加；重复点应拒绝。
    EXPECT_EQ(0, config_add_danger_point(&config, 0, 0));
    EXPECT_EQ(1, config_add_danger_point(&config, 0, 3));
    EXPECT_EQ(0, config_add_danger_point(&config, 0, 3));
    EXPECT_EQ(1, config.danger_count);
}

TEST_F(Iteration3Test, AlertsAtMultipleDangerPoints)
{
    Config config;
    Executor executor;

    // 行进路径依次经过 (0,3) 和 (2,3) 两个危险点。
    config_default(&config);
    ASSERT_EQ(1, config_add_danger_point(&config, 0, 3));
    ASSERT_EQ(1, config_add_danger_point(&config, 2, 3));
    ASSERT_EQ(1, config_init_executor(&config, &executor));
    executor_set_alert_handler(&executor, RecordAlert);

    // 首次到达 (0,3)，应报告第一次告警及正确参数。
    controller_forward_mile(&executor, 3);
    EXPECT_EQ(1, alert_count);
    EXPECT_EQ(IN_DANGEROUS, last_type);
    EXPECT_EQ(0, last_x);
    EXPECT_EQ(3, last_y);

    // 停留在危险点执行转向，按照需求应再次告警。
    controller_turn_right(&executor);
    EXPECT_EQ(2, alert_count);

    // 向东移动到第二个危险点 (2,3)，产生第三次告警。
    controller_forward_mile(&executor, 2);
    EXPECT_EQ(3, alert_count);
    EXPECT_EQ(2, last_x);
    EXPECT_EQ(3, last_y);

    // 在第二个危险点掉头，再次触发告警。
    controller_turn_round(&executor);
    EXPECT_EQ(4, alert_count);

    // 离开危险点后不应产生新告警。
    controller_forward_mile(&executor, 1);
    EXPECT_EQ(4, alert_count);
}
