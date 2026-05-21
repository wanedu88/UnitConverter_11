# UnitConverter_11 — GREEN 단계 보고서

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_11 — Dual-Track UI + Logic TDD |
| **작성일** | 2026-05-21 |
| **개정** | 2026-05-21 — ctest 71/71 PASS, 커버리지 게이트 충족 |
| **단계** | **GREEN** (최소 구현으로 TC·RED assertion 통과) |
| **선행 보고서** | [02_RED.md](02_RED.md) (RED 48/48 FAIL 정합) |
| **관련 문서** | [docs/red_tests.md](../docs/red_tests.md), [docs/test_plan.md](../docs/test_plan.md), [docs/defect_list.md](../docs/defect_list.md) |
| **브랜치** | `A_01` (PR 대상, base: `main`) |

---

## 1. 요약 (Executive Summary)

RED 단계에서 고정한 **14개 TC**(TC-A-01~07, TC-B-01~07)와 **Dual-Track RED 12건**에 대해, **커밋 단위 최소 구현(TDD GREEN)** 으로 전 테스트 통과 및 커버리지 게이트를 달성했다.

| 지표 | 목표 | 실측 |
|------|------|------|
| **TC-A/B (14건)** | 14 PASS | **14/14 PASS** |
| **ctest 전체** | 0 failures | **71/71 PASS** |
| **Domain 커버리지** | ≥ 95% | **95.5%** (42/44 lines) |
| **Boundary 커버리지** | ≥ 85% | **87.1%** (121/139 lines) |
| **비율 인라인** | `src/`·legacy에 3.28084/1.09361 금지 | **상수만** (`ConversionConstants.hpp`) |
| **main() 변환 로직** | Domain/Boundary 위임 | **위임 완료** |

**비율 고정 (불변):**

- **1 meter = 3.28084 feet** → `kMeterToFeet`
- **1 meter = 1.09361 yard** → `kMeterToYard`
- 환산은 **meter 허브** (`factorToMeter` 기반)만 사용

---

## 2. GREEN 배경 및 목표

### 2.1 RED 대비 변화

| 항목 | RED (이전) | GREEN (본 단계) |
|------|------------|-----------------|
| `UC11_GREEN_PHASE` | OFF (기본) | **ON** (기본) |
| `red_track_*.cpp` | `FAIL("RED")` only | **실제 REQUIRE** |
| `unit_converter_tests` | 가드로 FAIL | **36/36 PASS** |
| `UnitConverter.cpp` (legacy) | if-else + 인라인 비율 | **API 위임** |
| Entity `convert()` | 없음 / stub | **허브 공식** |
| `formatJson()` | 미구현 | **구현** |

### 2.2 GREEN 목표

| # | 목표 | 상태 |
|---|------|------|
| 1 | TC-A-01~07, TC-B-01~07 각각 **한 커밋씩** 최소 구현 | ✓ (10 GREEN 커밋 + 1 완료 커밋) |
| 2 | RED-A/B 스켈레톤 → 실제 assertion | ✓ |
| 3 | `ctest` **0 failures** | ✓ 71/71 |
| 4 | Entity ≥ 95%, Boundary ≥ 85% (lcov) | ✓ |
| 5 | legacy `main()`에서 변환 로직 제거 | ✓ |
| 6 | PR `A_01` → `main` 리뷰·머지 준비 | ✓ push 완료 |

### 2.3 범위 외 (본 보고서)

- **REFACTOR** (이름·구조 정리, Control 레이어 분리)
- CI 파이프라인 자동화 (lcov gate 스크립트)
- legacy CLI E2E stdin 자동화 전체

---

## 3. 커밋 순서 및 구현 매핑

TDD GREEN은 **한 번에 한 TC 묶음**만 구현·커밋했다 (REFACTOR 금지).

| 순서 | Track | TC | 구현 요지 | 커밋 | 해시 |
|------|-------|-----|-----------|------|------|
| 1 | B | TC-B-01 | `convert(meter→feet)` 최소 분기 | `feat(green): meter to feet` | `a09a333` |
| 2 | A | TC-A-02 | `:` 없음 → `invalid_argument` | `feat(green): validate missing colon` | `b477af1` |
| 3 | B | TC-B-02 | `convert(meter→yard)` | `feat(green): meter to yard` | `d6c163c` |
| 4 | A | TC-A-03 | 음수 → `invalid_argument` | `feat(green): validate negative value` | `ad54624` |
| 5 | B | TC-B-03 | `convert(feet→meter)` 역변환 | `feat(green): feet to meter reverse` | `7a917e4` |
| 6 | A | TC-A-04 | 없는 단위 → `Unknown unit` | `feat(green): validate unknown unit` | `8be6f20` |
| 7 | B | TC-B-04~05 | `convertAll` + `registerUnit` (허브 전환) | `feat(green): convertAll and registerUnit` | `dead2f3` |
| 8 | A | TC-A-01,06,07 | Happy path·포맷·경계 0 | `feat(green): boundary happy path` | `6e5fc88` |
| 9 | B | TC-B-06~07 | `loadConfig` 정상·fallback | `feat(green): loadConfig with fallback` | `71dbc01` |
| — | A+B | 완료·게이트 | TC-A-05, RED 교체, legacy, lcov | `chore(green): complete TC suite...` | `aeaf7c0` |

---

## 4. TC 검증 결과 (14건)

### 4.1 Track A — Boundary

| TC ID | 태그 | 시나리오 | 결과 |
|-------|------|----------|------|
| TC-A-01 | `[TC-A-01]` | `"meter:2.5"` 파싱·검증·feet 환산 | **PASS** |
| TC-A-02 | `[TC-A-02]` | `"meter2.5"` → `Invalid format` | **PASS** |
| TC-A-03 | `[TC-A-03]` | `"meter:-1.0"`, NaN → POL-NEG | **PASS** |
| TC-A-04 | `[TC-A-04]` | `"parsec:1.0"` → `Unknown unit` | **PASS** |
| TC-A-05 | `[TC-A-05]` | `"meter:abc"`, `:2.5`, `feet:1.2.3` | **PASS** |
| TC-A-06 | `[TC-A-06]` | table 좌측 `2.5 meter` 보존 | **PASS** |
| TC-A-07 | `[TC-A-07]` | `meter:0` → feet 0 | **PASS** |

**테스트 파일:** [tests/test_parser.cpp](../tests/test_parser.cpp), [tests/test_boundary.cpp](../tests/test_boundary.cpp)

### 4.2 Track B — Domain

| TC ID | 태그 | 시나리오 | 결과 |
|-------|------|----------|------|
| TC-B-01 | `[TC-B-01]` | `meter,2.5→feet` ≈ 8.20210 (ε 1e-5) | **PASS** |
| TC-B-02 | `[TC-B-02]` | `meter,1.0→yard` ≈ 1.09361 | **PASS** |
| TC-B-03 | `[TC-B-03]` | `feet,1.0→meter` ≈ 0.30480 | **PASS** |
| TC-B-04 | `[TC-B-04]` | `convertAll(meter,1.0)` 3건 | **PASS** |
| TC-B-05 | `[TC-B-05]` | `registerUnit(cubit)` 후 환산 | **PASS** |
| TC-B-06 | `[TC-B-06]` | `config/units.json` 로드 | **PASS** |
| TC-B-07 | `[TC-B-07]` | 없는 경로 → builtin 유지 | **PASS** |

**테스트 파일:** [tests/test_converter.cpp](../tests/test_converter.cpp), [tests/test_config.cpp](../tests/test_config.cpp)

---

## 5. 전체 테스트 (`ctest`)

### 5.1 실행 환경

```powershell
cmake -S . -B build -DUC11_GREEN_PHASE=ON
cmake --build build
ctest --test-dir build -V
```

### 5.2 결과 (2026-05-21)

```text
100% tests passed, 0 tests failed out of 71
Total Test time (real) ≈ 2.2 sec
```

| 타깃 | 테스트 수 | 결과 |
|------|-----------|------|
| `unit_converter_tests` | 36 | **36 PASS** |
| `unit_converter_red_tests` | 35 | **35 PASS** |
| **합계** | **71** | **71 PASS** |

> RED 단계 48건 대비 +23건: TC 전용 파일·커버리지 보강·RED 스켈레톤 실 assertion 분리 등으로 Catch2 discover 수 증가.

### 5.3 RED 스켈레톤 처리

[tests/red_track_a_boundary.cpp](../tests/red_track_a_boundary.cpp), [tests/red_track_b_domain.cpp](../tests/red_track_b_domain.cpp)의 `FAIL("RED")`를 **실제 Given/When/Then assertion**으로 교체하여 RED-ID와 TC 계약을 동시에 만족한다.

---

## 6. 커버리지 (gcov / lcov)

### 6.1 빌드·측정

```powershell
cmake -S . -B build -DUC11_COVERAGE=ON -DUC11_GREEN_PHASE=ON
cmake --build build
ctest --test-dir build

lcov --capture --directory build --output-file build/coverage.info
lcov --remove build/coverage.info "*/catch2/*" "*/tests/*" "*/build/_deps/*" "/usr/*" `
     --output-file build/coverage.filtered.info
lcov --extract build/coverage.filtered.info "*/src/entity/*" --output-file build/entity.info
lcov --extract build/coverage.filtered.info "*/src/boundary/*" --output-file build/boundary.info
lcov --summary build/entity.info
lcov --summary build/boundary.info
```

CMake: `UC11_COVERAGE` + `uc11_enable_coverage()` per target ([CMakeLists.txt](../CMakeLists.txt)).

### 6.2 레이어별 결과

| 레이어 | 파일 | 라인 커버리지 | 함수 | 게이트 |
|--------|------|---------------|------|--------|
| **Entity** | `UnitConverter.cpp` | 100% (12/12) | 100% | ≥ 95% ✓ |
| **Entity** | `UnitRegistry.cpp` | 93.8% (30/32) | 100% | (합산) |
| **Entity 합계** | `src/entity/` | **95.5%** (42/44) | 100% | **≥ 95% ✓** |
| **Boundary** | `InputParser.cpp` | 90.0% (18/20) | 100% | |
| **Boundary** | `InputValidator.cpp` | 100% (9/9) | 100% | |
| **Boundary** | `OutputFormatter.cpp` | 100% (22/22) | 100% | |
| **Boundary** | `ConfigLoader.cpp` | 79.5% (70/88) | 100% | |
| **Boundary 합계** | `src/boundary/` | **87.1%** (121/139) | 100% | **≥ 85% ✓** |

**잔여 Gap (REFACTOR 후보):** `ConfigLoader.cpp` YAML/JSON 오류 분기 일부, `UnitRegistry` 검증 예외 경로 소수.

### 6.3 비율 상수·인라인 검증

| 위치 | `3.28084` / `1.09361` 인라인 |
|------|------------------------------|
| `src/entity/ConversionConstants.hpp` | **허용** (`kMeterToFeet`, `kMeterToYard`) |
| `src/entity/*.cpp`, `src/boundary/*.cpp` | **없음** |
| `UnitConverter.cpp` (legacy) | **없음** (제거됨) |
| `tests/*.cpp` | golden literal 또는 `kMeterToFeet` 참조 |

---

## 7. 아키텍처·코드 변경 요약

### 7.1 레이어 구조 (GREEN 완료 시점)

```text
unit_converter_legacy (UnitConverter.cpp)
  └── uc11_boundary
        ├── InputParser      — unit:value, FORMAT/NUMBER 예외
        ├── InputValidator   — POL-NEG, NaN/Inf
        ├── OutputFormatter  — table, JSON (half-up 4)
        └── ConfigLoader     — JSON/YAML, missing → builtins
  └── uc11_entity
        ├── UnitRegistry     — registerUnit, factorToMeter
        └── UnitConverter    — convert (hub), convertAll
```

### 7.2 legacy `main()` (변환 로직 분리)

```cpp
// UnitConverter.cpp — 변환식·비율 없음
parseInputLine → validateInput → UnitRegistry::withBuiltins()
  → convertAll → formatTableLine (per row)
```

### 7.3 핵심 Domain 환산 (허브)

```cpp
const double meters = value * registry.factorToMeter(fromUnit);
return meters / registry.factorToMeter(toUnit);
```

### 7.4 신규·주요 API

| API | 모듈 | TC/RED |
|-----|------|--------|
| `parseInputLine` | Boundary | A-01,02,05 |
| `validateInput` | Boundary | A-03 |
| `formatTableLine` | Boundary | A-06 |
| `formatJson` | Boundary | A-06 / RED-A-06 |
| `convert` / `convertAll` | Entity | B-01~05 |
| `loadConfig` | Boundary | B-06,07 |

---

## 8. 결함 목록 갱신 (요지)

| ID | RED 시 상태 | GREEN 후 |
|----|-------------|----------|
| DEF-001 | meter→feet 0.0 | **Fixed** (허브·분기) |
| DEF-002 | API 부재 | **Fixed** (Entity/CMake) |
| DEF-003 | 음수 미검증 (legacy) | **Fixed** (Boundary; legacy 위임) |
| DEF-004 | half-up 미적용 (legacy) | **Fixed** (formatTableLine; legacy 위임) |
| DEF-005 | feet 역변환 누락 | **Fixed** |
| DEF-009 | FORMAT 예외 | **Fixed** (InputParser) |

상세: [docs/defect_list.md](../docs/defect_list.md)

---

## 9. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| Entity 라이브러리 | [src/entity/](../src/entity/) | 완료 |
| Boundary 라이브러리 | [src/boundary/](../src/boundary/) | 완료 |
| TC 테스트 (A/B) | [tests/test_*.cpp](../tests/) | 완료 |
| RED→GREEN assertion | [tests/red_track_*.cpp](../tests/) | 완료 |
| 비율 상수 | [src/entity/ConversionConstants.hpp](../src/entity/ConversionConstants.hpp) | 완료 |
| 커버리지 옵션 | [CMakeLists.txt](../CMakeLists.txt) `UC11_COVERAGE` | 완료 |
| PR 브랜치 | `origin/A_01` | push 완료 |
| **본 보고서** | [Report/03_GREEN.md](03_GREEN.md) | 완료 |

---

## 10. PR 및 머지 (green → A_01)

| 항목 | 내용 |
|------|------|
| **브랜치** | `A_01` |
| **Base** | `main` |
| **PR 생성** | https://github.com/wanedu88/UnitConverter_11/pull/new/A_01 |
| **제목 권장** | `feat(green): Dual-Track TDD GREEN complete (TC-A/B)` |

### 리뷰 체크리스트

- [ ] 14 TC 태그 필터 실행 PASS
- [ ] `ctest` 71/71 PASS
- [ ] lcov Entity ≥ 95%, Boundary ≥ 85%
- [ ] `src/`·legacy에 비율 인라인 없음
- [ ] `UnitConverter.cpp`에 환산 if-else 없음
- [ ] REG-04: `meter:2.5` → feet golden 유지

### 머지 후 권장

1. `main`에 머지 후 `green` 브랜치 동기화  
2. **REFACTOR** PR 별도 (REG-05: 동작 불변, 테스트 GREEN 유지)  
3. [Report/04_REFACTOR.md](04_REFACTOR.md) (예정) — Control 레이어·CLI E2E

---

## 11. REFACTOR 진입 조건 (다음 단계)

| 순서 | 작업 | 완료 기준 |
|------|------|-----------|
| 1 | `A_01` PR 리뷰·머지 | `main` GREEN 반영 |
| 2 | Control/UseCase 추출 (선택) | `main()` 추가 축소 |
| 3 | `ConfigLoader` 오류 분기 정리 | Boundary lcov 90%+ |
| 4 | CI `UC11_COVERAGE` gate 스크립트 | PR 실패 시 자동 차단 |
| 5 | README RED To-Do 체크박스 갱신 | 14 TC `[x]` |

**금지:** 테스트 기대값 완화·`FAIL` 제거만으로 통과 · REFACTOR 중 TC 회귀 무시.

---

## 12. 검증 체크리스트 (QA 서명용)

| # | 항목 | 기대 | 확인 |
|---|------|------|------|
| 1 | TC-A-01~07 | 7 PASS | ☐ |
| 2 | TC-B-01~07 | 7 PASS | ☐ |
| 3 | `ctest` | 71/71 PASS | ☐ |
| 4 | Entity lcov | ≥ 95% | ☐ |
| 5 | Boundary lcov | ≥ 85% | ☐ |
| 6 | 비율 인라인 없음 (`src/`, legacy) | 상수만 | ☐ |
| 7 | legacy `main()` 위임 | 환산식 없음 | ☐ |
| 8 | PR `A_01` 리뷰 | 승인 | ☐ |

---

## 13. 결론

- **Dual-Track TDD GREEN**을 커밋 순서표(9+1)에 따라 완료했으며, **TC 14건·ctest 71건·커버리지 게이트**를 충족했다.
- 변환 비즈니스 로직은 **Entity 허브**로 일원화했고, legacy CLI는 **Boundary API 위임**만 수행한다.
- 비율 **3.28084 / 1.09361**은 `ConversionConstants.hpp`에만 정의한다.
- **REFACTOR** 및 CI 게이트 자동화는 `A_01` 머지 이후 단계에서 진행한다.

---

*본 문서는 GREEN 단계 인수·커버리지·PR 준비를 기술한다. RED 배경은 [02_RED.md](02_RED.md)를 참조한다.*
