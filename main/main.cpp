#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "board_config.h"
#include "i2c_bus.h"
#include "sensor_manager.h"
#include "bme280.h"
#include "pm2009.h"
#include "cm1107.h"
#include "cb_hcho.h"
#include "battery.h"
#include "lcd_i2c.h"

// LCD(16x2)에 측정값을 4페이지로 나눠 표시
static void showPage(LcdI2c& lcd, const SensorData& d, int page)
{
    char l1[40], l2[40];
    switch (page) {
    case 0:
        snprintf(l1, sizeof l1, "T:%.1fC H:%.0f%%", d.temp_c, d.humidity);
        snprintf(l2, sizeof l2, "P:%.1fhPa", d.pressure_hpa);
        break;
    case 1:
        snprintf(l1, sizeof l1, "PM1:%d 2.5:%d", d.pm1, d.pm25);
        snprintf(l2, sizeof l2, "PM10:%d", d.pm10);
        break;
    case 2:
        snprintf(l1, sizeof l1, "CO2:%dppm", d.co2_ppm);
        snprintf(l2, sizeof l2, "HCHO:%.3fppm", d.hcho_ppm);
        break;
    default:
        snprintf(l1, sizeof l1, "VBAT:%.2fV", d.vbat);
        l2[0] = '\0';
        break;
    }
    lcd.print(0, l1);
    lcd.print(1, l2);
}

extern "C" void app_main(void)
{
    I2cBus bus(board::I2C_SDA, board::I2C_SCL);
    ESP_ERROR_CHECK(bus.init());

    Bme280 bme(bus);
    Pm2009 pm(bus);
    Cm1107 co2(bus);
    CbHcho hcho(board::HCHO_UART_NUM, board::HCHO_TX, board::HCHO_RX);
    Battery bat;
    LcdI2c lcd(bus);

    SensorManager mgr;
    mgr.add(&bme);
    mgr.add(&pm);
    mgr.add(&co2);
    mgr.add(&hcho);
    mgr.add(&bat);
    mgr.initAll();

    bool lcd_ok = (lcd.init() == ESP_OK);
    printf("[LCD] init %s\n", lcd_ok ? "OK" : "실패");

    int page = 0;
    while (true) {
        SensorData d;
        mgr.readAll(d);
        printf("T:%.1fC H:%.1f%% P:%.1fhPa | PM1:%d PM2.5:%d PM10:%d | CO2:%dppm | HCHO:%.3fppm | VBAT:%.2fV\n",
               d.temp_c, d.humidity, d.pressure_hpa, d.pm1, d.pm25, d.pm10,
               d.co2_ppm, d.hcho_ppm, d.vbat);

        if (lcd_ok) showPage(lcd, d, page);
        page = (page + 1) % 4;

        vTaskDelay(pdMS_TO_TICKS(board::SAMPLE_PERIOD_MS));
    }
}
