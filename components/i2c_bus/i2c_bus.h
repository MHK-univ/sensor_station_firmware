#pragma once
#include <cstdint>
#include "driver/i2c_master.h"
#include "esp_err.h"

// I2C 버스 1개를 만들고, 센서별로 주소/속도를 달리해 디바이스를 등록
class I2cBus {
public:
    I2cBus(gpio_num_t sda, gpio_num_t scl, i2c_port_num_t port = I2C_NUM_0);
    ~I2cBus();
    I2cBus(const I2cBus&) = delete;
    I2cBus& operator=(const I2cBus&) = delete;

    esp_err_t init();
    i2c_master_dev_handle_t addDevice(uint8_t addr, uint32_t speed_hz);  // 실패 시 nullptr
    esp_err_t probe(uint8_t addr, int timeout_ms = 100);

private:
    gpio_num_t sda_, scl_;
    i2c_port_num_t port_;
    i2c_master_bus_handle_t bus_ = nullptr;
};
