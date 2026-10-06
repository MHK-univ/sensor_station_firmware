#pragma once
#include "sensor_base.h"
#include "driver/gpio.h"

class CbHcho : public ISensor {
public:
    CbHcho(int uart_num, gpio_num_t tx, gpio_num_t rx)
        : uart_(uart_num), tx_(tx), rx_(rx) {}
    const char* name() const override { return "CB-HCHO"; }
    esp_err_t init() override;
    esp_err_t read(SensorData& out) override;

private:
    int uart_;
    gpio_num_t tx_, rx_;
};
