#include "sensor_manager.h"
#include <cstdio>

void SensorManager::add(ISensor* s)
{
    sensors_.push_back({s, false});
}

void SensorManager::initAll()
{
    for (auto& e : sensors_) {
        esp_err_t r = e.sensor->init();
        e.ok = (r == ESP_OK);
        printf("[%s] init %s (%s)\n", e.sensor->name(), e.ok ? "OK" : "실패", esp_err_to_name(r));
    }
}

void SensorManager::readAll(SensorData& out)
{
    for (auto& e : sensors_) {
        if (!e.ok) continue;
        esp_err_t r = e.sensor->read(out);
        if (r != ESP_OK) printf("[%s] read 실패 (%s)\n", e.sensor->name(), esp_err_to_name(r));
    }
}
