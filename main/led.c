#include "led.h"


void init(spi_device_handle_t *handle) {
    spi_bus_config_t bus_conf = {
        .mosi_io_num = SDA, 
        .miso_io_num = -1, 
        .sclk_io_num = CLK, 
        .data_io_default_level = 0, 
        .max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE};
    spi_device_interface_config_t device_conf = {
        .mode = 0, 
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .clock_speed_hz = SPI_MASTER_FREQ_10M,
        .cs_ena_posttrans = 2,
        .spics_io_num = CS,
        .queue_size = 1};
    gpio_config_t out_conf = {
        .pin_bit_mask = ((1ULL << A0)|(1ULL << RST)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&out_conf);
    gpio_set_level(A0, 0);
    spi_bus_initialize(SPI3_HOST, &bus_conf, SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI3_HOST, &device_conf, handle);
}

void send(wrapper *w) {
    spi_transaction_t trans_conf = {
        .length = 8*w->byte_length,
        .tx_buffer = w->buf
    };
    spi_device_transmit(*(w->handle), &trans_conf);
}

void send_cmd(wrapper *w) {
    gpio_set_level(A0, 0);
    send(w);
}

void send_data(wrapper *w) {
    gpio_set_level(A0, 1);
    send(w);
}

void start(wrapper *w) {
    w->byte_length = 1;
    init(w->handle);
    gpio_set_level(RST, 0);
    vTaskDelay(10);
    gpio_set_level(RST, 1);
    w->buf[0] = 0x01;
    send_cmd(w); 
    vTaskDelay(50);
    w->buf[0] = 0x11;
    send_cmd(w);
    vTaskDelay(150);
    w->buf[0] = 0x29;
    send_cmd(w);
}

void colmod(wrapper *w) {
    w->byte_length = 1;
    w->buf[0] = 0x3a;
    send_cmd(w);
    w->buf[0] = 0x03;
    send_data(w);
}

void cset(wrapper *w, uint8_t start, uint8_t end) {
    w->byte_length = 1;
    w->buf[0] = 0x2a;
    send_cmd(w);
    w->byte_length = 4;
    w->buf[0] = 0x00;
    w->buf[1] = start;
    w->buf[2] = 0x00;
    w->buf[3] = end;
    send_data(w);
}

void rset(wrapper *w, uint8_t start, uint8_t end) {
    w->byte_length = 1;
    w->buf[0] = 0x2b;
    send_cmd(w);
    w->byte_length = 4;
    w->buf[0] = 0x00;
    w->buf[1] = start;
    w->buf[2] = 0x00;
    w->buf[3] = end;
    send_data(w);
}

void whitescreen(wrapper *w) {
    cset(w, 0x00, 0x7F);
    rset(w, 0x00, 0x7F);
    w->byte_length = 1;
    w->buf[0] = 0x2c;
    send_cmd(w);
    w->byte_length = 24; // 16
    for(int i = 0; i < 24; i++) {
        w->buf[i] = 0xff;
    }
    for(int i = 0; i < 1024; i++) { //1024 = 128 * 128 / 16
        send_data(w);
    }
}

void blackscreen(wrapper *w) {
    cset(w, 0x00, 0x7F);
    rset(w, 0x00, 0x7F);
    w->byte_length = 1;
    w->buf[0] = 0x2c;
    send_cmd(w);
    w->byte_length = 24; // 16
    for(int i = 0; i < 24; i++) {
        w->buf[i] = 0x00;
    }
    for(int i = 0; i < 1024; i++) {
        send_data(w);
    }
}

void draw_dot(wrapper *w, dot *d, uint8_t color) {
    cset(w, d->x, d->x+3); //not going to worry about wrap around issues rn
    rset(w, d->y, d->y+3);
    w->byte_length = 1;
    w->buf[0] = 0x2c;
    send_cmd(w);
    w->byte_length = 24;
    if(color == 1) {
        for(int i = 0; i < 8; i++) {
        w->buf[3*i] = 0x0f;
        w->buf[3*i+1] = 0x00;
        w->buf[3*i+2] = 0xf0;
        }
    }
    else if(color == 2) {
        for(int i = 0; i < 8; i++) {
        w->buf[3*i] = 0x00;
        w->buf[3*i+1] = 0xf0;
        w->buf[3*i+2] = 0x0f;
        }
    }
    else {
        for(int i = 0; i < 24; i++) {
            w->buf[i] = 0x00;
        }
    }
    send_data(w);
}

void draw_dotline(wrapper *w, uint8_t x_start, uint8_t y_start, uint8_t color, uint8_t xy) {
    if(xy == 0) {
        cset(w, x_start, x_start+3); //not going to worry about wrap around issues rn
        rset(w, y_start, y_start);
    }
    else {
        cset(w, x_start, x_start); //not going to worry about wrap around issues rn
        rset(w, y_start, y_start+3);
    }
    w->byte_length = 1;
    w->buf[0] = 0x2c;
    send_cmd(w);
    w->byte_length = 6;
    if(color == 1) {
        for(int i = 0; i < 2; i++) {
        w->buf[3*i] = 0x0f;
        w->buf[3*i+1] = 0x00;
        w->buf[3*i+2] = 0xf0;
        }
    }
    else {
        for(int i = 0; i < 6; i++) {
            w->buf[i] = 0x00;
        }
    }
    send_data(w);
}