extern "C" {
#include "config.h"
#include "controller.h"
}

#include <gtest/gtest.h>

namespace {

struct RecordedPoint {
    int x;
    int y;
};

int alert_count;
int clean_count;
RecordedPoint cleaned_points[MAX_CLEAN_POINTS];

void RecordAlert(AlertType, int, int)
{
    // 这里只关心告警调用次数，坐标参数由迭代三测试覆盖。
    ++alert_count;
}

void RecordClean(int x, int y)
{
    // 按调用顺序记录每一次清扫坐标，验证逐格移动的触发顺序。
    cleaned_points[clean_count].x = x;
    cleaned_points[clean_count].y = y;
    ++clean_count;
}

class Iteration4Test : public testing::Test {
protected:
    void SetUp() override
    {
        // 每个用例执行前重置所有 Alert/Clean 测试记录。
        alert_count = 0;
        clean_count = 0;
        for (int i = 0; i < MAX_CLEAN_POINTS; ++i) {
            cleaned_points[i] = {0, 0};
        }
    }

    void Init(Config *config, Executor *executor)
    {
        // 使用 Config 初始化，并将真实组件替换为测试记录函数。
        ASSERT_EQ(1, config_init_executor(config, executor));
        executor_set_alert_handler(executor, RecordAlert);
        executor_set_clean_handler(executor, RecordClean);
    }
};

}  // namespace

TEST_F(Iteration4Test, RejectsInitialDuplicateAndExcessCleanPoints)
{
    Config config;
    config_default(&config);

    // 初始位置不能清扫。
    EXPECT_EQ(0, config_add_clean_point(&config, 0, 0));
    // 依次填满固定容量，所有合法点都应添加成功。
    for (int i = 1; i <= MAX_CLEAN_POINTS; ++i) {
        EXPECT_EQ(1, config_add_clean_point(&config, i, 0));
    }
    // 已有坐标属于重复点，新坐标则因为容量已满而失败。
    EXPECT_EQ(0, config_add_clean_point(&config, 1, 0));
    EXPECT_EQ(0, config_add_clean_point(&config, 100, 0));
    EXPECT_EQ(MAX_CLEAN_POINTS, config.clean_count);
}

TEST_F(Iteration4Test, CleansMultiplePointsAlongOneMovement)
{
    Config config;
    Executor executor;

    config_default(&config);
    ASSERT_EQ(1, config_add_clean_point(&config, 0, 1));
    ASSERT_EQ(1, config_add_clean_point(&config, 0, 2));
    Init(&config, &executor);

    // 从 (0,0) 向北前进3格，沿途经过两个清扫点。
    controller_forward_mile(&executor, 3);

    // 必须按路径顺序在 (0,1)、(0,2) 各清扫一次。
    ASSERT_EQ(2, clean_count);
    EXPECT_EQ(0, cleaned_points[0].x);
    EXPECT_EQ(1, cleaned_points[0].y);
    EXPECT_EQ(0, cleaned_points[1].x);
    EXPECT_EQ(2, cleaned_points[1].y);
}

TEST_F(Iteration4Test, CleansEachPointOnlyOnceWhenPassingAgain)
{
    Config config;
    Executor executor;

    config_default(&config);
    ASSERT_EQ(1, config_add_clean_point(&config, 0, 2));
    Init(&config, &executor);

    // 三次经过 (0,2)，但该点完成后不能重复清扫。
    controller_forward_mile(&executor, 3);
    controller_backward_mile(&executor, 3);
    controller_forward_mile(&executor, 3);

    EXPECT_EQ(1, clean_count);
}

TEST_F(Iteration4Test, CleansPointWhileMovingBackward)
{
    Config config;
    Executor executor;

    config_default(&config);
    // 从 (0,3) 朝北后退到 (0,0)，沿途经过 (0,1)。
    config.y = 3;
    ASSERT_EQ(1, config_add_clean_point(&config, 0, 1));
    Init(&config, &executor);

    controller_backward_mile(&executor, 3);

    ASSERT_EQ(1, clean_count);
    EXPECT_EQ(0, cleaned_points[0].x);
    EXPECT_EQ(1, cleaned_points[0].y);
}

TEST_F(Iteration4Test, DoesNotCleanWithoutConfiguredPoints)
{
    Config config;
    Executor executor;

    config_default(&config);
    Init(&config, &executor);

    // 未配置清扫点时，往返移动都不能调用 Clean。
    controller_forward_mile(&executor, 3);
    controller_backward_mile(&executor, 3);

    EXPECT_EQ(0, clean_count);
}

TEST_F(Iteration4Test, AlertsAndSkipsCleanWhenPointsOverlap)
{
    Config config;
    Executor executor;

    config_default(&config);
    // 同一坐标同时配置为危险点和清扫点，Alert 优先。
    ASSERT_EQ(1, config_add_danger_point(&config, 0, 2));
    ASSERT_EQ(1, config_add_clean_point(&config, 0, 2));
    Init(&config, &executor);

    // 第一次经过：告警一次，禁止清扫。
    controller_forward_mile(&executor, 3);
    EXPECT_EQ(1, alert_count);
    EXPECT_EQ(0, clean_count);

    // 回程和再次去程又经过两次：累计告警3次，始终不清扫。
    controller_backward_mile(&executor, 3);
    controller_forward_mile(&executor, 3);
    EXPECT_EQ(3, alert_count);
    EXPECT_EQ(0, clean_count);
}

TEST_F(Iteration4Test, ReinitializationResetsCleanedState)
{
    Config config;
    Executor executor;

    config_default(&config);
    ASSERT_EQ(1, config_add_clean_point(&config, 0, 1));
    Init(&config, &executor);

    // 第一次初始化周期内到达清扫点，清扫一次。
    controller_forward_mile(&executor, 1);
    ASSERT_EQ(1, clean_count);

    // 重新初始化会清除 cleaned 状态，再经过时应再次清扫。
    Init(&config, &executor);
    controller_forward_mile(&executor, 1);
    EXPECT_EQ(2, clean_count);
}
