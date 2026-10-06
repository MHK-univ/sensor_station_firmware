#include "cm1107.h"
#include "board_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t Cm1107::init()
{
    dev_ = bus_.addDevice(board::ADDR_CM1107, board::SPEED_CM1107);
    return dev_ ? ESP_OK : ESP_FAIL;
}

esp_err_t Cm1107::read(SensorData& out)
{
    for (int n = 0; n < 3; n++) {
        uint8_t cmd = 0x01;
        uint8_t rx[5];
        i2c_master_transmit(dev_, &cmd, 1, 200);
        vTaskDelay(pdMS_TO_TICKS(100));
        if (i2c_master_receive(dev_, rx, 5, 200) == ESP_OK &&
            (uint8_t)(0 - (rx[0] + rx[1] + rx[2] + rx[3])) == rx[4]) {
            out.co2_ppm = rx[1] * 256 + rx[2];
            return ESP_OK;
        }
    }
    return ESP_FAIL;
}
