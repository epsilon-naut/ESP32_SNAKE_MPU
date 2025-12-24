#ifndef MPU_H
#define MPU_H

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>

#define ST_ACCEL 0x0F
#define SMPL_DIV 0x19
#define CNF_GYRO 0x1B
#define CNF_ACCEL 0x1C
#define FIFO_EN 0x23
#define INT_STATUS 0x3A
#define RD_ACCEL_X_1 0x3B
#define RD_ACCEL_X_2 0x3C
#define RD_ACCEL_Y_1 0x3D
#define RD_ACCEL_Y_2 0x3E
#define RD_ACCEL_Z_1 0x3F
#define RD_ACCEL_Z_2 0x40
#define RD_GYRO_P_1 0x43
#define RD_GYRO_P_2 0x44
#define RD_GYRO_Y_1 0x45
#define RD_GYRO_Y_2 0x46
#define RD_GYRO_R_1 0x47
#define RD_GYRO_R_2 0x48
#define I2C_ADDRESS 0x68
#define PWR_MGMT 0x6B
#define WHO_AM_I 0x75

extern i2c_master_bus_config_t i2c_mst_config;

extern i2c_master_bus_handle_t bus_handle;

extern i2c_device_config_t i2c_mpu_config;

extern i2c_master_dev_handle_t mpu_handle;

void delay(int ms);

int sign(uint16_t *num);

void read_register(uint8_t *data, uint8_t Reg, size_t size);

void setup();

void wakey_wakey();

void check_connection_vulgar();

void sanity_check();

void check_change_sample(uint8_t sample);

void config_gyro(int range);

void config_accel(int range);

void fifo_enable(uint8_t gyro_r, uint8_t gyro_p, uint8_t gyro_y, uint8_t accel);

void self_test_accel(uint8_t *data);

double read_accel_x(int range, uint16_t *data, uint8_t *data1, uint8_t *data2);
double read_accel_y(int range, uint16_t *data, uint8_t *data1, uint8_t *data2);
double read_accel_z(int range, uint16_t *data, uint8_t *data1, uint8_t *data2);

uint16_t read_seed(int range, uint16_t *data, uint8_t *data1, uint8_t *data2);

double read_gyro_r(int range, uint16_t *data, uint8_t *data1, uint8_t *data2);
double read_gyro_p(int range, uint16_t *data, uint8_t *data1, uint8_t *data2);
double read_gyro_y(int range, uint16_t *data, uint8_t *data1, uint8_t *data2);

#endif