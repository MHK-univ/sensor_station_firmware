#include "lcd_i2c.h"
#include <cstring>
#include "board_config.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// PCF8574 비트 배치: P0=RS P1=RW P2=EN P3=BL P4~P7=D4~D7
static constexpr uint8_t RS = 0x01;
static constexpr uint8_t EN = 0x04;

void LcdI2c::expanderWrite(uint8_t data)
{
    i2c_master_transmit(dev_, &data, 1, 100);
}

void LcdI2c::write4(uint8_t high_nibble, bool rs)
{
    uint8_t d = (high_nibble & 0xF0) | bl_ | (rs ? RS : 0);
    expanderWrite(d | EN);
    esp_rom_delay_us(2);
    expanderWrite(d & ~EN);
    esp_rom_delay_us(60);
}

void LcdI2c::send(uint8_t val, bool rs)
{
    write4(val & 0xF0, rs);
    write4((val << 4) & 0xF0, rs);
}

esp_err_t LcdI2c::init()
{
    dev_ = bus_.addDevice(board::ADDR_LCD, board::SPEED_LCD);
    if (!dev_) return ESP_FAIL;
    if (bus_.probe(board::ADDR_LCD) != ESP_OK) return ESP_ERR_NOT_FOUND;

    vTaskDelay(pdMS_TO_TICKS(50));
    write4(0x30, false); vTaskDelay(pdMS_TO_TICKS(5));
    write4(0x30, false); vTaskDelay(pdMS_TO_TICKS(2));
    write4(0x30, false); vTaskDelay(pdMS_TO_TICKS(2));
    write4(0x20, false);              // 4비트 모드 전환
    send(0x28, false);                // 4비트, 2줄, 5x8
    send(0x0C, false);                // 화면 ON, 커서 OFF
    send(0x06, false);                // 입력 시 커서 우측 이동
    clear();
    return ESP_OK;
}

void LcdI2c::clear()
{
    send(0x01, false);
    vTaskDelay(pdMS_TO_TICKS(3));
}

void LcdI2c::print(uint8_t row, const char* text)
{
    send(0x80 | (row ? 0x40 : 0x00), false);  // 커서 위치
    size_t len = strlen(text);
    for (size_t i = 0; i < 16; i++)
        send(i < len ? (uint8_t)text[i] : ' ', true);
}

void LcdI2c::backlight(bool on)
{
    bl_ = on ? 0x08 : 0x00;
    expanderWrite(bl_);
}
