#include "controller.h"

void controller_turn_right(Executor *executor)
{
    executor_turn_right(executor);
}

void controller_turn_left(Executor *executor)
{
    executor_turn_left(executor);
}

void controller_turn_round(Executor *executor)
{
    executor_turn_round(executor);
}

void controller_forward_mile(Executor *executor, int distance)
{
    executor_forward_mile(executor, distance);
}

void controller_backward_mile(Executor *executor, int distance)
{
    executor_backward_mile(executor, distance);
}


void controller_get_position(const Executor *executor,
                             int *x,
                             int *y,
                             Heading *heading)
{
    executor_get_position(executor, x, y, heading);
}


