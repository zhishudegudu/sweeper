#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "executor.h"

void controller_turn_right(Executor *executor);
void controller_turn_left(Executor *executor);
void controller_turn_round(Executor *executor);
void controller_forward_mile(Executor *executor, int distance);
void controller_backward_mile(Executor *executor, int distance);
void controller_get_position(const Executor *executor,
                             int *x,
                             int *y,
                             Heading *heading);

#endif
