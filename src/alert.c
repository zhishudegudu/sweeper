#include "alert.h"

#include <stdio.h>

void alert(AlertType type, int x, int y)
{
    printf("alert: type=%d, x=%d, y=%d\n", (int)type, x, y);
}
