#include <stdio.h>
#include <stdint.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define UART_PORT UART_NUM_1
#define TX_PIN 43
#define RX_PIN 44

void app_main(void)
{
    // 1) UART 설정: 9600bps, 8비트, 패리티 없음, 정지비트 1
    uart_config_t cfg = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(UART_PORT, 256, 0, 0, NULL, 0);
    uart_param_config(UART_PORT, &cfg);
    uart_set_pin(UART_PORT, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 2) "농도 알려줘" 명령 (데이터시트 규정값)
    const uint8_t query[4] = {0x11, 0x01, 0x01, 0xED};

    while (1) {
        uart_flush_input(UART_PORT);                 // 이전에 남은 데이터 비우기
        uart_write_bytes(UART_PORT, query, 4);       // 명령 전송

        // 3) 응답 16바이트 읽기 (최대 0.5초 대기)
        uint8_t rx[16];
        int len = uart_read_bytes(UART_PORT, rx, 16, pdMS_TO_TICKS(500));

        if (len != 16) {
            printf("응답 없음 (%d바이트 수신)\n", len);
        } else {
            // 4) 체크섬: 16바이트를 모두 더하면 0이 되어야 정상
            uint8_t sum = 0;
            for (int i = 0; i < 16; i++) sum += rx[i];

            if (rx[0] != 0x16 || sum != 0) {
                printf("체크섬 오류\n");
            } else {
                // 5) 값 계산
                float hcho = (rx[3] * 256 + rx[4]) / 1000.0;
                float temp = (rx[7] * 256 + rx[8]) / 10.0;
                float hum  = (rx[9] * 256 + rx[10]) / 10.0;
                printf("HCHO: %.3f ppm | 온도: %.1f C | 습도: %.1f %% | 상태: %d\n",
                       hcho, temp, hum, rx[13]);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}