#include "config.h"
#include "controller.h"

#include <stdio.h>

int main(void)
{
    Config config;
    Executor executor;
    int x;
    int y;
    Heading heading;

    config_default(&config);
    config_init_executor(&config, &executor);

    controller_turn_right(&executor);
    controller_get_position(&executor, &x, &y, &heading);

    printf("x=%d, y=%d, heading=%d\n", x, y, heading);
    return 0;
}
