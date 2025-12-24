#ifndef MOVE_H
#define MOVE_H
#include "led.h"
#include "mpu.h"
#include "snake.h"

void start_apples(dot *app, snake *sn);

int add_snake(wrapper *w, snake *sn, double speed, uint8_t xy);

int move_snake(wrapper *w, snake *sn, double speed, uint8_t xy, dot *app, int *score);

#endif