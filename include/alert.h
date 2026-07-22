#ifndef ALERT_H
#define ALERT_H

typedef enum {
    IN_DANGEROUS = 3
} AlertType;

void alert(AlertType type, int x, int y);

#endif
