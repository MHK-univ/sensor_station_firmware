#pragma once
#include "sensor_base.h"
#include "i2c_bus.h"

class Cm1107 : public ISensor {
public:
    explicit Cm1107(I2cBus& bus) : bus_(bus) {}
    const char* name() const override { return "CM1107"; }
    esp_err_t init() override;
    esp_err_t read(SensorData& out) override;

private:
    I2cBus& bus_;
    i2c_master_dev_handle_t dev_ = nullptr;
};
