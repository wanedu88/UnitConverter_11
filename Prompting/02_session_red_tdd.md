# UnitConverter_11 — 대화 세션 기록 (RED / TDD / QA)
_Exported on 2026-05-21 from Cursor agent session (UnitConverter_11 workspace)_

**워크스페이스:** `c:\DEV\UnitConverter_11`  
**선행 문서:** [01_spec.md](01_spec.md) (문제 정의·Phase 4~5 설계 대화)  
**본 세션 주제:** 테스트 계획 · Catch2 · Dual-Track RED · 결함 문서 · 보고서

---

## 세션 요약

| 단계 | 사용자 요청 (요지) | 산출물 | 비고 |
|------|-------------------|--------|------|
| 1 | README/PRD 기준 샘플 예제 1개 선택 (코드 금지) | 선택 결과 문서 (응답) | `meter:2.5` → feet `8.2021` |
| 2 | test_plan.md 작성 (Catch2, 경계, gcov) | [docs/test_plan.md](../docs/test_plan.md) | 시니어 QA 리드 |
| 3 | README에 RED To-Do 리스트 섹션 삽입 | [README.md](../README.md) § RED | Track A/B, 커버리지, 결함 연결 |
| 4 | Catch2 테스트 작성 (타입별 ≥5, Green) | `src/entity/*`, `src/boundary/*`, `tests/test_*.cpp`, `CMakeLists.txt` | **36/36 PASS** |
| 5 | 빌드 실패 로그 분석 (로그 미첨부) | 결함 분석 응답 (DEF-001 등) | `expected 8.202100 but got 0.000000` |
| 6 | defect_list.md 정리 | [docs/defect_list.md](../docs/defect_list.md) | DEF-001~018 |
| 7 | Dual-Track RED 명세만 (구현/GREEN/REFACTOR 금지) | [docs/red_tests.md](../docs/red_tests.md) | 12 시나리오 |
| 8 | Catch2 스켈레톤 `FAIL("RED")` only | `tests/red_track_*.cpp`, `unit_converter_red_tests` | **12/12 FAIL** |
| 9 | Report/02_RED.md | [Report/02_RED.md](../Report/02_RED.md) | RED 단계 보고서 |
| 10 | Prompting에 대화 저장 | 본 파일 | |

---

## 1. 샘플 예제 선택 (테스트 플랜용)

**선택 기능:** `meter:2.5` 입력 시 **meter → feet** 변환  

| 항목 | 값 |
|------|-----|
| README 요구 | requirement.md **1번**(입출력), **4번**(정확도 테스트) |
| PRD | AC-01, GH-01 |
| 입력 | `meter:2.5` |
| feet 행 | `2.5 meter = 8.2021 feet` (PRD 4자리 half-up) |
| Entity ε | `8.20210`, ε ≤ 1e-5 |
| 비율 | **1 meter = 3.28084 feet** |

**선택 이유:** REG-04 golden, 비율·POL-OUT·happy/경계(`meter:0`)를 한 패턴으로 검증 가능.

---

## 2. 사용자 프롬프트 원문 요약

### Turn 1 — 샘플 예제 선택
```
README + PRD 참고, test_plan용 샘플 1개
기준: 핵심 변환, 비즈니스 로직(3.28084/1.09361), happy+경계
출력: 기능명, 요구사항 번호, 입출력, 이유 2줄 / 코드 금지
```

### Turn 2 — 테스트 계획서
```
Catch2, 경계값 6종, 예외, 동적등록, config JSON/YAML
Domain 95%+, Boundary 85%+, gcov/lcov 전략
→ docs/test_plan.md
```

### Turn 3 — README RED 체크리스트
```
## RED 단계 To-Do 리스트 (Track A/B, 커버리지, 결함 연결)
기존 내용 유지, 섹션만 삽입
```

### Turn 4 — Catch2 테스트 (GREEN)
```
아이템 타입별 최소 5개, Given-When-Then, test_[타입]_[조건]_[기대]
cmake build Green
→ Entity/Boundary 구현 + 6 test files, 36 tests
```

### Turn 5 — 결함 분석
```
실패 로그 붙여넣기 placeholder — 로그 없음
expected 8.202100 vs 0.000000 분석, 심각도, 최소 diff, Green 절차
```

### Turn 6 — defect_list.md
```
표 형식 DEF-xxx, README 결함 체크박스 업데이트
```

### Turn 7 — Dual-Track RED 명세
```
구현/GREEN/REFACTOR 금지, Track A 6 + Track B 6
출력: # UI RED / # Logic RED 형식
```

### Turn 8 — RED 스켈레톤
```
TEST_CASE + FAIL("RED") 한 줄만
```

### Turn 9 — 02_RED.md 보고서
```
Report/02_RED.md
```

### Turn 10 — Prompting 저장
```
Prompting에 현재까지 대화내용 저장
```

---

## 3. 에이전트 수행 결과 (산출물·결정)

### 3.1 문서

| 파일 | 역할 |
|------|------|
| `docs/test_plan.md` | TP-UC11-001, BV-xx, E2E, gcov Phase A/B |
| `docs/defect_list.md` | DL-UC11-001, DEF-001~018, Open/Fixed |
| `docs/red_tests.md` | RED-A/B 명세, baseline 실패 예상 |
| `Report/02_RED.md` | RED 단계 보고서 |
| `README.md` | RED To-Do, 관련 문서 링크 |

### 3.2 코드 (GREEN 트랙 — Turn 4)

```text
src/entity/
  ConversionConstants.hpp   # 3.28084, 1.09361, ε
  UnitRegistry.hpp/cpp
  UnitConverter.hpp/cpp     # hub: meters = v * factor; out = m / factor

src/boundary/
  InputParser.hpp/cpp       # invalid_argument
  InputValidator.hpp/cpp    # POL-NEG
  ConfigLoader.hpp/cpp      # JSON/YAML simple parse; missing → builtins
  OutputFormatter.hpp/cpp   # POL-OUT, half-up 4

tests/
  test_normal_conversion.cpp
  test_boundary_values.cpp
  test_exceptions.cpp
  test_dynamic_registration.cpp
  test_config_load.cpp
  test_boundary_contract.cpp

config/units.json
tests/fixtures/  units.yaml, bad.json, zero_factor.json
CMakeLists.txt   # uc11_entity, uc11_boundary, Catch2 FetchContent
```

### 3.3 RED 스켈레톤 (Turn 8)

```text
tests/red_track_a_boundary.cpp   # 6 × FAIL("RED")  [red][boundary]
tests/red_track_b_domain.cpp     # 6 × FAIL("RED")  [red][domain]
target: unit_converter_red_tests  # Catch2 only, 12 failed
```

### 3.4 빌드·테스트 (Windows)

```powershell
cmake -G Ninja -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure

# GREEN
unit_converter_tests.exe     → 36/36 PASS

# RED
unit_converter_red_tests.exe → 12/12 FAIL ("RED")
```

CMake: Win32에서 `g++` 자동 탐지 추가.

---

## 4. 핵심 계약 (세션 전반 고정)

| ID | 내용 |
|----|------|
| 비율 | 1 m = 3.28084 ft; 1 m = 1.09361 yd |
| 허브 | feet↔yard meter 경유만 |
| 입력 | `{unit}:{value}` |
| 예외 | `std::invalid_argument` (README RED Track A) |
| POL-NEG | value ≥ 0, 0 허용 |
| POL-OUT | 좌측 입력 value·unit 원문 |
| 표시 | 우측 4자리 half-up |
| Golden | `meter:2.5`, feet 8.2021 / raw 8.20210 (ε 1e-5) |
| TC-B-07 | config 없음 → builtin 3.28084 / 1.09361 유지 |

---

## 5. Track A / B RED ↔ Catch2 매핑

| RED ID | TEST_CASE | Track |
|--------|-----------|-------|
| RED-A-01 | `test_parse_meter_25_returns_conversion_result` | A |
| RED-A-02 | `test_parse_no_colon_throws_invalid_argument` | A |
| RED-A-03 | `test_validate_meter_neg10_throws_invalid_argument` | A |
| RED-A-04 | `test_convert_parsec10_throws_unknown_unit` | A |
| RED-A-05 | `test_format_table_preserves_input_meter_25` | A |
| RED-A-06 | `test_format_json_meter_25_valid_schema` | A |
| RED-B-01 | `test_convert_meter_to_feet_within_1e5` | B |
| RED-B-02 | `test_convert_meter_to_yard_ratio_109361` | B |
| RED-B-03 | `test_convert_all_meter10_returns_three_units` | B |
| RED-B-04 | `test_register_cubit_then_convert_to_meter` | B |
| RED-B-05 | `test_load_config_json_applies_feet_factor` | B |
| RED-B-06 | `test_load_config_missing_keeps_builtin_ratios` | B |

---

## 6. 결함·리스크 (세션에서 문서화)

| ID | 심각도 | 요지 |
|----|--------|------|
| DEF-001 | Critical | meter→feet 0.0 (meter 분기 누락) |
| DEF-003 | Major | legacy 음수 미검증 |
| DEF-004 | Minor | legacy 4자리 half-up 없음 |
| DEF-017~018 | Info | Windows NMake / linker |

**README 결함 체크:**
- [x] defect_list.md
- [x] Catch2 36/36 (Entity 경로)
- [ ] legacy Open 4건

---

## 7. 미완료 / GREEN 진입 시 할 일

1. `red_track_*.cpp`에서 `FAIL("RED")` → 실제 assertion 교체  
2. JSON formatter (RED-A-06) 구현  
3. legacy `UnitConverter.cpp` DEF-003·004·010·013 또는 deprecated  
4. `03_GREEN.md` 보고서 (권장)  
5. gcov Entity ≥95%, Boundary ≥85% CI 게이트  

---

## 8. 사용자 규칙·제약 (세션 준수)

- Git commit / push — 사용자 요청 시만  
- RED 단계: 구현·GREEN·REFACTOR 금지 (Turn 7~8)  
- Turn 4에서는 요청에 따라 **GREEN 구현 허용** → 36 tests  
- 최소 scope, Catch2, Given-When-Then, `test_*_*_*` 네이밍  

---

## 9. 관련 링크 (저장소)

| 문서 | 경로 |
|------|------|
| PRD | [docs/PRD.md](../docs/PRD.md) |
| TODO | [docs/TODO.md](../docs/TODO.md) |
| Spec 보고서 | [Report/01_spec.md](../Report/01_spec.md) |
| RED 보고서 | [Report/02_RED.md](../Report/02_RED.md) |

---

*본 파일은 에이전트 세션의 요청·응답·산출물을 요약한다. 전체 턴 원문은 Cursor chat export가 아니며, 재현·GREEN 작업 시 [docs/red_tests.md](../docs/red_tests.md)와 RED 스켈레톤을 정본으로 사용한다.*
