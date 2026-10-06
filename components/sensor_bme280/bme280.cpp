#include "bme280.h"
#include "board_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t Bme280::regRead(uint8_t reg, uint8_t* data, size_t len)
{
    return i2c_master_transmit_receive(dev_, &reg, 1, data, len, 100);
}

esp_err_t Bme280::regWrite(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {reg, val};
    return i2c_master_transmit(dev_, buf, 2, 100);
}

esp_err_t Bme280::readCalibration()
{
    uint8_t c[26], e[7];
    esp_err_t r = regRead(0x88, c, 26);
    if (r != ESP_OK) return r;
    r = regRead(0xE1, e, 7);
    if (r != ESP_OK) return r;
    T1_ = c[0] | (c[1] << 8);   T2_ = c[2] | (c[3] << 8);   T3_ = c[4] | (c[5] << 8);
    P1_ = c[6] | (c[7] << 8);   P2_ = c[8] | (c[9] << 8);   P3_ = c[10] | (c[11] << 8);
    P4_ = c[12] | (c[13] << 8); P5_ = c[14] | (c[15] << 8); P6_ = c[16] | (c[17] << 8);
    P7_ = c[18] | (c[19] << 8); P8_ = c[20] | (c[21] << 8); P9_ = c[22] | (c[23] << 8);
    H1_ = c[25];
    H2_ = e[0] | (e[1] << 8);
    H3_ = e[2];
    H4_ = ((int8_t)e[3] << 4) | (e[4] & 0x0F);
    H5_ = ((int8_t)e[5] << 4) | (e[4] >> 4);
    H6_ = (int8_t)e[6];
    return ESP_OK;
}

esp_err_t Bme280::init()
{
    dev_ = bus_.addDevice(board::ADDR_BME280, board::SPEED_BME280);
    if (!dev_) return ESP_FAIL;

    uint8_t id = 0;
    esp_err_t r = regRead(0xD0, &id, 1);
    if (r != ESP_OK) return r;
    if (id != 0x60) return ESP_ERR_NOT_FOUND;  // 0x58 = BMP280(습도 없음)

    regWrite(0xE0, 0xB6);  // 소프트 리셋
    vTaskDelay(pdMS_TO_TICKS(10));
    r = readCalibration();
    if (r != ESP_OK) return r;
    regWrite(0xF2, 0x01);  // 습도 측정 (0xF4보다 먼저)
    return regWrite(0xF4, 0x27);  // 온도/기압 + 연속 측정
}

esp_err_t Bme280::read(SensorData& out)
{
    uint8_t d[8];
    esp_err_t r = regRead(0xF7, d, 8);
    if (r != ESP_OK) return r;

    int32_t adc_P = ((int32_t)d[0] << 12) | (d[1] << 4) | (d[2] >> 4);
    int32_t adc_T = ((int32_t)d[3] << 12) | (d[4] << 4) | (d[5] >> 4);
    int32_t adc_H = (d[6] << 8) | d[7];

    double v1 = (adc_T / 16384.0 - T1_ / 1024.0) * T2_;
    double v2 = (adc_T / 131072.0 - T1_ / 8192.0);
    v2 = v2 * v2 * T3_;
    double t_fine = v1 + v2;
    double temp = t_fine / 5120.0;

    v1 = t_fine / 2.0 - 64000.0;
    v2 = v1 * v1 * P6_ / 32768.0;
    v2 = v2 + v1 * P5_ * 2.0;
    v2 = v2 / 4.0 + P4_ * 65536.0;
    v1 = (P3_ * v1 * v1 / 524288.0 + P2_ * v1) / 524288.0;
    v1 = (1.0 + v1 / 32768.0) * P1_;
    double press = 0;
    if (v1 != 0) {
        press = 1048576.0 - adc_P;
        press = (press - v2 / 4096.0) * 6250.0 / v1;
        v1 = P9_ * press * press / 2147483648.0;
        v2 = press * P8_ / 32768.0;
        press = press + (v1 + v2 + P7_) / 16.0;
    }

    double h = t_fine - 76800.0;
    h = (adc_H - (H4_ * 64.0 + H5_ / 16384.0 * h)) *
        (H2_ / 65536.0 * (1.0 + H6_ / 67108864.0 * h * (1.0 + H3_ / 67108864.0 * h)));
    h = h * (1.0 - H1_ * h / 524288.0);
    if (h > 100.0) h = 100.0;
    if (h < 0.0) h = 0.0;

    out.temp_c = (float)temp;
    out.humidity = (float)h;
    out.pressure_hpa = (float)(press / 100.0);
    return ESP_OK;
}
