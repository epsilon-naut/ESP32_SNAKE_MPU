#include "move.h"

void app_main(void)
{
    setup();
    check_connection_vulgar();
    wakey_wakey();
    ESP_LOGI("MPU6050","Setup finished!");
    sanity_check();
    check_change_sample(0x07);
    double x_accel;
    double y_accel;
    double z_accel;
    double r_gyro;
    double p_gyro;
    double y_gyro;
    uint16_t data[2];
    uint8_t data1;
    uint8_t data2;
    spi_device_handle_t handle;
    uint8_t buf[24];
    wrapper w = {.handle = &handle, .buf = buf, .byte_length = 1};
    uint8_t ins[2];
    snake *sn = (snake *)malloc(sizeof(snake));
    dot p;
    int score = 1;

    snakestart(sn, (dot){.x = 64, .y = 64});    
    dot *apple = (dot *)malloc(sizeof(dot));
    start_apples(apple, sn);
    start(&w);
    colmod(&w);
    blackscreen(&w);
    draw_dot(&w, &(sn->head->pos), 2);
    draw_dot(&w, apple, 2);

    x_accel = read_accel_x(2, data, &data1, &data2);
    y_accel = read_accel_y(2, data, &data1, &data2);
    z_accel = read_accel_z(2, data, &data1, &data2);
    
    ESP_LOGI("MPU6050", "%lf", x_accel);
    ESP_LOGI("MPU6050", "%lf", z_accel);
    ESP_LOGI("MPU6050", "%lf", y_accel);
    move_snake(&w, sn, x_accel, 0, apple, &score);
    move_snake(&w, sn, y_accel, 1, apple, &score);
    int i = 0;
    int uneaten = 1;
    int alive = 1;
    while(alive) {
        x_accel = read_accel_x(2, data, &data1, &data2);
        y_accel = read_accel_y(2, data, &data1, &data2);
        z_accel = read_accel_z(2, data, &data1, &data2);
        
        //ESP_LOGI("MPU6050", "%lf", x_accel);
        //ESP_LOGI("MPU6050", "%lf", z_accel);
        //ESP_LOGI("MPU6050", "%lf", y_accel);
        if(abs(x_accel) >= abs(y_accel)) {
            if(move_snake(&w, sn, x_accel, 0, apple, &score)) {
                whitescreen(&w);
                alive = 0;
            }
        }
        else {
            if(move_snake(&w, sn, y_accel, 1, apple, &score)) {
                whitescreen(&w);
                alive = 0;
            }
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    ESP_LOGI("Score", "%d", score);
    free(sn);
    free(apple);
}

