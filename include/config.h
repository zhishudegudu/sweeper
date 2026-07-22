#ifndef CONFIG_H
#define CONFIG_H

#include "executor.h"

typedef struct {
    int x;
    int y;
    Heading heading;
    DangerPoint danger_points[MAX_DANGER_POINTS];
    int danger_count;
} Config;

void config_default(Config *config);
int config_add_danger_point(Config *config, int x, int y);
int config_init_executor(const Config *config, Executor *executor);

#endif
