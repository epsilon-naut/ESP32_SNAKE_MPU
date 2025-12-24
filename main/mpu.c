#include <stdio.h>
#include <stdlib.h>
#include <esp_log.h>
#include <math.h>
#include <time.h>
#include "mpu.h"

void delay(int ms) {
    clock_t start = clock();
    clock_t end = clock();
    while((double)(end - start)/CLOCKS_PER_SEC*1000 < ms) {
        end = clock();
    }
}
                        
i2c_master_bus_config_t i2c_mst_config = { 
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_NUM_0,
    .scl_io_num = GPIO_NUM_22,
    .sda_io_num = GPIO_NUM_21,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true
};

i2c_device_config_t i2c_mpu_config = {
    .dev_addr_length = I2C_ADDR_BIT_7,
    .device_address = I2C_ADDRESS,
    .scl_speed_hz = 100000
};

i2c_master_bus_handle_t bus_handle;

i2c_master_dev_handle_t mpu_handle;

int sign(uint16_t *num) {
    if(*num >= pow(2, 15)) {
        return -1 * (pow(2, 16)-1 - *num);
    }
    return *num;
}

void read_register(uint8_t *data, uint8_t Reg, size_t size) {
    uint8_t reg[1] = {Reg};
    i2c_master_transmit_receive(mpu_handle, reg, 1, data, size, -1);
}

void setup() {
    i2c_new_master_bus(&i2c_mst_config, &bus_handle);
    i2c_master_bus_add_device(bus_handle, &i2c_mpu_config, &mpu_handle);
}

void wakey_wakey() {
    uint8_t trans[2] = {PWR_MGMT, 0x01};
    uint8_t *data = (uint8_t *)malloc(1);
    i2c_master_transmit(mpu_handle, trans, 2, -1);
    read_register(data, PWR_MGMT, 1);
    ESP_LOGI("MPU6050", "%u", *data);
}

void check_connection_vulgar() {
    if(i2c_master_probe(bus_handle, I2C_ADDRESS, -1) == ESP_OK) {
        ESP_LOGI("MPU6050", "What the fuck");
    }
    else if(i2c_master_probe(bus_handle, I2C_ADDRESS, -1) == ESP_ERR_TIMEOUT) {
        ESP_LOGI("MPU6050", "huh");
    }
    else {
        ESP_LOGI("MPU6050", "fuck you");
    }
}

void sanity_check() {
    uint8_t *data = (uint8_t *)malloc(1);
    read_register(data, WHO_AM_I, 1);
    ESP_LOGI("MPU6050", "%u", *data);
}

void check_change_sample(uint8_t sample) {
    uint8_t *data = (uint8_t *)malloc(1);
    uint8_t trans[2] = {SMPL_DIV, sample};
    i2c_master_transmit(mpu_handle, trans, 2, -1);
    read_register(data, SMPL_DIV, 1);
    ESP_LOGI("MPU6050", "%u", *data);
}

void config_gyro(int range) {
    uint8_t conf;
    if (range == 250) {
        conf = 0x00;
    }    
    else if (range == 500) {
        conf = 0x08;
    }
    else if (range == 1000) {
        conf = 0x10;
    }
    else {
        conf = 0x18;
    }
    uint8_t trans[2] = {CNF_GYRO, conf};
    i2c_master_transmit(mpu_handle, trans, 2, -1);
}

void config_accel(int range) {
    uint8_t conf;
    if (range == 2) {
        conf = 0x00;
    }    
    else if (range == 4) {
        conf = 0x08;
    }
    else if (range == 8) {
        conf = 0x10;
    }
    else {
        conf = 0x18;
    }
    uint8_t trans[2] = {CNF_ACCEL, conf};
    i2c_master_transmit(mpu_handle, trans, 2, -1);
}

void fifo_enable(uint8_t gyro_r, uint8_t gyro_p, uint8_t gyro_y, uint8_t accel) {
    uint8_t conf = 0x40 * gyro_r + 0x20 * gyro_p + 0x10 * gyro_y + 0x08 * accel;
    uint8_t trans[2] = {FIFO_EN, conf};
    i2c_master_transmit(mpu_handle, trans, 2, -1);
}

void self_test_accel(uint8_t *data) {
    config_accel(8);
    read_register(data, ST_ACCEL, 1);
}

double read_accel_x(int range, uint16_t *data, uint8_t *data1, uint8_t *data2) {
    config_accel(range);
    int signed_data;
    read_register(data1, INT_STATUS, 1);
    if(*data1 % 2 == 1) {
        read_register(data1, RD_ACCEL_X_1, 1);
        read_register(data2, RD_ACCEL_X_2, 1);
        *data = *data1 * 256 + *data2;
        signed_data = sign(data);
        return range * signed_data / pow(2, 15) * 9.81;
    }
    else {
        ESP_LOGI("MPU6050", "Not ready.");
    }
    return range * *data / pow(2, 15) * 9.81;
}

double read_accel_y(int range, uint16_t *data, uint8_t *data1, uint8_t *data2) {
    config_accel(range);
    int signed_data;
    read_register(data1, INT_STATUS, 1);
    if(*data1 % 2 == 1) {
        read_register(data1, RD_ACCEL_Y_1, 1);
        read_register(data2, RD_ACCEL_Y_2, 1);
        *data = *data1 * 256 + *data2;
        signed_data = sign(data);
        return range * signed_data / pow(2, 15) * 9.81;
    }
    else {
        ESP_LOGI("MPU6050", "Not ready.");
    }
    return range * *data / pow(2, 15) * 9.81;
}

double read_accel_z(int range, uint16_t *data, uint8_t *data1, uint8_t *data2) {
    config_accel(range);
    int signed_data;
    read_register(data1, INT_STATUS, 1);
    if(*data1 % 2 == 1) {
        read_register(data1, RD_ACCEL_Z_1, 1);
        read_register(data2, RD_ACCEL_Z_2, 1);
        *data = *data1 * 256 + *data2;
        signed_data = sign(data);
        return range * signed_data / pow(2, 15) * 9.81;
    }
    else {
        ESP_LOGI("MPU6050", "Not ready.");
    }
    return range * *data / pow(2, 15) * 9.81;
}

uint16_t read_seed(int range, uint16_t *data, uint8_t *data1, uint8_t *data2) {
    config_accel(range);
    read_register(data1, INT_STATUS, 1);
    if(*data1 % 2 == 1) {
        read_register(data1, RD_ACCEL_Z_1, 1);
        read_register(data2, RD_ACCEL_Z_2, 1);
        *data = *data1 * 256 + *data2;
        return *data;
    }
    else {
        ESP_LOGI("MPU6050", "Not ready.");
        return 0;
    }
}

double read_gyro_r(int range, uint16_t *data, uint8_t *data1, uint8_t *data2) {
    config_accel(range);
    int signed_data;
    read_register(data1, INT_STATUS, 1);
    if(*data1 % 2 == 1) {
        read_register(data1, RD_GYRO_R_1, 1);
        read_register(data2, RD_GYRO_R_2, 1);
        *data = *data1 * 256 + *data2;
        signed_data = sign(data);
        return range * signed_data / pow(2, 15);
    }
    else {
        ESP_LOGI("MPU6050", "Not ready.");
    }
    return range * *data / pow(2, 15);
}

double read_gyro_p(int range, uint16_t *data, uint8_t *data1, uint8_t *data2) {
    config_accel(range);
    int signed_data;
    read_register(data1, INT_STATUS, 1);
    if(*data1 % 2 == 1) {
        read_register(data1, RD_GYRO_P_1, 1);
        read_register(data2, RD_GYRO_P_2, 1);
        *data = *data1 * 256 + *data2;
        signed_data = sign(data);
        return range * signed_data / pow(2, 15);
    }
    else {
        ESP_LOGI("MPU6050", "Not ready.");
    }
    return range * *data / pow(2, 15);
}

double read_gyro_y(int range, uint16_t *data, uint8_t *data1, uint8_t *data2) {
    config_accel(range);
    int signed_data;
    read_register(data1, INT_STATUS, 1);
    if(*data1 % 2 == 1) {
        read_register(data1, RD_GYRO_Y_1, 1);
        read_register(data2, RD_GYRO_Y_2, 1);
        *data = *data1 * 256 + *data2;
        signed_data = sign(data);
        return range * signed_data / pow(2, 15);
    }
    else {
        ESP_LOGI("MPU6050", "Not ready.");
    }
    return range * *data / pow(2, 15);
}