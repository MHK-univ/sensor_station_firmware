#pragma once
#include "sensor_base.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

// GPIO3 = ADC1_CH2, 분압비 1/2
class Battery : public ISensor {
public:
    const char* name() const override { return "Battery"; }
    esp_err_t init() override;
    esp_err_t read(SensorData& out) override;

private:
    adc_oneshot_unit_handle_t adc_ = nullptr;
    adc_cali_handle_t cali_ = nullptr;
};
