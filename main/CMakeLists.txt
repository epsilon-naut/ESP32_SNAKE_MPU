idf_component_register(SRCS "move.c" "snake.c" "led.c" "mpu.c" "main.c"
                    INCLUDE_DIRS "." REQUIRES esp_driver_gpio esp_driver_spi esp_driver_i2c)