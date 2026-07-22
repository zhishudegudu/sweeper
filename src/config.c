#include "config.h"

void config_default(Config *config)
{
    config->x = 0;
    config->y = 0;
    config->heading = NORTH;
    config->danger_count = 0;
}

int config_add_danger_point(Config *config, int x, int y)
{
    int i;

    if (config->x == x && config->y == y) {
        return 0;
    }
    if (config->danger_count >= MAX_DANGER_POINTS) {
        return 0;
    }
    for (i = 0; i < config->danger_count; ++i) {
        if (config->danger_points[i].x == x &&
            config->danger_points[i].y == y) {
            return 0;
        }
    }

    config->danger_points[config->danger_count].x = x;
    config->danger_points[config->danger_count].y = y;
    ++config->danger_count;
    return 1;
}

int config_init_executor(const Config *config, Executor *executor)
{
    int i;

    executor_init(executor, config->x, config->y, config->heading);
    for (i = 0; i < config->danger_count; ++i) {
        if (!executor_add_danger_point(executor,
                                       config->danger_points[i].x,
                                       config->danger_points[i].y)) {
            return 0;
        }
    }
    return 1;
}
