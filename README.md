# CAP_SensorStation

Edge AI 기반 환경 센서 스테이션 및 실시간 모니터링 플랫폼 — ESP32-S3 펌웨어

> 영남대학교 캡스톤 디자인 | 상태: 개발 중 (v0.1.0-dev)

## 개요

실내 환경(온·습도, 미세먼지, CO₂, 포름알데히드)을 하나의 보드에서 측정하고, 엣지(ESP32-S3)에서 분석하여 실시간 모니터링 플랫폼으로 전송하는 센서 스테이션입니다. 정전 대비 배터리와 OTA 펌웨어 업데이트를 지원하는 것을 목표로 합니다.

## 주요 기능

- [x] 센서 4종 개별 측정 (I2C / UART)
- [x] 통합 회로 설계 (Overall.SchDoc)
- [ ] 센서 통합 측정 루프
- [ ] 엣지 AI 추론
- [ ] 무선 데이터 전송 (실시간 모니터링 플랫폼)
- [ ] OTA 펌웨어 업데이트
- [ ] LCD 표시
- [ ] 배터리 전원 전환 (정전 대비)

## 하드웨어 구성

| 구분 | 부품 | 측정 항목 | 통신 |
|---|---|---|---|
| MCU | LOLIN S3 Pro (ESP32-S3) | - | - |
| 온습도/기압 | GY-BME280-5V | 온도, 습도, 기압 | I2C (0x76) |
| 미세먼지 | PM2009 | PM1.0 / PM2.5 / PM10 | I2C (0x28) |
| CO₂ | CM1107 | CO₂ (NDIR) | I2C (0x31) |
| 포름알데히드 | CB-HCHO-V4S | HCHO | UART |
| 전원 | LiPo 3.7V (JST-PH 2.0) + DC-DC 5V 승압 | - | - |

### 핀 연결

| 신호 | ESP32-S3 | 연결 대상 |
|---|---|---|
| I2C SDA | GPIO9 | BME280, PM2009, CM1107 (공용 버스, 4.7kΩ 풀업 → 3.3V) |
| I2C SCL | GPIO14 | BME280, PM2009, CM1107 (공용 버스, 4.7kΩ 풀업 → 3.3V) |
| UART TX | TXD0 | CB-HCHO RXD |
| UART RX | RXD0 | CB-HCHO TXD |
| BT+ / BT- | 배터리 커넥터 | LiPo 3.7V |

### 전원 구성

```
LiPo 3.7V ──► LOLIN S3 Pro (BT+/BT-) ──VIN──► DC-DC 5V 승압 ──► PM2009, CM1107, CB-HCHO (5V)
                                        └──────────────────────► BME280 (VIN)
```

- PM2009, CM1107은 5V ±0.1V가 필요하여 승압 컨버터를 사용합니다.
- 외부 전원은 USB-C 전원 전용 판넬 단자(5V)를 사용할 예정입니다.

## 개발 환경

- ESP-IDF v6.1
- Visual Studio Code + ESP-IDF 확장
- Target: `esp32s3`

## 빌드 및 업로드

```bash
git clone https://github.com/MHK-univ/CAP_SensorStation.git
cd CAP_SensorStation
idf.py set-target esp32s3
idf.py build
idf.py -p COMx flash monitor
```

`COMx`는 본인 PC의 포트 번호로 변경하세요. (VS Code: 하단 상태바의 포트/플래시 버튼 사용 가능)

## 폴더 구조

```
CAP_SensorStation/
├─ main/            # 애플리케이션 로직
├─ components/      # 센서별 드라이버 (bme280, cm1107, pm2009, cb_hcho)
├─ docs/            # 회로도, 데이터시트, 테스트 결과
├─ hardware/        # 회로도(SchDoc), PCB, 패키징(STEP)
├─ Reference/       # 센서별 단독 테스트 코드 (보관용)
├─ sdkconfig.defaults
└─ partitions.csv   # OTA 파티션 테이블
```

## 개발 규칙

- 브랜치: `main`(안정) / `dev`(개발)
- 커밋 접두사: `feat:` `fix:` `docs:` `refactor:`
- 릴리스: 태그(`v0.x.0`) + GitHub Releases에 `.bin` 첨부

## 로드맵

1. 센서 4종 통합 측정 및 로그 출력
2. 배터리·5V 승압 전원 검증
3. 엣지 AI API 선정 및 적용
4. 데이터 전송 및 모니터링 플랫폼 연동
5. OTA 업데이트
6. PCB 제작 및 패키징

## 팀

4팀 — 영남대학교 (총 4인)

## 라이선스

TBD
