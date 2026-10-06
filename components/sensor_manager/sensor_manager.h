#pragma once
#include <vector>
#include "sensor_base.h"

class SensorManager {
public:
    void add(ISensor* s);
    void initAll();                  // init 실패한 센서는 read에서 제외
    void readAll(SensorData& out);   // 센서별 결과를 out에 통합

private:
    struct Entry { ISensor* sensor; bool ok; };
    std::vector<Entry> sensors_;
};
