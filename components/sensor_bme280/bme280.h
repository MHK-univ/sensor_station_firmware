#pragma once
#include "sensor_base.h"
#include "i2c_bus.h"

class Bme280 : public ISensor {
public:
    explicit Bme280(I2cBus& bus) : bus_(bus) {}
    const char* name() const override { return "BME280"; }
    esp_err_t init() override;
    esp_err_t read(SensorData& out) override;

private:
    esp_err_t regRead(uint8_t reg, uint8_t* data, size_t len);
    esp_err_t regWrite(uint8_t reg, uint8_t val);
    esp_err_t readCalibration();

    I2cBus& bus_;
    i2c_master_dev_handle_t dev_ = nullptr;
    // 공장 보정 계수
    uint16_t T1_ = 0, P1_ = 0;
    int16_t T2_ = 0, T3_ = 0;
    int16_t P2_ = 0, P3_ = 0, P4_ = 0, P5_ = 0, P6_ = 0, P7_ = 0, P8_ = 0, P9_ = 0;
    uint8_t H1_ = 0, H3_ = 0;
    int16_t H2_ = 0, H4_ = 0, H5_ = 0;
    int8_t H6_ = 0;
};
