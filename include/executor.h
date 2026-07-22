#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "alert.h"

typedef enum {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
} Heading;

typedef void (*AlertHandler)(AlertType type, int x, int y);

#define MAX_DANGER_POINTS 16

typedef struct {
    int x;
    int y;
} DangerPoint;

typedef struct {
    int x;
    int y;
    Heading heading;
    DangerPoint danger_points[MAX_DANGER_POINTS];
    int danger_count;
    AlertHandler alert_handler;
} Executor;

void executor_init(Executor *executor, int x, int y, Heading heading);
void executor_turn_right(Executor *executor);
void executor_turn_left(Executor *executor);
void executor_turn_round(Executor *executor);
void executor_forward_mile(Executor *executor, int distance);
void executor_backward_mile(Executor *executor, int distance);
int executor_add_danger_point(Executor *executor, int x, int y);
void executor_set_alert_handler(Executor *executor, AlertHandler handler);
void executor_get_position(const Executor *executor,
                           int *x,
                           int *y,
                           Heading *heading);

#endif
