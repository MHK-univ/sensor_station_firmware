#include "battery.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static constexpr adc_channel_t BAT_CH = ADC_CHANNEL_2;
static constexpr int SAMPLES = 16;
static constexpr float DIVIDER = 2.0f;

esp_err_t Battery::init()
{
    adc_oneshot_unit_init_cfg_t unit_cfg = {};
    unit_cfg.unit_id = ADC_UNIT_1;
    esp_err_t r = adc_oneshot_new_unit(&unit_cfg, &adc_);
    if (r != ESP_OK) return r;

    adc_oneshot_chan_cfg_t ch_cfg = {};
    ch_cfg.atten = ADC_ATTEN_DB_12;
    ch_cfg.bitwidth = ADC_BITWIDTH_DEFAULT;
    r = adc_oneshot_config_channel(adc_, BAT_CH, &ch_cfg);
    if (r != ESP_OK) return r;

    adc_cali_curve_fitting_config_t cali_cfg = {};
    cali_cfg.unit_id = ADC_UNIT_1;
    cali_cfg.atten = ADC_ATTEN_DB_12;
    cali_cfg.bitwidth = ADC_BITWIDTH_DEFAULT;
    return adc_cali_create_scheme_curve_fitting(&cali_cfg, &cali_);
}

esp_err_t Battery::read(SensorData& out)
{
    int sum = 0;
    for (int i = 0; i < SAMPLES; i++) {
        int raw = 0, mv = 0;
        esp_err_t r = adc_oneshot_read(adc_, BAT_CH, &raw);
        if (r != ESP_OK) return r;
        r = adc_cali_raw_to_voltage(cali_, raw, &mv);
        if (r != ESP_OK) return r;
        sum += mv;
        vTaskDelay(pdMS_TO_TICKS(2));
    }
    out.vbat = (sum / (float)SAMPLES) * DIVIDER / 1000.0f;
    return ESP_OK;
}
