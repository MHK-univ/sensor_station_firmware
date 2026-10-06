#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "board_config.h"
#include "i2c_bus.h"

// 임시 테스트: I2C 버스 클래스 동작 확인 (센서 3종 응답 여부)
extern "C" void app_main(void)
{
    I2cBus bus(board::I2C_SDA, board::I2C_SCL);
    if (bus.init() != ESP_OK) {
        printf("I2C 버스 초기화 실패\n");
        return;
    }
    const uint8_t addrs[] = {board::ADDR_BME280, board::ADDR_PM2009, board::ADDR_CM1107};
    while (true) {
        for (uint8_t a : addrs)
            printf("0x%02X: %s\n", a, bus.probe(a) == ESP_OK ? "OK" : "응답 없음");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
