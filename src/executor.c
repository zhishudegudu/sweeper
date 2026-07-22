#include "executor.h"

static void executor_check_danger(const Executor *executor)
{
    int i;

    for (i = 0; i < executor->danger_count; ++i) {
        if (executor->x == executor->danger_points[i].x &&
            executor->y == executor->danger_points[i].y) {
            executor->alert_handler(IN_DANGEROUS,
                                    executor->x,
                                    executor->y);
            return;
        }
    }
}

void executor_init(Executor *executor, int x, int y, Heading heading)
{
    executor->x = x;
    executor->y = y;
    executor->heading = heading;
    executor->danger_count = 0;
    executor->alert_handler = alert;
}

void executor_turn_right(Executor *executor)
{
    executor->heading = (Heading)((executor->heading + 1) % 4);
    executor_check_danger(executor);
}

void executor_turn_left(Executor *executor)
{
    executor->heading = (Heading)((executor->heading + 3) % 4);
    executor_check_danger(executor);
}

void executor_turn_round(Executor *executor)
{
    executor->heading = (Heading)((executor->heading + 2) % 4);
    executor_check_danger(executor);
}

void executor_forward_mile(Executor *executor, int distance)
{
    switch (executor->heading) {
        case NORTH:
            executor->y += distance;
            break;

        case EAST:
            executor->x += distance;
            break;

        case SOUTH:
            executor->y -= distance;
            break;

        case WEST:
            executor->x -= distance;
            break;
    }
    executor_check_danger(executor);
}

void executor_backward_mile(Executor *executor, int distance)
{
    switch (executor->heading) {
        case NORTH:
            executor->y -= distance;
            break;

        case EAST:
            executor->x -= distance;
            break;

        case SOUTH:
            executor->y += distance;
            break;

        case WEST:
            executor->x += distance;
            break;
    }
    executor_check_danger(executor);
}

int executor_add_danger_point(Executor *executor, int x, int y)
{
    int i;

    if (executor->x == x && executor->y == y) {
        return 0;
    }
    if (executor->danger_count >= MAX_DANGER_POINTS) {
        return 0;
    }
    for (i = 0; i < executor->danger_count; ++i) {
        if (executor->danger_points[i].x == x &&
            executor->danger_points[i].y == y) {
            return 0;
        }
    }

    executor->danger_points[executor->danger_count].x = x;
    executor->danger_points[executor->danger_count].y = y;
    ++executor->danger_count;
    return 1;
}

void executor_set_alert_handler(Executor *executor, AlertHandler handler)
{
    executor->alert_handler = handler;
}

void executor_get_position(const Executor *executor,
                           int *x,
                           int *y,
                           Heading *heading)
{
    *x = executor->x;
    *y = executor->y;
    *heading = executor->heading;
}
