#include "pm2009.h"
#include "board_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t Pm2009::init()
{
    dev_ = bus_.addDevice(board::ADDR_PM2009, board::SPEED_PM2009);
    return dev_ ? ESP_OK : ESP_FAIL;
}

esp_err_t Pm2009::read(SensorData& out)
{
    for (int n = 0; n < 3; n++) {
        uint8_t rx[32];
        if (i2c_master_receive(dev_, rx, 32, 300) == ESP_OK && rx[0] == 0x16) {
            uint8_t cs = 0;  // 앞 31바이트 XOR == 마지막 바이트
            for (int i = 0; i < 31; i++) cs ^= rx[i];
            if (cs == rx[31]) {
                out.pm1  = rx[7]  * 256 + rx[8];
                out.pm25 = rx[9]  * 256 + rx[10];
                out.pm10 = rx[11] * 256 + rx[12];
                return ESP_OK;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    return ESP_FAIL;
}
