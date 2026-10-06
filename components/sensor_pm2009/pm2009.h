#pragma once
#include "sensor_base.h"
#include "i2c_bus.h"

class Pm2009 : public ISensor {
public:
    explicit Pm2009(I2cBus& bus) : bus_(bus) {}
    const char* name() const override { return "PM2009"; }
    esp_err_t init() override;
    esp_err_t read(SensorData& out) override;

private:
    I2cBus& bus_;
    i2c_master_dev_handle_t dev_ = nullptr;
};
