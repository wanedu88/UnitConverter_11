# UnitConverter_11 — RED 단계 보고서

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_11 — Dual-Track UI + Logic TDD |
| **작성일** | 2026-05-21 |
| **개정** | 2026-05-21 — ctest 48/48 FAIL 정합 (`UC11_GREEN_PHASE`) |
| **단계** | **RED** (실패 테스트 작성 · GREEN assertion 기본 비활성) |
| **선행 보고서** | [01_spec.md](01_spec.md) (Phase 5 요구·계약 정본화) |
| **관련 문서** | [docs/red_tests.md](../docs/red_tests.md), [docs/test_plan.md](../docs/test_plan.md), [docs/defect_list.md](../docs/defect_list.md) |

---

## 1. 요약 (Executive Summary)

본 단계에서는 baseline **`UnitConverter.cpp`**(단일 `main()`, if-else, `std::invalid_argument` 없음)를 기준으로 **Dual-Track RED**를 수행했다.

| Track | 범위 | 테스트 수 | RED 판정 (기본 빌드) |
|-------|------|-----------|----------------------|
| **A — UI / Boundary** | 파싱·검증·포맷·JSON | 12 (`red_track_*`) | **FAIL** (`FAIL("RED")`) |
| **A+B — GREEN 파일 (RED 가드)** | `test_*.cpp` | 36 (`unit_converter_tests`) | **FAIL** (`uc11_require_green_phase`) |
| **합계 (ctest)** | 두 실행 파일 | **48** | **48/48 FAIL** |

**원칙:** RED 단계에서 `ctest`는 **0% passed**가 정상이다. GREEN 검증은 `-DUC11_GREEN_PHASE=ON`으로만 수행한다.

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

### 4.2 CMake 타깃 · RED/GREEN 전환

| 타깃 | 테스트 수 | 기본(RED) | `-DUC11_GREEN_PHASE=ON` |
|------|-----------|-----------|-------------------------|
| `unit_converter_red_tests` | 12 | 12 FAIL | 12 FAIL (스켈레톤 유지) |
| `unit_converter_tests` | 36 | 36 FAIL | 36 PASS (구현 검증) |
| **ctest 합계** | **48** | **48 FAIL** | 36 PASS + 12 FAIL |

| CMake 옵션 | 기본값 | 역할 |
|------------|--------|------|
| `UC11_GREEN_PHASE` | **OFF** | OFF 시 `tests/TestPhase.hpp`가 `FAIL("RED")` 발생 |

```cmake
# CMakeLists.txt (요지)
option(UC11_GREEN_PHASE "TDD GREEN: enable real assertions" OFF)
# unit_converter_tests: UC11_GREEN_PHASE=1 일 때만 실제 REQUIRE 실행
```

```cpp
// tests/TestPhase.hpp
inline void uc11_require_green_phase() {
#if !defined(UC11_GREEN_PHASE)
    FAIL("RED");
#endif
}
```

`test_*.cpp` 36건 각 `TEST_CASE` 첫 줄에 `uc11_require_green_phase();` 적용.

### 4.3 실행 결과

#### (1) 이슈 — 48개 중 12개만 FAIL (2026-05-21 초기)

| 관찰 | 원인 |
|------|------|
| `ctest`: 36 passed, 12 failed | `unit_converter_tests`에 Entity/Boundary **조기 GREEN** 연결 |
| 사용자 기대 | RED 단계 = **48/48 FAIL** |

**판정:** 36 PASS는 RED 원칙 위반(테스트가 구현을 이미 검증).

#### (2) 조치 후 — 48/48 FAIL (정합)

```text
ctest --test-dir build

0% tests passed, 48 tests failed out of 48
```

| 구분 | 실패 메시지 |
|------|-------------|
| #1~#36 | `explicitly with message: RED` (`uc11_require_green_phase`) |
| #37~#48 | `explicitly with message: RED` (`red_track_*.cpp`) |

#### (3) 명령

```powershell
# RED (기본) — 48/48 FAIL
cmake -G Ninja -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure

# GREEN (36건만 통과 확인)
cmake -G Ninja -S . -B build -DUC11_GREEN_PHASE=ON
cmake --build build
ctest --test-dir build
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
| RED 가드 | [tests/TestPhase.hpp](../tests/TestPhase.hpp) | 완료 |
| 세션 기록 | [Prompting/02_session_red_tdd.md](../Prompting/02_session_red_tdd.md) | 완료 |
| 본 보고서 | [Report/02_RED.md](02_RED.md) | 완료 (본 개정) |

---

## 8. GREEN 단계 진입 조건 (다음 작업)

| 순서 | 작업 | 완료 기준 |
|------|------|-----------|
| 1 | `FAIL("RED")` → 실제 `REQUIRE` / `REQUIRE_THROWS` 교체 | RED-A/B 각 Then 충족 |
| 2 | Boundary: `InputParser`, `InputValidator`, `OutputFormatter` (+ JSON) | Track A GREEN |
| 3 | Entity: `UnitRegistry`, `UnitConverter`, `ConfigLoader` | Track B GREEN |
| 4 | `-DUC11_GREEN_PHASE=ON` → `unit_converter_tests` 36/36 PASS | ctest |
| 5 | `red_track_*.cpp` `FAIL("RED")` 제거 → 12/12 PASS | ctest 48/48 |
| 6 | `unit_converter_tests` REG-04 `meter:2.5` 유지 | 회귀 |
| 6 | legacy `UnitConverter.cpp` Open 결함 4건 처리 또는 deprecated | DEF-003 등 Close |

**금지 (GREEN 전):** 테스트 기대값 완화·`FAIL` 제거만으로 통과시키기 · 구현 없이 스킵.

---

## 9. 리스크 및 가정

| 리스크 | 완화 |
|--------|------|
| RED/GREEN 테스트 이중 트랙 혼동 | `UC11_GREEN_PHASE` OFF=RED / ON=GREEN; `red_track`은 GREEN까지 FAIL 유지 |
| 36 PASS during RED | `TestPhase.hpp` 가드 (본 보고서 §4.3) |
| README `8.2` vs PRD `8.2021` | 테스트는 **4자리 half-up** + raw ε 1e-5 병기 |
| Windows CMake NMake 실패 | Ninja + g++ ([CMakeLists.txt](../CMakeLists.txt) Win32 탐지) |
| `loadConfig` 실패 vs 기본값 | TC-B-07 / RED-B-06 정책 문서화 유지 |

---

## 10. 결론

- **Dual-Track RED 12건** + **GREEN 명세 36건** = ctest **48건**; 기본 빌드에서 **48/48 FAIL**이 RED 단계 정상 상태이다.
- 초기 **36 PASS / 12 FAIL**은 조기 GREEN 연결 때문이었으며, `TestPhase.hpp` + `UC11_GREEN_PHASE=OFF`로 정합하였다.
- 계약(비율·POL-NEG·POL-OUT·ERR)은 [red_tests.md](../docs/red_tests.md), [defect_list.md](../docs/defect_list.md)에 추적한다.
- **GREEN** 진입: `-DUC11_GREEN_PHASE=ON` (36 PASS) → `red_track` assertion 구현 (48 PASS) → **03_GREEN.md** 권장.

---

## 11. 검증 체크리스트 (QA 서명용)

| # | 항목 | 기대 | 확인 |
|---|------|------|------|
| 1 | `UC11_GREEN_PHASE` 기본 OFF | ON 없음 | ☐ |
| 2 | `ctest` 총 48건 | 48 failed | ☐ |
| 3 | 실패 메시지 `RED` | 48건 모두 | ☐ |
| 4 | GREEN 빌드 36 PASS | `-DUC11_GREEN_PHASE=ON` | ☐ |
| 5 | `red_track` 12건 | GREEN 전까지 FAIL | ☐ |

---

*본 문서는 RED 단계·ctest 정합 검증을 기술한다. GREEN 인수는 후속 03_GREEN.md에서 다룬다.*
