#include "cb_hcho.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t CbHcho::init()
{
    uart_config_t cfg = {};
    cfg.baud_rate = 9600;
    cfg.data_bits = UART_DATA_8_BITS;
    cfg.parity = UART_PARITY_DISABLE;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    cfg.source_clk = UART_SCLK_DEFAULT;

    uart_port_t port = (uart_port_t)uart_;
    esp_err_t r = uart_driver_install(port, 256, 0, 0, NULL, 0);
    if (r != ESP_OK) return r;
    r = uart_param_config(port, &cfg);
    if (r != ESP_OK) return r;
    return uart_set_pin(port, tx_, rx_, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

esp_err_t CbHcho::read(SensorData& out)
{
    static const uint8_t query[4] = {0x11, 0x01, 0x01, 0xED};
    uart_port_t port = (uart_port_t)uart_;

    uart_flush_input(port);
    uart_write_bytes(port, query, 4);

    uint8_t rx[16];
    int len = uart_read_bytes(port, rx, 16, pdMS_TO_TICKS(500));
    if (len != 16) return ESP_ERR_TIMEOUT;

    uint8_t sum = 0;  // 16바이트 합 == 0
    for (int i = 0; i < 16; i++) sum += rx[i];
    if (rx[0] != 0x16 || sum != 0) return ESP_ERR_INVALID_CRC;

    out.hcho_ppm = (rx[3] * 256 + rx[4]) / 1000.0f;
    return ESP_OK;
}
