#pragma once
#include <cmath>
#include <cstdint>
#include "esp_err.h"

// 전체 센서 측정값 (미측정 값: float=NAN, int=-1)
struct SensorData {
    // BME280
    float temp_c       = NAN;
    float humidity     = NAN;
    float pressure_hpa = NAN;
    // PM2009
    int pm1  = -1;
    int pm25 = -1;
    int pm10 = -1;
    // CM1107
    int co2_ppm = -1;
    // CB-HCHO
    float hcho_ppm = NAN;
    // Battery
    float vbat = NAN;
};

// 모든 센서 클래스의 공통 인터페이스
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual const char* name() const = 0;
    virtual esp_err_t init() = 0;
    virtual esp_err_t read(SensorData& out) = 0;  // 자기 담당 필드만 채움
};
