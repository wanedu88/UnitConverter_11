# PRD — 확장 가능한 C++ 길이 단위 변환기 (UnitConverter_11)

**문서 버전:** 1.0  
**기반:** Phase 4 Epic / User Stories / Gherkin / Level 5 체크리스트  
**범위:** README, UnitConverter.cpp 리팩터, Catch2 테스트, config, .cursorrules  
**금지:** 구현 코드·클래스 설계·빌드 스크립트 (본 문서는 요구사항만 정의)

---

# 1. 프로젝트 개요

## 1.1 한 줄 목적문 (What / Who / Why)

| 요소 | 내용 |
|------|------|
| **What** | meter 기준 길이 단위 변환 CLI와 계약 기반 테스트·레이어 분리 구조 |
| **Who** | C++17·클린 아키텍처·TDD를 학습하는 개발자 |
| **Why** | 단일 파일 변환기를 **계약 고정·회귀 보호·확장 가능한 구조**로 전환하여 실무 설계 역량을 측정 가능하게 습득 |

## 1.2 배경 및 문제 정의 (관찰 관점)

현재 저장소에는 `unit:value` 입력을 받아 meter·feet·yard로 환산해 표 형식으로 출력하는 단일 실행 구조가 있다. 파싱·환산·출력·에러 처리가 한 흐름에 묶여 있고, 비율이 소스에 고정되어 있으며, 음수 검증·설정 로드·동적 단위·JSON/CSV 출력은 없다. README는 OCP/SRP, 입력 검증, 설정 외부화, 동적 등록, 다중 출력을 요구하고 6시간 실습 Activities를 정의한다. 학습자는 “숫자가 맞는지”보다 **계약(에러 문구, exit code, 비율, 좌측 입력 보존)** 을 테스트로 소유해야 하며, 성공은 데모 실행이 아니라 **인수 테스트·커버리지·회귀 정책 충족**으로 판정한다.

## 1.3 목표 (측정 가능)

| ID | 목표 | 측정 |
|----|------|------|
| G-01 | README 비율·`meter:2.5` 예시와 환산 결과 일치 | Catch2 ratio 테스트 ε ≤ 1×10⁻⁴ (표시값 기준) |
| G-02 | 입력·에러·출력 계약 100% 테스트화 | ERR-* 8종 각 ≥1 GREEN; Gherkin GH-01~08 매핑 테스트 존재 |
| G-03 | Entity / Control / Boundary 의존 방향 준수 | Entity가 boundary·control 헤더 미포함; 정적 검토 0 위반 |
| G-04 | 단위 추가 시 환산 if-else 분기 0 증가 | cubit 등록 후 Converter 분기 증가 0줄; US-02·US-06 GREEN |
| G-05 | 커버리지 임계 충족 | Entity ≥95%, Boundary ≥85%, Data ≥90%, Control 100% |

## 1.4 비목표 (Non-Goal)

| ID | 비목표 |
|----|--------|
| NG-01 | 웹 UI, GUI, REST API 제공 |
| NG-02 | 질량·온도·부피 등 길이 외 단위계 |
| NG-03 | 다중 사용자·동시성·DB 영속화 |

---

# 2. 사용자 및 이해관계자

## 2.1 타깃 사용자 (페르소나 1개)

| 항목 | 내용 |
|------|------|
| **이름** | 학습자 Min |
| **역량** | C++ 기초, 포인터/STL 읽기 가능; TDD·BCE는 초급 |
| **목표** | 6시간 내 계약·테스트·레이어 분리 산출물 제출 |
| **제약** | AI 보조 사용 가능; 인수는 Catch2·체크리스트로만 판정 |

**이해관계자:** 강사(인수·회고), CI(회귀·커버리지), 본인(Traceability 표 작성).

## 2.2 주요 사용 시나리오 (Phase 4 Journey 기반)

| ID | 시나리오 | Journey 단계 | 결과 |
|----|----------|--------------|------|
| SC-01 | README·UnitConverter.cpp 분석 후 계약 초안 작성 | Awareness → Entry | 입력·에러·비율·POL-OUT·POL-NEG 문서 |
| SC-02 | Entity RED→GREEN 후 Boundary Mock 테스트 | Action | 파싱·직렬화·stderr golden 분리 |
| SC-03 | config 로드·cubit 등록·통합 검증 후 회고 | Validation → Outcome | GH-01~08·US-01~07 GREEN, Traceability 표 |

---

# 3. 기능 요구사항

## 3.1 핵심 기능 목록

| ID | 기능 | 우선순위 |
|----|------|----------|
| F-01 | `unit:value` 파싱 및 검증 | **필수** |
| F-02 | meter 기준 Registry 환산 (builtin meter/feet/yard) | **필수** |
| F-03 | 등록된 모든 단위로 일괄 변환 출력 | **필수** |
| F-04 | table 기본 콘솔 출력 (POL-OUT) | **필수** |
| F-05 | 음수 거부·0 허용 (POL-NEG) | **필수** |
| F-06 | Catch2 단위·통합·계약 테스트 | **필수** |
| F-07 | JSON / CSV 출력 선택 | **권장** |
| F-08 | JSON 설정 파일에서 비율 로드 | **권장** |
| F-09 | 런타임 동적 단위 등록 | **권장** |
| F-10 | YAML 설정 | **선택** |

## 3.2 기능별 입·출력 계약 (문자열 수준)

### F-01 입력 파싱

| 항목 | 계약 |
|------|------|
| **입력** | 한 줄 문자열: `{unit}:{value}` |
| **unit** | `[a-z][a-z0-9_]*`, 길이 1~32; 대문자 입력 시 소문자 정규화(정책 고정 시) |
| **value** | 10진 실수; `std::stod` 실패 시 거부 |
| **성공** | 구조체 `{ unit, value }`, `value >= 0` |
| **실패 stderr** | `Invalid format. Use unit:value (ex: meter:2.5)` — 콜론 없음 |
| | `Invalid number: {token}` — 파싱 실패 |
| | `Negative value not allowed: {value}` — POL-NEG |
| **exit** | 성공 0, 실패 1 |

### F-02·F-03 환산

| 항목 | 계약 |
|------|------|
| **입력** | 검증된 `{ unit, value }`, Registry에 unit 존재 |
| **처리** | `meters = value × factorToMeter(unit)`; `out = meters / factorToMeter(target)` |
| **출력** | 대상 단위별 `{ target_unit, converted_value }` 집합, 개수 = Registry 단위 수 |
| **실패** | `Unknown unit: {unit}`, exit 1, stdout 빈 |

### F-04 table 출력 (POL-OUT)

| 항목 | 계약 |
|------|------|
| **입력** | 원 입력 `value`, `unit`; 변환 결과 목록 |
| **출력 줄** | `{value} {unit} = {converted} {target_unit}` (좌측은 사용자 입력 그대로) |
| **순서** | `listUnits()` 사전순(lexicographic) |
| **반올림** | 우측 `converted` 소수 4자리, half-up |

### F-07 JSON

| 항목 | 계약 |
|------|------|
| **출력** | `{"input":{"unit":"…","value":n},"conversions":[{"unit":"…","value":n},…]}` |
| **수치** | table과 동일 반올림 규칙 |

### F-08 설정 로드

| 항목 | 계약 |
|------|------|
| **입력** | JSON 파일 경로 |
| **성공** | Registry에 `units[]` 반영 |
| **실패** | `Config load failed: {reason}`, exit 1 |

### F-09 동적 등록

| 항목 | 계약 |
|------|------|
| **입력** | `register:{unit}:{factor_to_meter}` (한 줄) |
| **성공** | Registry +1; 이후 변환·convertAll에 포함 |
| **실패** | `Unit registration failed: {reason}` (중복·비율≤0) |

## 3.3 제약 사항 (Gherkin Background와 일치)

| 제약 | 규칙 | Gherkin / Policy |
|------|------|------------------|
| **기준 단위** | meter | Background: base unit is meter |
| **builtin 비율** | 1 meter = 3.28084 feet; 1 meter = 1.09361 yard | Background Given |
| **입력 형식** | 반드시 `:` 1개로 unit·value 분리 | GH-02 |
| **음수** | 거부; 0 허용 | POL-NEG-01~04, GH-05, GH-08 |
| **미지원 단위** | 변환 없음; `Unknown unit: {unit}` | GH-07 |
| **성공 시 stderr** | 빈 문자열 | Background + GH-01 |
| **좌측 보존** | 출력 모든 줄 좌측 = 입력 value·unit 원문 | POL-OUT-01~03, GH-01, GH-06, GH-08 |
| **파싱 실패** | 비숫자·다중 소수점 → `Invalid number: {token}` | GH-03, GH-04 |

### POL-NEG (음수 입력 정책)

| Rule | Detail |
|------|--------|
| POL-NEG-01 | Numeric value must be `>= 0`. Zero is allowed. |
| POL-NEG-02 | Negative values (including `-0.0` if parsed as negative) are rejected. |
| POL-NEG-03 | On rejection: exit code `1`, no conversion lines on stdout. |
| POL-NEG-04 | stderr: `Negative value not allowed: {value}` |

### POL-OUT (표현 계약)

| Rule | Detail |
|------|--------|
| POL-OUT-01 | Every conversion line includes the **original** input value and unit on the left-hand side. |
| POL-OUT-02 | Table line pattern: `{input_value} {input_unit} = {converted_value} {target_unit}` |
| POL-OUT-03 | Example: input `meter:2.5` → line contains `2.5 meter = … feet` |

---

# 4. 비기능 요구사항

## 4.1 기술 스택

| 항목 | 고정값 |
|------|--------|
| 언어 | C++17 이상 |
| 빌드 | CMake (CI 권장; README Makefile 호환 가능) |
| 테스트 | Catch2 v3.x |
| 포맷 | clang-format (.clang-format) |

## 4.2 아키텍처 원칙

| 레이어 | 책임 | 의존 |
|--------|------|------|
| **Entity** | Quantity, UnitRegistry, UnitConverter, DomainError | 표준 라이브러리만 |
| **Control** | ConvertLength, RegisterUnit, LoadConfig 유스케이스 | Entity, Data 포트 |
| **Boundary** | Parse, Validate, Format, CliApp, stderr, exit | Control (Entity 직접 환산 금지) |

- **SRP:** 클래스당 변경 이유 1개.
- **OCP:** 신규 단위 = Registry·config 항목·Formatter 전략 추가; Converter 분기 확장 금지.
- **금지:** Entity에 iostream·파일·JSON; Boundary에 환산 상수; `catch(...)` 삼키기; 전역 mutable Registry.

## 4.3 테스트 커버리지 목표

| 레이어 | 라인 | 오류 경로 |
|--------|------|-----------|
| Entity | ≥ 95% | InvalidQuantity, UnknownUnit, DuplicateUnit, EmptyRegistry |
| Boundary | ≥ 85% | ERR-* 8종 각 100% (≥1 테스트/코드) |
| Data | ≥ 90% | 파일 없음, JSON 오류, factor≤0 |
| Control | 100% | 유스케이스 메서드 각 ≥1 호출 |

## 4.4 확장성 원칙

- 신규 단위 추가 시 **수정 허용:** Registry 등록, config JSON 항목, Formatter 등록.
- **수정 금지:** meter 정규화 공식, ERR-* 전문, POL-OUT 좌측 패턴, builtin golden 테스트 기대값.
- feet↔yard는 **meter 경유만**; 독립 비율 하드코딩 금지.

---

# 5. 데이터 요구사항

## 5.1 단위 비율 상수 (meter 허브)

| 단위 | factor_to_meter (1 unit = k meter) | README 근거 |
|------|-----------------------------------|-------------|
| meter | 1.0 | 기준 |
| feet | 0.3048 (= 1 / 3.28084) | 1 m = 3.28084 ft |
| yard | 0.9144 (= 1 / 1.09361) | 1 m = 1.09361 yd |

**검증:** `convert(1 meter → feet)` = 3.28084 ±1×10⁻⁴; `convert(1 meter → yard)` = 1.09361 ±1×10⁻⁴.

## 5.2 설정 외부화

| 항목 | 규칙 |
|------|------|
| **1차 형식** | JSON 파일 `config/units.json` |
| **스키마** | `{ "base_unit": "meter", "units": [ { "name": "feet", "factor_to_meter": 0.3048 }, … ] }` |
| **실패** | 파싱 오류·누락 파일·factor_to_meter ≤ 0 → `Config load failed: {reason}`, exit 1 |
| **테스트** | InMemory ConfigSource로 단위 테스트; 통합만 File |
| **YAML** | 선택; JSON GREEN 후 동일 스키마 매핑 |

## 5.3 동적 단위 등록 계약

| 항목 | 규칙 |
|------|------|
| **형식** | `register:{unit_name}:{factor_to_meter}` (한 줄) |
| **예시** | `register:cubit:0.4572` → 1 cubit = 0.4572 meter |
| **성공** | Registry에 cubit 추가; `cubit:1` → meter 0.4572 ±1×10⁻⁴ |
| **실패** | 중복 name → registration failed; factor ≤ 0 → InvalidRatio |
| **불변** | 등록 후 builtin 3단위 환산 테스트 전부 기존 golden 통과 |

---

# 6. 출력 요구사항

## 6.1 콘솔 기본 포맷 (table)

| 필드 | 규칙 |
|------|------|
| 줄 패턴 | `{input_value} {input_unit} = {converted_value} {target_unit}` |
| 좌측 | POL-OUT: 입력 value·unit **변경 없음** |
| 우측 | 4자리 half-up |
| 줄 수 | Registry 단위 개수와 동일 |
| 순서 | unit name lexicographic |

## 6.2 JSON (권장)

```json
{
  "input": { "unit": "string", "value": number },
  "conversions": [
    { "unit": "string", "value": number }
  ]
}
```

- `conversions.length` = Registry 단위 수.
- `value` = table과 동일 반올림.
- `input` = 사용자 입력 unit·value 원문.

## 6.3 CSV (권장)

| 행 | 내용 |
|----|------|
| 1행 헤더 | `source_unit,source_value,target_unit,target_value` |
| 데이터 | 각 target 1행; `source_*` = 입력 고정, `target_*` = 변환 결과 |

## 6.4 Table / JSON / CSV 일관성

| 규칙 | 검증 |
|------|------|
| 동일 입력·Registry·포맷에서 **수치 집합 동일** | parity 테스트 |
| 포맷만 상이, 환산 로직 1경로 | Entity 단일 convertAll |

---

# 7. 성공 지표

## 7.1 인수 기준 (Phase 4 Story AC 정합)

| ✓ | ID | 인수 기준 | Story / Gherkin |
|---|-----|-----------|-----------------|
| ☐ | AC-01 | `meter:2.5` exit 0; 좌측 `2.5 meter` 보존; feet·yard ε 이내 | US-01, US-03, US-04, GH-01 |
| ☐ | AC-02 | `meter2.5` / `meter:abc` / `feet:1.2.3` 각 계약 stderr·exit 1 | US-01, GH-02~04 |
| ☐ | AC-03 | `yard:-3` → `Negative value not allowed: -3`, stdout 빈 | US-01, GH-05 |
| ☐ | AC-04 | `lightyear:1` → `Unknown unit: lightyear` | US-06, GH-07 |
| ☐ | AC-05 | cubit 등록 후 convertAll 4항목; `meter:2.5` builtin golden 유지 | US-02, US-06 |
| ☐ | AC-06 | 깨진 config → Config load failed, stdout 빈 | US-05 |
| ☐ | AC-07 | table·json·csv 동일 수치; 미지원 포맷 ERR-OUTPUT | US-04 |
| ☐ | AC-08 | Entity≥95%, Boundary≥85%, ERR-* 8종 GREEN | Epic E-COV, E-CON |

## 7.2 회귀 보호 규칙

| ID | 규칙 | 위반 시 |
|----|------|---------|
| REG-01 | Background 비율 3.28084·1.09361 변경 금지 | 테스트 먼저 RED, 승인 후만 변경 |
| REG-02 | ERR-* stderr 전문·exit code 변경 금지 | golden diff ≠ 0 → CI 실패 |
| REG-03 | POL-OUT 좌측 패턴 변경 금지 | GH-01, GH-06, GH-08 실패 |
| REG-04 | `meter:2.5` E2E golden 유지 | T-INT-01 diff 0 |
| REG-05 | refactor는 전 테스트 GREEN 후만 | tdd_rules refactor_phase |

---

# 8. 용어 정의 (Glossary)

| 용어 | 정의 |
|------|------|
| **기준 단위 (base unit)** | meter; 모든 환산의 허브. `factor_to_meter`는 1단위 = k meter. |
| **Registry** | UnitId와 ConversionRatio의 등록·조회 집합; 환산 시 유일 출처. |
| **Quantity** | `(unit, value)` 쌍; `value >= 0` 불변. |
| **factor_to_meter** | 1 해당 단위가 몇 meter인지 나타내는 양수 상수. |
| **POL-NEG** | 음수 거부·0 허용·stderr `Negative value not allowed: {value}` 정책 세트. |
| **POL-OUT** | 출력 좌측에 사용자 입력 value·unit을 변경 없이 반복하는 표현 계약. |
| **계약 테스트** | stderr 전문, exit code, stdout 패턴/JSON/CSV 스키마를 assert하는 Catch2 테스트. |
| **Golden** | `meter:2.5`, builtin 비율, ERR-* 메시지 등 회귀 기준 고정값. |
| **Entity / Control / Boundary** | BCE 레이어; Entity=순수 도메인, Control=유스케이스, Boundary=CLI·파싱·포맷. |
| **Dual-Track TDD** | Entity 환산 Track과 Boundary 계약 Track을 Mock으로 병렬 RED→GREEN. |

---

# 부록 A — Gherkin 시나리오 인덱스 (Phase 4)

| # | 시나리오 | 추적 |
|---|----------|------|
| GH-01 | Happy path meter:2.5 | AC-01 |
| GH-02 | Invalid format (no colon) | AC-02 |
| GH-03 | Invalid number abc | AC-02 |
| GH-04 | Malformed decimal 1.2.3 | AC-02 |
| GH-05 | Negative yard:-3 | AC-03 |
| GH-06 | Feet input preservation | AC-01, POL-OUT |
| GH-07 | Unknown unit lightyear | AC-04 |
| GH-08 | Zero meter:0 | AC-01, POL-NEG |

---

# 부록 B — Phase 4 추적 매트릭스

| PRD 절 | Epic | Journey | Story | Gherkin |
|--------|------|---------|-------|---------|
| §1.3 G-01~05 | E-COV, E-CON, E-REG | J-04, J-05 | US-03, US-07 | GH-BG, GH-01 |
| §3 | F-01~09 | J-03 | US-01~06 | GH-01~08 |
| §6 | F-04, F-07 | — | US-04 | GH-01, GH-06, GH-08 |
| §7 | E-* | J-06 | US-* | GH-* |

---

*본 PRD는 `docs/PRD.md`로 관리하며, 구현·코드·빌드 스크립트를 포함하지 않는다.*
