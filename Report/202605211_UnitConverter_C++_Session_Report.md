# UnitConverter_11 — 통합 세션 보고서 (RED → GREEN → Golden Master → REFACTOR)

| 항목 | 내용 |
|------|------|
| **프로젝트** | `c:\DEV\UnitConverter_11` — Dual-Track UI + Logic TDD (C++17) |
| **파일명** | `Report/202605211_UnitConverter_C++_Session_Report.md` |
| **작성일** | 2026-05-21 |
| **현재 브랜치** | `refactoring` |
| **작업자** | wanedu88 \<wanedu88@gmail.com\> |
| **선행 보고서** | [01_spec.md](01_spec.md), [02_RED.md](02_RED.md), [03_GREEN.md](03_GREEN.md), [04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md), [05_REFACTOR.md](05_REFACTOR.md) |
| **관련 문서** | [docs/PRD.md](../docs/PRD.md), [docs/TODO.md](../docs/TODO.md), [docs/test_plan.md](../docs/test_plan.md), [docs/defect_list.md](../docs/defect_list.md) |

---

## 1. 작업 개요 (브랜치 · 날짜 · 작업자)

| 항목 | 내용 |
|------|------|
| **작업자** | wanedu88 |
| **작업 기간** | 2026-05-20 ~ 2026-05-21 |
| **TDD 단계** | RED → GREEN → Golden Master(REG-04) → REFACTOR(REG-05) → 커버리지 점검 |
| **브랜치 이력** | `spec` → `red` → `green` (PR #3 머지) → `refactoring` (현재) |
| **최신 커밋** | `30fac3f` — `docs(refactor): add REFACTOR report and session notes (05)` |
| **ctest (REFACTOR 후)** | **76/76 PASS** (`-DUC11_GREEN_PHASE=ON`) |
| **불변 비율** | 1 m = **3.28084** ft, 1 m = **1.09361** yd (meter 허브만) |

**세션 목표:** baseline 단일 `UnitConverter.cpp`에서 BCE 구조·계약 테스트·커버리지 게이트·Golden Master 회귀까지 일괄 달성.

---

## 2. 완료된 To-Do 항목 요약 (Phase 6 / v1.0 Must-Have 기준)

> **Phase 6** = PRD Phase 5 이후 **구현·TDD 사이클** 단계. 판정 기준: [docs/TODO.md](../docs/TODO.md) Must-Have + [README.md](../README.md) RED/GREEN 체크리스트.

### 2.1 필수 (Must-Have) — 완료·부분 완료

| 상태 | 작업 (docs/TODO.md) | 완료 근거 |
|------|---------------------|-----------|
| ✅ | 프로젝트 골격·CMake·Catch2 | `src/entity`, `control`, `boundary`, `data`, `tests/`; Ninja 빌드·`ctest` 76 PASS |
| ✅ | Entity — Registry·Converter·허브 환산 | TC-B-01~07, `convert`/`convertAll` GREEN |
| ✅ | 입력 파싱·검증 (F-01) | TC-A-01~05, `parseInputLine` |
| ✅ | 음수 거부·0 허용 (POL-NEG) | TC-A-03, TC-A-07, `validateInput` |
| ✅ | 미지원 단위 | TC-A-04, `factorToMeter` → `Unknown unit` |
| ✅ | table 출력 (POL-OUT) | TC-A-06, `formatTableLine` 4자리 half-up |
| ✅ | Control — ConvertLength | `ConvertLengthUseCase`, TC-C-01 |
| ⚠️ | ERR-* 계약 8종 golden | 메시지·예외 타입 단위 테스트 다수; **stderr 8종 스냅샷 diff 0 미완** |
| ⚠️ | Gherkin GH-01~08 매핑 표 | 테스트 ID는 존재; **Traceability 표 제출·강사 서명 미완** |
| ✅ | BCE 의존 방향 | entity에 iostream 없음; boundary→control→entity |
| ✅ | Entity 커버리지 ≥95% | lcov **96.1%** (49/51) |
| ✅ | Boundary 커버리지 ≥85% | lcov **86.8%** (66/76, `src/boundary/`) |
| ⚠️ | 인수 AC-01~04, AC-08 | 테스트 GREEN; **강사 인수 체크박스 미완** |

### 2.2 README TDD 체크리스트 (RED 단계 To-Do)

| Track | 항목 | 상태 |
|-------|------|------|
| A | TC-A-01 ~ TC-A-07 | ✅ GREEN PASS (구현·테스트 완료) |
| B | TC-B-01 ~ TC-B-07 | ✅ GREEN PASS |
| 커버리지 | Domain ≥95%, Boundary ≥85% | ✅ (REFACTOR 후 재측정) |
| 결함 | defect_list.md 생성 | ✅ |
| legacy | DEF-003·004·010·013 Open | ⚠️ Boundary/Entity 위임으로 **동작은 수정**; README 체크박스·일부 E2E 미정리 |

### 2.3 Golden Master 체크리스트

| ID | 항목 | 상태 |
|----|------|------|
| GM-01~07, GM-09 | 기준 파일·테스트·CI workflow | ✅ |
| GM-08 | GitHub required status check | ☐ 수동 설정 대기 |

### 2.4 권장 (Should-Have) — 부분

| 상태 | 항목 |
|------|------|
| ✅ | JSON 출력 (`formatJson`, TC/RED-A-06) |
| ✅ | config JSON/YAML 로드 (TC-B-06, TC-C-02) |
| ✅ | 동적 등록 cubit (TC-B-05, TC-C-03) |
| ☐ | CSV 출력, table/json/csv parity |
| ☐ | Data 레이어 lcov ≥90% (ConfigLoader 81.8%) |
| ☐ | 인수 AC-05~07 |

---

## 3. RED 단계 결과

### 3.1 작성한 테스트 목록

**Dual-Track RED 스켈레톤 (12건)** — `FAIL("RED")` only

| ID | Catch2 `TEST_CASE` | 파일 |
|----|-------------------|------|
| RED-A-01 | `test_parse_meter_25_returns_conversion_result` | `tests/red_track_a_boundary.cpp` |
| RED-A-02 | `test_parse_no_colon_throws_invalid_argument` | 동일 |
| RED-A-03 | `test_validate_meter_neg10_throws_invalid_argument` | 동일 |
| RED-A-04 | `test_convert_parsec10_throws_unknown_unit` | 동일 |
| RED-A-05 | `test_format_table_preserves_input_meter_25` | 동일 |
| RED-A-06 | `test_format_json_meter_25_valid_schema` | 동일 |
| RED-B-01 | `test_convert_meter_to_feet_within_1e5` | `tests/red_track_b_domain.cpp` |
| RED-B-02 | `test_convert_meter_to_yard_ratio_109361` | 동일 |
| RED-B-03 | `test_convert_all_meter10_returns_three_units` | 동일 |
| RED-B-04 | `test_register_cubit_then_convert_to_meter` | 동일 |
| RED-B-05 | `test_load_config_json_applies_feet_factor` | 동일 |
| RED-B-06 | `test_load_config_missing_keeps_builtin_ratios` | 동일 |

**GREEN 파일 RED 가드 (36건)** — `uc11_require_green_phase()` → `FAIL("RED")` when `UC11_GREEN_PHASE=OFF`

- `tests/test_parser.cpp`, `test_boundary.cpp`, `test_converter.cpp`, `test_config.cpp` 등 (`unit_converter_tests`)

### 3.2 실패 확인 여부

| 빌드 옵션 | ctest 결과 | 판정 |
|-----------|------------|------|
| 기본 (`UC11_GREEN_PHASE=OFF`) | **48/48 FAIL** | ✅ RED 정합 (조치 커밋 `d513468`) |
| 초기 이슈 | 36 PASS / 12 FAIL | ❌ RED 원칙 위반 → 가드 강화 후 수정 |

**대표 baseline 실패 시나리오 (분석용):**

| 입력 | 기대 | baseline 실제 | 결함 ID |
|------|------|---------------|---------|
| `meter:2.5` | feet ≈ 8.202100 | 0.0 (분기 누락) | DEF-001 |
| `meter:-1.0` | 예외 | 음수 출력 | DEF-003 |
| `meter2.5` | FORMAT 예외 | 미정의 | DEF-009 |
| `parsec:1.0` | Unknown unit | — | — |

**RED 핵심 커밋:** `9becfe0` feat(red): Dual-Track stack · `d513468` fix(red): 48/48 FAIL guard

---

## 4. GREEN 단계 결과

### 4.1 통과한 테스트

| 구분 | 수량 | 결과 |
|------|------|------|
| TC-A-01 ~ TC-A-07 | 7 | PASS |
| TC-B-01 ~ TC-B-07 | 7 | PASS |
| `unit_converter_tests` (확장 포함) | 36+ | PASS |
| `unit_converter_red_tests` (assertion 교체 후) | 35 | PASS |
| **GREEN 시점 ctest 합계** | **71/71** | PASS |

**TC-C (REFACTOR 중 추가, GREEN 범위 외):** TC-C-01~03 — Control UseCase 보호 테스트 (76/76에 포함)

### 4.2 GREEN 커밋 메시지 (순서)

| # | 커밋 메시지 | 해시 (요지) |
|---|-------------|-------------|
| 1 | `feat(green): meter to feet` | `a09a333` |
| 2 | `feat(green): validate missing colon` | `b477af1` |
| 3 | `feat(green): meter to yard` | `d6c163c` |
| 4 | `feat(green): validate negative value` | `ad54624` |
| 5 | `feat(green): feet to meter reverse` | `7a917e4` |
| 6 | `feat(green): validate unknown unit` | `8be6f20` |
| 7 | `feat(green): convertAll and registerUnit` | `dead2f3` |
| 8 | `feat(green): boundary happy path` | `6e5fc88` |
| 9 | `feat(green): loadConfig with fallback` | `71dbc01` |
| 10 | `chore(green): complete TC suite, coverage gates, and legacy domain delegation` | `aeaf7c0` |
| — | `docs: add GREEN phase report and session notes (03_GREEN)` | `4c762ce` |
| — | Merge PR #3 `green` → base `C_11` | `7419408` |

**GREEN 시점 커버리지:** Entity **95.5%**, Boundary **87.1%** (ConfigLoader가 boundary 경로에 포함된 측정)

---

## 5. Refactoring 결과

### 5.1 선택 항목 (REFACTOR ID)

| ID | 내용 | 커밋 |
|----|------|------|
| R-C1 | `ConvertLengthUseCase` 추출 | `e6c7e6c` |
| R-B1 | `CliApp` 추출, `main()` 1줄 위임 | `ac6b1d5` |
| R-U2 | `ErrMsg` / `DomainErr` / `ConfigErr` 상수화 | `2f7e4b7` |
| R-C2, R-L1 | `LoadConfigUseCase`, `RegisterUnitUseCase`, `src/data/` | `91279ab` |

**Golden Master (REFACTOR 직전):** `2de1fcf` feat(golden): REG-04 E2E approval

### 5.2 변경 파일 (요지)

| 레이어 | 신규·이동 |
|--------|-----------|
| `src/control/` | `ConvertLengthUseCase`, `LoadConfigUseCase`, `RegisterUnitUseCase` |
| `src/data/` | `ConfigLoader.cpp` (boundary에서 분리) |
| `src/boundary/` | `CliApp.cpp`, `InputParser`, `InputValidator`, `OutputFormatter` |
| `src/entity/` | `UnitRegistry`, `UnitConverter`, `ConversionConstants`, `ConversionRule` |
| `UnitConverter.cpp` | `return uc11::CliApp{}.run();` only |
| `tests/` | `test_control_*.cpp`, `test_golden_master.cpp`, fixtures |

### 5.3 회귀 테스트 통과 여부

| 검증 | 결과 |
|------|------|
| ctest 전체 | **76/76 PASS** |
| TC-A-01~07, TC-B-01~07 | PASS |
| TC-C-01~03 | PASS |
| `ctest -R GoldenMaster` (REG-04/05) | PASS |
| Golden Master stdout diff | **0** (4 시나리오) |
| 비율·메시지 문자열 | 변경 없음 (상수화만) |

---

## 6. 커버리지 현황 (레이어별 수치)

**측정일:** 2026-05-21 · **빌드:** `-DUC11_COVERAGE=ON -DUC11_GREEN_PHASE=ON` · **실행:** `ctest` 76/76 후 `lcov`

### 6.1 레이어 합계

| 레이어 | Stmts (lines) | Hit | Miss | Cover | 게이트 | 판정 |
|--------|---------------|-----|------|-------|--------|------|
| **Entity** (`src/entity/`) | 51 | 49 | 2 | **96.1%** | ≥ 95% | ✅ |
| **Boundary** (`src/boundary/`) | 76 | 66 | 10 | **86.8%** | ≥ 85% | ✅ |
| **legacy** `UnitConverter.cpp` | 2 | 2 | 0 | **100%** | — | 위임만 |
| **Data** `ConfigLoader.cpp` | 88 | 72 | 16 | **81.8%** | ≥ 90% (권장) | ⚠️ |
| **Control** (3 UseCase) | — | — | — | 미집계 | 100% (PRD) | 후속 |

### 6.2 파일별 (Invariant·미달 분석)

| 파일 | Stmts | Miss | Cover | 미달·미커버 요지 |
|------|------:|-----:|------:|------------------|
| `UnitConverter.cpp` (entity) | 12 | 0 | 100% | meter↔feet/yard 허브 환산 전 경로 실행 |
| `UnitRegistry.cpp` | 35 | 2 | 94.3% | L18·L56 닫는 괄호 (비로직) |
| `DomainErrors.hpp` | 4 | 0 | 100% | duplicate/unknown 메시지 |
| `InputValidator.cpp` | 9 | 0 | 100% | 음수·NaN 분기 |
| `InputParser.cpp` | 20 | 2 | 90.0% | `stod` **out_of_range** (L33–34) |
| `OutputFormatter.cpp` | 22 | 0 | 100% | POL-OUT |
| `CliApp.cpp` | 21 | 8 | 61.9% | stdin EOF, catch 경로 (단위 테스트는 하위 모듈 직접 호출) |
| legacy `UnitConverter.cpp` | 2 | 0 | 100% | `main()` → `CliApp` |

**Invariant 분기 점검:**

| Invariant | 커버 | 비고 |
|-----------|------|------|
| meter↔feet/yard 비율 | ✅ | if-else 단위 분기 없음; TC-B-01~03 등 |
| 음수 입력 | ✅ | TC-A-03, `validateInput` L15–18 |
| 없는 단위 | ✅ | TC-A-04, `factorToMeter` L43–44 |

**gcov (legacy):** `UnitConverter.cpp` Lines executed **100% of 2** (환산 로직은 `src/entity/UnitConverter.cpp`에서 측정).

---

## 7. 미완료 항목 및 다음 단계 제안

### 7.1 미완료 (우선순위)

| 우선순위 | 항목 | 제안 |
|----------|------|------|
| P1 | **GM-08** Branch protection + `Golden Master (REG-04)` required check | GitHub Settings 수동 설정 |
| P1 | **ERR-* 8종 stderr golden** (REG-02) | Catch2 스냅샷 또는 approval 파일 8종 |
| P2 | **CliApp** catch·stdin EOF 커버리지 | 파이프/stdin mock 통합 테스트 |
| P2 | **InputParser** `out_of_range` | `meter:1e400` 계약 테스트 |
| P2 | **Data lcov ≥90%** | ConfigLoader 오류 fixture 보강 |
| P3 | **Control lcov 100%** | 전용 `[control]` 스위트 |
| P3 | CSV 출력·AC-05~07 | Should-Have 마일스톤 M5 |
| P3 | DEF-013 단위명 trim/lower | PRD 정책 확정 후 별도 PR |
| P3 | CI lcov gate 스크립트 | `test_plan.md` §8.6 자동화 |

### 7.2 다음 단계 (권장 순서)

1. `refactoring` → `C_11` / `main` PR 생성 · GM-08 설정  
2. REG-02 ERR stderr golden 8종 추가  
3. Boundary 여유 확보(선택): CliApp E2E + Parser overflow  
4. v1.0 태그 전: Traceability 표 · 강사 인수 AC 서명  
5. M5: CSV·config 실패 E2E·Data 커버리지 90%+

---

## 8. 발견된 이슈 및 해결 방법

| # | 이슈 | 원인 | 해결 |
|---|------|------|------|
| 1 | RED 단계인데 ctest 36 PASS | `unit_converter_tests`에 조기 구현 연결 | `UC11_GREEN_PHASE` 가드 + `TestPhase.hpp`; `d513468` |
| 2 | GREEN PR base 브랜치 혼동 | `A_01` vs `C_11` | 문서·PR target `C_11`으로 정정 (`cd2dd2e`) |
| 3 | REFACTOR 후 Boundary lcov 84%대 | stale `.gcno` (ConfigLoader 경로 변경) | clean rebuild 후 재측정 → **86.8%** |
| 4 | Windows `gcov` 실패 | `.gcno` basename 불일치 | **lcov DA 라인**으로 미커버 분석 (세션 점검) |
| 5 | legacy README Open 결함 | 체크리스트 미갱신 | 동작은 Boundary/Entity 위임으로 수정; 문서 동기화 필요 |

**결함 목록:** [docs/defect_list.md](../docs/defect_list.md) — DEF-001~009 등 GREEN에서 Fixed; DEF-013 Open.

---

## 9. 생성형 AI 활용 회고

### 9.1 도움이 된 순간

- **Dual-Track RED 명세 → Catch2 스켈레톤 일괄 생성:** RED-A/B 12건·TC 14건을 Given/When/Then 표로 고정한 뒤 파일 단위로 분리해 RED 48/48 FAIL 재현이 빨랐다.
- **커버리지 게이트 해석:** lcov `entity.info` / `boundary.info`의 `DA:*,0` 라인으로 Invariant(비율·음수·unknown) vs 구조 미스(괄호)·CliApp E2E 미스를 구분할 수 있었다.
- **REFACTOR 커밋 단위 제안:** R-C1→R-B1→R-U2→R-C2 순서로 Golden Master 깨지 않게 최소 diff 유지.
- **Golden Master approve 패턴:** 기준 없을 때 생성·있을 때 diff — REFACTOR 중 stdout drift 방지에 효과적.

### 9.2 한계

- **Windows gcov CLI:** MinGW 산출물 경로에서 `gcov` 직접 호출이 불안정해, 로컬 상세 라인 분석은 lcov·기존 `.gcov` 산출물에 의존했다.
- **초기 RED 판정 오판:** 구현이 일부 선행된 상태에서 “RED 완료”로 오인하기 쉬움 — **반드시 `UC11_GREEN_PHASE=OFF` + 48/48 FAIL**로 재검증 필요.
- **Phase 6 TODO와 README 체크박스 불일치:** 코드는 GREEN인데 README legacy Open 항목이 남아 있어, AI·학습자 모두 “미완”으로 보일 수 있음.

### 9.3 TC 작성 팁 (재사용)

1. **Invariant 먼저:** `3.28084` / `1.09361` / POL-NEG / `Unknown unit` 문자열을 TC 제목·태그(`[TC-B-01]`)에 박아 두면 REFACTOR 후에도 추적이 쉽다.
2. **RED는 한 스위치:** `uc11_require_green_phase()` 한 곳으로 36건을 RED에 묶고, `red_track_*.cpp`는 12건만 `FAIL("RED")`로 유지한다.
3. **Boundary vs Domain 파일 분리:** 파싱·stderr는 `test_parser` / `test_boundary`; `convert` ε는 `test_converter` — AI가 잘못된 레이어에 assert 넣는 실수를 줄인다.
4. **커버리지 보강은 계약 테스트로:** 구현 추가 없이 `parseInputLine("meter:1e400")`, stdin EOF 파이프만으로 Parser·CliApp miss를 줄일 수 있다.
5. **Golden Master는 REFACTOR 직전:** table 포맷·반올림이 안정된 GREEN 직후에 고정해야 diff 노이즈가 적다.

---

## 부록 A — 상세 보고서 링크

| 단계 | 보고서 |
|------|--------|
| Spec | [01_spec.md](01_spec.md) |
| RED | [02_RED.md](02_RED.md) |
| GREEN | [03_GREEN.md](03_GREEN.md) |
| Golden Master | [04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md) |
| REFACTOR | [05_REFACTOR.md](05_REFACTOR.md) |

## 부록 B — 검증 명령 (재현)

```powershell
cmake -S . -B build -DUC11_COVERAGE=ON -DUC11_GREEN_PHASE=ON
cmake --build build
ctest --test-dir build --output-on-failure

lcov --capture --directory build --output-file build/coverage.info
lcov --remove build/coverage.info "*/catch2/*" "*/tests/*" "*/build/_deps/*" "/usr/*" "*/mingw64/*" `
     --output-file build/coverage.filtered.info
lcov --extract build/coverage.filtered.info "*/src/entity/*" --output-file build/entity.info
lcov --extract build/coverage.filtered.info "*/src/boundary/*" --output-file build/boundary.info
lcov --summary build/entity.info
lcov --summary build/boundary.info
```

---

*본 문서는 2026-05-21 세션의 RED·GREEN·Golden Master·REFACTOR·커버리지 점검을 단일 보고서로 통합한 것이다. 단계별 상세는 부록 A 링크를 참조한다.*
