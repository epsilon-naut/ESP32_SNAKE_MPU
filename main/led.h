#ifndef LED_H
#define LED_H

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <time.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <math.h>
#include <stdlib.h>
#include "dot.h"

#define CS 5
#define RST 32
#define A0 14
#define SDA 23
#define CLK 18

#define CPOL 0
#define CPHA 0

//currently BGR for some reason idk

typedef struct wrapper {
    spi_device_handle_t *handle;
    uint8_t *buf;
    int byte_length;
} wrapper;

void init(spi_device_handle_t *handle);
void send(wrapper *w);
void send_cmd(wrapper *w);
void send_data(wrapper *w);
void start(wrapper *w);
void colmod(wrapper *w);
void cset(wrapper *w, uint8_t start, uint8_t end);
void rset(wrapper *w, uint8_t start, uint8_t end);
void whitescreen(wrapper *w);
void blackscreen(wrapper *w);
void draw_dot(wrapper *w, dot *d, uint8_t color);
void draw_dotline(wrapper *w, uint8_t x_start, uint8_t y_start, uint8_t color, uint8_t xy);


#endif