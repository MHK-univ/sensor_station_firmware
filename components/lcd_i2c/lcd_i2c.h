#pragma once
#include <cstdint>
#include "i2c_bus.h"

// HD44780 16x2 문자 LCD + PCF8574 I2C 백팩 (4비트 모드)
class LcdI2c {
public:
    explicit LcdI2c(I2cBus& bus) : bus_(bus) {}
    esp_err_t init();
    void clear();
    void print(uint8_t row, const char* text);  // 16자로 자르고 공백 패딩
    void backlight(bool on);

private:
    void expanderWrite(uint8_t data);
    void write4(uint8_t high_nibble, bool rs);
    void send(uint8_t val, bool rs);

    I2cBus& bus_;
    i2c_master_dev_handle_t dev_ = nullptr;
    uint8_t bl_ = 0x08;  // 백라이트 비트
};
