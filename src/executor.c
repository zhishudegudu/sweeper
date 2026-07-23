#include "executor.h"

static int executor_check_danger(const Executor *executor)
{
    int i;

    for (i = 0; i < executor->danger_count; ++i) {
        if (executor->x == executor->danger_points[i].x &&
            executor->y == executor->danger_points[i].y) {
            executor->alert_handler(IN_DANGEROUS,
                                    executor->x,
                                    executor->y);
            return 1;
        }
    }
    return 0;
}

static void executor_check_clean(Executor *executor)
{
    int i;

    for (i = 0; i < executor->clean_count; ++i) {
        CleanPoint *point = &executor->clean_points[i];

        if (!point->cleaned &&
            executor->x == point->x &&
            executor->y == point->y) {
            executor->clean_handler(executor->x, executor->y);
            point->cleaned = 1;
            return;
        }
    }
}

static void executor_after_step(Executor *executor)
{
    if (executor_check_danger(executor)) {
        return;
    }
    executor_check_clean(executor);
}

static void executor_move_one_step_forward(Executor *executor)
{
    switch (executor->heading) {
        case NORTH:
            ++executor->y;
            break;
        case EAST:
            ++executor->x;
            break;
        case SOUTH:
            --executor->y;
            break;
        case WEST:
            --executor->x;
            break;
    }
}

static void executor_move_one_step_backward(Executor *executor)
{
    switch (executor->heading) {
        case NORTH:
            --executor->y;
            break;
        case EAST:
            --executor->x;
            break;
        case SOUTH:
            ++executor->y;
            break;
        case WEST:
            ++executor->x;
            break;
    }
}

void executor_init(Executor *executor, int x, int y, Heading heading)
{
    executor->x = x;
    executor->y = y;
    executor->heading = heading;
    executor->danger_count = 0;
    executor->clean_count = 0;
    executor->alert_handler = alert;
    executor->clean_handler = clean;
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
    int step;

    for (step = 0; step < distance; ++step) {
        executor_move_one_step_forward(executor);
        executor_after_step(executor);
    }
}

void executor_backward_mile(Executor *executor, int distance)
{
    int step;

    for (step = 0; step < distance; ++step) {
        executor_move_one_step_backward(executor);
        executor_after_step(executor);
    }
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

int executor_add_clean_point(Executor *executor, int x, int y)
{
    int i;

    if (executor->x == x && executor->y == y) {
        return 0;
    }
    if (executor->clean_count >= MAX_CLEAN_POINTS) {
        return 0;
    }
    for (i = 0; i < executor->clean_count; ++i) {
        if (executor->clean_points[i].x == x &&
            executor->clean_points[i].y == y) {
            return 0;
        }
    }

    executor->clean_points[executor->clean_count].x = x;
    executor->clean_points[executor->clean_count].y = y;
    executor->clean_points[executor->clean_count].cleaned = 0;
    ++executor->clean_count;
    return 1;
}

void executor_set_alert_handler(Executor *executor, AlertHandler handler)
{
    executor->alert_handler = handler;
}

void executor_set_clean_handler(Executor *executor, CleanHandler handler)
{
    executor->clean_handler = handler;
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
