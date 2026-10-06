#pragma once
#include <cstdint>
#include "driver/gpio.h"

namespace board {

// I2C 공용 버스
constexpr gpio_num_t I2C_SDA = GPIO_NUM_9;
constexpr gpio_num_t I2C_SCL = GPIO_NUM_10;

// I2C 주소
constexpr uint8_t ADDR_BME280 = 0x76;
constexpr uint8_t ADDR_PM2009 = 0x28;
constexpr uint8_t ADDR_CM1107 = 0x31;

// I2C 속도 (센서별)
constexpr uint32_t SPEED_BME280 = 100000;
constexpr uint32_t SPEED_PM2009 = 50000;
constexpr uint32_t SPEED_CM1107 = 20000;

// CB-HCHO UART
constexpr gpio_num_t HCHO_TX = GPIO_NUM_43;
constexpr gpio_num_t HCHO_RX = GPIO_NUM_44;

// 측정 주기
constexpr uint32_t SAMPLE_PERIOD_MS = 2000;

}  // namespace board
