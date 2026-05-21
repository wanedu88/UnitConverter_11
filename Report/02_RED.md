# UnitConverter_11 — RED 단계 보고서

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_11 — Dual-Track UI + Logic TDD |
| **작성일** | 2026-05-21 |
| **단계** | **RED** (실패 테스트 작성 · 구현/GREEN/REFACTOR 미수행) |
| **선행 보고서** | [01_spec.md](01_spec.md) (Phase 5 요구·계약 정본화) |
| **관련 문서** | [docs/red_tests.md](../docs/red_tests.md), [docs/test_plan.md](../docs/test_plan.md), [docs/defect_list.md](../docs/defect_list.md) |

---

## 1. 요약 (Executive Summary)

본 단계에서는 baseline **`UnitConverter.cpp`**(단일 `main()`, if-else, `std::invalid_argument` 없음)를 기준으로 **Dual-Track RED**를 수행했다.

| Track | 범위 | RED 산출물 | 현재 판정 |
|-------|------|------------|-----------|
| **A — UI / Boundary** | 파싱·검증·포맷·JSON | 명세 6건 + Catch2 스켈레톤 6건 | **12/12 FAIL** (`FAIL("RED")`) |
| **B — Domain / Logic** | `convert` · `convertAll` · `registerUnit` · `loadConfig` | 명세 6건 + Catch2 스켈레톤 6건 | 동일 |

**원칙 준수:** 구현 코드 추가·GREEN assertion·REFACTOR **금지**. RED는 “의도적 실패”로 계약을 고정한다.

**비율 고정 (Background):**

- **1 meter = 3.28084 feet**
- **1 meter = 1.09361 yard**
- feet ↔ yard는 **meter 허브만** (독립 비율 하드코딩 금지)

---

## 2. RED 배경 및 목표

### 2.1 Baseline 상태 (RED 대상)

```text
UnitConverter.cpp
  ├── main() 단일 진입
  ├── if-else: meter / feet / yard
  ├── std::invalid_argument 없음
  ├── POL-NEG(음수) 미검증
  ├── POL-OUT / 4자리 half-up / JSON 미구현
  └── convert(), Registry, loadConfig API 없음
```

### 2.2 RED 목표

| # | 목표 |
|---|------|
| 1 | README·PRD 계약을 **실패하는 테스트**로 고정 |
| 2 | UI( Boundary )와 Logic( Entity ) 책임 **분리 검증** |
| 3 | 대표 golden `meter:2.5` → feet **8.202100** (ε ≤ 1e-5) 회귀 기준 선언 |
| 4 | 결함·재현 경로를 [defect_list.md](../docs/defect_list.md)에 추적 |

### 2.3 범위 외 (본 보고서)

- Entity/Boundary **GREEN 구현** (별도 마일스톤·`unit_converter_tests` 36건)
- REFACTOR · 커버리지 gcov 게이트 달성
- legacy CLI 동작 완전 대체

---

## 3. Dual-Track RED 명세

정본: [docs/red_tests.md](../docs/red_tests.md)

### 3.1 Track A — UI / Boundary (6건)

| ID | Catch2 이름 | Given (요지) | Then (계약) | Invariant |
|----|-------------|--------------|-------------|-----------|
| RED-A-01 | `test_parse_meter_25_returns_conversion_result` | `"meter:2.5"` | feet ≈ **8.202100** | `v × 3.28084` |
| RED-A-02 | `test_parse_no_colon_throws_invalid_argument` | `"meter2.5"` | `invalid_argument` + FORMAT 메시지 | 환산 0회 |
| RED-A-03 | `test_validate_meter_neg10_throws_invalid_argument` | `"meter:-1.0"` | POL-NEG stderr/예외 | 음수 결과 금지 |
| RED-A-04 | `test_convert_parsec10_throws_unknown_unit` | `"parsec:1.0"` | `Unknown unit: parsec` | stdout 빈 |
| RED-A-05 | `test_format_table_preserves_input_meter_25` | table 출력 | 좌측 `2.5 meter` 보존 | POL-OUT |
| RED-A-06 | `test_format_json_meter_25_valid_schema` | JSON 요청 | `input` + `conversions[]` 스키마 | table parity |

### 3.2 Track B — Domain / Logic (6건)

| ID | Catch2 이름 | Scenario | Invariant |
|----|-------------|----------|-----------|
| RED-B-01 | `test_convert_meter_to_feet_within_1e5` | `convert(meter, 2.5, feet)` | hub; ε ≤ 1e-5 |
| RED-B-02 | `test_convert_meter_to_yard_ratio_109361` | `convert(meter, 1.0, yard)` | **1.09361** |
| RED-B-03 | `test_convert_all_meter10_returns_three_units` | `convertAll` 3단위 | \|R\| = 결과 수 |
| RED-B-04 | `test_register_cubit_then_convert_to_meter` | cubit 0.4572 | REG-04 golden 불변 |
| RED-B-05 | `test_load_config_json_applies_feet_factor` | JSON/YAML 로드 | factor 0.3048 |
| RED-B-06 | `test_load_config_missing_keeps_builtin_ratios` | 없는 경로 | 3.28084 / 1.09361 유지 |

---

## 4. Catch2 RED 스켈레톤

### 4.1 파일 구성

| 파일 | 클래스/모듈 | 테스트 수 | 본문 |
|------|-------------|-----------|------|
| [tests/red_track_a_boundary.cpp](../tests/red_track_a_boundary.cpp) | Boundary RED | 6 | `FAIL("RED");` only |
| [tests/red_track_b_domain.cpp](../tests/red_track_b_domain.cpp) | Domain RED | 6 | `FAIL("RED");` only |

**예시 (전 테스트 동일 패턴):**

```cpp
TEST_CASE("test_convert_meter_to_feet_within_1e5", "[red][domain][RED-B-01]") {
    FAIL("RED");
}
```

### 4.2 CMake 타깃

| 타깃 | 용도 | 링크 |
|------|------|------|
| `unit_converter_red_tests` | **RED 전용** (12건) | Catch2 only |
| `unit_converter_tests` | GREEN 회귀 (36건, 별도 트랙) | `uc11_entity` + `uc11_boundary` |

`catch_discover_tests`로 CTest 등록.

### 4.3 실행 결과 (2026-05-21)

```text
.\build\unit_converter_red_tests.exe

test cases:  12 | 12 failed
assertions:  12 | 12 failed
message:     explicitly with message: RED
```

**판정:** RED 단계 **정상** — 모든 스켈레톤이 의도적으로 실패.

```powershell
cmake -G Ninja -S . -B build
cmake --build build
.\build\unit_converter_red_tests.exe -r red    # 태그 필터
ctest --test-dir build -R unit_converter_red_tests
```

---

## 5. README RED 체크리스트 연동

[README.md](../README.md) § RED 단계 To-Do 리스트와 매핑:

| README ID | RED ID | 스켈레톤 |
|-----------|--------|----------|
| TC-A-01 | RED-A-01 | ✓ |
| TC-A-02 | RED-A-02 | ✓ |
| TC-A-03 | RED-A-03 | ✓ |
| TC-A-04 | RED-A-04 | ✓ |
| TC-A-05~06 | RED-A-05, A-06 | ✓ |
| TC-B-01 | RED-B-01 | ✓ |
| TC-B-02 | RED-B-02 | ✓ |
| TC-B-03~04 | RED-B-03, B-04 | ✓ |
| TC-B-05~06 | RED-B-05, B-06 | ✓ |

**결함 목록:** [docs/defect_list.md](../docs/defect_list.md) 생성 완료 (DEF-001~018).  
baseline Open: DEF-003·004·010·013 (음수·반올림·`catch(...)`·단위 trim).

---

## 6. 대표 실패 시나리오 (RED 분석)

| 증상 | 기대 | 실제 (baseline) | 결함 ID | 심각도 |
|------|------|-----------------|---------|--------|
| `meter:2.5` → feet | 8.202100 | 0.000000 (분기 누락 시) | DEF-001 | Critical |
| `meter:-1.0` | 예외 | 음수 출력 | DEF-003 | Major |
| `meter2.5` | FORMAT 예외 | 미정의 동작 | DEF-009 | Major |
| JSON 출력 | 스키마 | 미구현 | RED-A-06 | — |

상세 재현·수정 요약은 defect_list 표 참조.

---

## 7. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| RED 명세 (Given/When/Then) | [docs/red_tests.md](../docs/red_tests.md) | 완료 |
| 테스트 계획 | [docs/test_plan.md](../docs/test_plan.md) | 완료 |
| 결함 목록 | [docs/defect_list.md](../docs/defect_list.md) | 완료 |
| Track A 스켈레톤 | [tests/red_track_a_boundary.cpp](../tests/red_track_a_boundary.cpp) | 완료 |
| Track B 스켈레톤 | [tests/red_track_b_domain.cpp](../tests/red_track_b_domain.cpp) | 완료 |
| 본 보고서 | [Report/02_RED.md](02_RED.md) | 완료 |

---

## 8. GREEN 단계 진입 조건 (다음 작업)

| 순서 | 작업 | 완료 기준 |
|------|------|-----------|
| 1 | `FAIL("RED")` → 실제 `REQUIRE` / `REQUIRE_THROWS` 교체 | RED-A/B 각 Then 충족 |
| 2 | Boundary: `InputParser`, `InputValidator`, `OutputFormatter` (+ JSON) | Track A GREEN |
| 3 | Entity: `UnitRegistry`, `UnitConverter`, `ConfigLoader` | Track B GREEN |
| 4 | `unit_converter_red_tests` 12/12 PASS | ctest GREEN |
| 5 | `unit_converter_tests` 36/36 유지 (REG-04) | 회귀 |
| 6 | legacy `UnitConverter.cpp` Open 결함 4건 처리 또는 deprecated | DEF-003 등 Close |

**금지 (GREEN 전):** 테스트 기대값 완화·`FAIL` 제거만으로 통과시키기 · 구현 없이 스킵.

---

## 9. 리스크 및 가정

| 리스크 | 완화 |
|--------|------|
| RED/GREEN 테스트 이중 트랙 혼동 | `unit_converter_red_tests` vs `unit_converter_tests` 분리 유지 |
| README `8.2` vs PRD `8.2021` | 테스트는 **4자리 half-up** + raw ε 1e-5 병기 |
| Windows CMake NMake 실패 | Ninja + g++ ([CMakeLists.txt](../CMakeLists.txt) Win32 탐지) |
| `loadConfig` 실패 vs 기본값 | TC-B-07 / RED-B-06 정책 문서화 유지 |

---

## 10. 결론

- **Dual-Track RED 12건**이 명세·스켈레톤·CTest로 고정되었고, 전건 **`FAIL("RED")`** 로 실패한다.
- 계약(비율·POL-NEG·POL-OUT·ERR 메시지)은 [red_tests.md](../docs/red_tests.md)와 [defect_list.md](../docs/defect_list.md)에 추적 가능하다.
- **GREEN**은 스켈레톤 본문 교체 + BCE 최소 구현으로 진행하며, 별도 **03_GREEN.md** 보고서에서 인수·커버리지를 기록하는 것을 권장한다.

---

*본 문서는 RED 단계 산출물만 기술한다. GREEN 구현 상세는 코드·테스트 diff 및 후속 보고서에서 다룬다.*
