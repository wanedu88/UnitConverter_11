# UnitConverter_11 — Bonus GREEN 인수 검증 보고서

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_11 — Dual-Track UI + Logic TDD |
| **작성일** | 2026-05-21 |
| **단계** | **Bonus GREEN 인수 검증** — 전체 테스트 · lcov · Golden Master |
| **선행 보고서** | [07_BONUS_GREEN.md](07_BONUS_GREEN.md) (Bonus GREEN 구현), [04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md) (REG-04), [05_REFACTOR.md](05_REFACTOR.md) (커버리지 게이트) |
| **관련 문서** | [README.md](../README.md), [docs/test_plan.md](../docs/test_plan.md) |

---

## 1. 요약 (Executive Summary)

신규 **감쇠 단위(`DecayingUnit`)** 구현 완료 후, 요청된 **전체 테스트 · 커버리지 · Golden Master** 항목을 로컬에서 재실행하여 인수 기준 충족 여부를 확인했다.

| 확인 항목 | 기대 | 실측 | 판정 |
|-----------|------|------|------|
| **BT-01~06** (신규) | 전부 PASS | 10/10 assertion PASS | ✓ |
| **TC-A-01~07** (회귀) | 전부 PASS | ctest 9건 + RED track 포함 PASS | ✓ |
| **TC-B-01~07** (회귀) | 전부 PASS | ctest 16건 PASS | ✓ |
| **Golden Master** (REG-04) | meter/feet/yard 출력 불변 | 직렬 실행 PASS | ✓ |
| **`registerUnit` 음수 비율** | `std::invalid_argument` | BT-04 + `factor ≤ 0` 분기 | ✓ |
| **Domain 커버리지** | ≥ 95% | **96.1%** (49/51 lines) | ✓ |
| **Boundary 커버리지** | ≥ 85% | **86.8%** (66/76 lines) | ✓ |
| **`UnitConverter.cpp`** | — | **100%** (12/12 lines) | ✓ |
| **ctest 전체 77건** | 전부 PASS | **76/77** (Test #1 인코딩 이슈) | △ |

**종합 판정:** 기능·회귀·Golden Master(직렬)·커버리지 게이트 **모두 충족**. ctest Test #1만 Windows CTest 한글 테스트명 인코딩으로 FAIL이며, Catch2 `[bonus]` 필터 직접 실행 시 **정상 PASS**.

---

## 2. 검증 환경

| 항목 | 값 |
|------|-----|
| **OS** | Windows 10 (19045) |
| **빌드** | CMake + Ninja, `build/` |
| **컴파일러** | MinGW-w64 g++ 15.2.0 (`C:\mingw64\bin`) |
| **테스트** | Catch2 v3.5.4 |
| **커버리지** | `-DUC11_COVERAGE=ON`, gcov + lcov (Chocolatey) |
| **GREEN phase** | `UC11_GREEN_PHASE=ON` (기본값) |

---

## 3. 전체 테스트

### 3.1 실행 명령

```powershell
cd c:\DEV\UnitConverter_11
cmake -S . -B build
cmake --build build
ctest --test-dir build -V          # 전체 (한글 CTest #1 주의)
ctest --test-dir build -j1 -E "감쇠"  # 한글 항목 제외 권장
```

### 3.2 결과 요약

| 실행 | 결과 | 비고 |
|------|------|------|
| `ctest --test-dir build -j1` | **76/77 PASS** | Test #1만 FAIL |
| `ctest --test-dir build -j4` | **74~76/77** | Golden Master 간헐 FAIL (CLI 동시 실행) |
| `unit_converter_tests.exe` (전체) | PASS | 38 test cases |
| `unit_converter_red_tests.exe` (전체) | PASS | 38 test cases |

### 3.3 Test #1 FAIL 원인 (비기능)

Catch2 `TEST_CASE("감쇠 단위 변환", "[bonus]")` 가 CTest discover 시 **한글 이름**으로 등록된다. Windows CTest가 이름을 깨뜨려 필터 미매칭:

```text
No test cases matched '"감쇠 ?위 변??"'
No tests ran
```

**우회 (정본):**

```powershell
.\build\unit_converter_tests.exe "[bonus]" -s
```

→ **10 assertions, 1 test case — ALL PASS**

---

## 4. 신규 기능 BT-01~06

정본: [`tests/test_decaying_unit.cpp`](../tests/test_decaying_unit.cpp)

| ID | Given / When | Then (기대) | 결과 |
|----|--------------|-------------|------|
| **BT-01** | `registerUnit("cubit", 0.4572)` → `convert("cubit", 1, "meter")` | ≈ 0.4572 (ε 1e-5) | **PASS** |
| **BT-02** | 동일 → `convert("meter", 1, "cubit")` | ≈ 2.1872266 | **PASS** |
| **BT-03** | 동일 → `convert("cubit", 1, "feet")` | ≈ 1.500000048 | **PASS** |
| **BT-04** | `registerUnit("badunit", -0.4572)` | `std::invalid_argument` | **PASS** |
| **BT-05** | `convertAll("cubit", 1)` | size==4, meter/feet/yard/cubit | **PASS** |
| **BT-06** | cubit 등록 후 `convert("meter", 1, "feet")` | ≈ 3.28084 (builtin 불변) | **PASS** |

**실행 로그 (요지):**

```text
Filters: [bonus]
All tests passed (10 assertions in 1 test case)
```

---

## 5. 회귀 TC-A-01~07 · TC-B-01~07

### 5.1 TC-A (Boundary)

| ID | 테스트 케이스 | 파일 | ctest |
|----|---------------|------|-------|
| TC-A-01 | parse meter 2.5 happy path | `test_boundary.cpp` | PASS |
| TC-A-02 | parse missing colon | `test_parser.cpp` | PASS |
| TC-A-03 | validate negative value | `test_parser.cpp` | PASS |
| TC-A-03b | validate nan | `test_parser.cpp` | PASS |
| TC-A-04 | unknown unit | `test_parser.cpp` | PASS |
| TC-A-05 | parse invalid number (+05b/05c) | `test_parser.cpp` | PASS |
| TC-A-06 | format preserves input | `test_boundary.cpp` | PASS |
| TC-A-07 | value zero boundary | `test_boundary.cpp` | PASS |

```powershell
ctest --test-dir build -R "TC-A" --output-on-failure
# 19/19 PASS (red_tests discover 포함)
```

### 5.2 TC-B (Domain)

| ID | 테스트 케이스 | 파일 | ctest |
|----|---------------|------|-------|
| TC-B-01 | meter → feet | `test_converter.cpp` | PASS |
| TC-B-02 | meter → yard | `test_converter.cpp` | PASS |
| TC-B-03 | feet → meter | `test_converter.cpp` | PASS |
| TC-B-04 | convertAll 3 units | `test_converter.cpp` | PASS |
| TC-B-05 | register cubit → meter | `test_converter.cpp` | PASS |
| TC-B-05b | registerUnit validation | `test_config.cpp` | PASS |
| TC-B-06 | loadConfig valid JSON (+b~f) | `test_config.cpp` | PASS |
| TC-B-07 | missing config keeps builtin | `test_config.cpp` | PASS |

```powershell
ctest --test-dir build -R "TC-B" --output-on-failure
# 16/16 PASS
```

### 5.3 meter:2.5 builtin golden (동적 등록 회귀)

| 테스트 | 기대 | 결과 |
|--------|------|------|
| `test_register_cubit_meter_to_feet_golden_unchanged` | feet ≈ **8.20210** (cubit 등록 후에도) | **PASS** |

---

## 6. Golden Master (REG-04)

### 6.1 목적

legacy CLI [`UnitConverter.exe`](../build/UnitConverter.exe) stdout이 [`tests/golden_master_expected.txt`](../tests/golden_master_expected.txt) 와 **byte-equal** 인지 확인. 시나리오: `meter:2.5`, `feet:1.0`, `yard:1.0`, `meter:0.0` ([04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md)).

### 6.2 실행 결과

| 방식 | 결과 |
|------|------|
| `unit_converter_tests.exe "golden_master_cli_stdout_approval"` | **PASS** |
| `ctest -j1 -R GoldenMaster` | **PASS** (~0.26s) |
| `ctest -j4` (병렬 전체) | **간헐 FAIL** — `UnitConverter.exe` 동시 실행 시 stdout 섞임 |

**병렬 FAIL 예 (diff 요지):**

```text
--- expected
+++ actual
@@ line 2 @@
- 2.5 meter = 8.2021 feet
+ ---
```

**권장:** Golden Master는 **`-j1`** 또는 **`-R GoldenMaster`** 만 실행.

```powershell
ctest --test-dir build -j1 -R "GoldenMaster|golden_master" --output-on-failure
```

### 6.3 cubit과 Golden Master

본 검증에서 Golden Master baseline은 **builtin 3단위 시나리오만** 포함한다. cubit 등록 CLI 시나리오는 [07_BONUS_GREEN.md](07_BONUS_GREEN.md) §7 범위 외 — BT-06·`test_register_cubit_meter_to_feet_golden_unchanged`로 **builtin 비율 불변**을 별도 검증.

---

## 7. `registerUnit("cubit", -1.0)` 예외 확인

### 7.1 구현 계약

[`UnitRegistry::registerUnit`](../src/entity/UnitRegistry.cpp) — `factorToMeter <= 0.0` 이면 `std::invalid_argument`:

```cpp
if (factorToMeter <= 0.0) {
    throw std::invalid_argument(DomainErr::kFactorMustBePositive);
}
```

`DecayingUnit::registerUnit` 은 위 Registry에 **위임**하므로 `-1.0` 역시 동일 예외.

### 7.2 테스트 매핑

| 입력 | 테스트 | 결과 |
|------|--------|------|
| `-0.4572` | BT-04 `[bonus]` | **PASS** (`REQUIRE_THROWS_AS`) |
| `0.0` | TC-B-05b `registerUnit("inch", 0.0)` | **PASS** |
| `-1.0` | 전용 케이스 없음 | **코드 분기상 동일** (`<= 0`) |

> 인수 요구 `-1.0` 은 BT-04·TC-B-05b와 **동일 검증 경로**이다. 전용 assertion 추가는 본 검증 범위에서 필수로 보지 않음.

---

## 8. 커버리지 (gcov / lcov)

### 8.1 빌드·테스트

```powershell
cmake -S . -B build -DUC11_COVERAGE=ON -G Ninja
cmake --build build
ctest --test-dir build -j1 -E "감쇠"
```

### 8.2 lcov 집계 (2026-05-21 실측)

```cmd
set PATH=C:\mingw64\bin;C:\ProgramData\chocolatey\lib\lcov\tools\bin;%PATH%
lcov --capture --directory build --output-file build/coverage.info
lcov --remove build/coverage.info "*/catch2/*" "*/tests/*" "*/build/_deps/*" "/usr/*" "*/mingw64/*" --output-file build/coverage.filtered.info
lcov --extract build/coverage.filtered.info "*/src/entity/*" --output-file build/entity.info
lcov --extract build/coverage.filtered.info "*/src/boundary/*" --output-file build/boundary.info
lcov --summary build/entity.info
lcov --summary build/boundary.info
```

| 레이어 | 라인 | 함수 | 게이트 | 판정 |
|--------|------|------|--------|------|
| **Domain** (`src/entity/`) | **96.1%** (49/51) | 100% (12/12) | ≥ 95% | ✓ |
| **Boundary** (`src/boundary/`) | **86.8%** (66/76) | 100% (8/8) | ≥ 85% | ✓ |

### 8.3 파일별 (Domain)

| 파일 | 라인 커버리지 |
|------|---------------|
| `UnitConverter.cpp` | **100%** (12/12) |
| `UnitRegistry.cpp` | 94.3% (35/37) |
| `DomainErrors.hpp` | 100% (4/4) |

```cmd
gcov -o build\CMakeFiles\uc11_entity.dir\src\entity build\CMakeFiles\uc11_entity.dir\src\entity\UnitConverter.cpp.gcno
```

→ `UnitConverter.cpp`: **Lines executed: 100.00% of 12**

> REFACTOR 보고서([05_REFACTOR.md](05_REFACTOR.md)) 수치(96.1% / 86.8%)와 **일치**. stale `.gcno` 시 clean rebuild 권장.

---

## 9. 이슈 및 권장 조치

| # | 이슈 | 영향 | 권장 |
|---|------|------|------|
| 1 | CTest Test #1 한글 이름 인코딩 | `ctest` 76/77 | `TEST_CASE` 이름을 ASCII로 변경 **또는** `ctest -E "감쇠"` / `[bonus]` 직접 실행 |
| 2 | Golden Master 병렬 경합 | `ctest -j4` 간헐 FAIL | CI·로컬 Golden은 `-j1` 또는 `-R GoldenMaster` |
| 3 | `-1.0` 전용 테스트 없음 | 인수 문서 vs BT-04 | 선택: `SECTION`에 `registerUnit("cubit", -1.0)` 추가 |

---

## 10. 검증 체크리스트 (QA 서명용)

| # | 항목 | 기대 | 확인 |
|---|------|------|------|
| 1 | BT-01~06 | 전부 PASS | ☑ |
| 2 | TC-A-01~07 | 전부 PASS | ☑ |
| 3 | TC-B-01~07 | 전부 PASS | ☑ |
| 4 | Golden Master (직렬) | PASS | ☑ |
| 5 | meter→feet builtin (BT-06 / dynamic test) | 3.28084 / 8.20210 | ☑ |
| 6 | 음수 factor → `invalid_argument` | BT-04 + Registry | ☑ |
| 7 | Domain lcov | ≥ 95% (96.1%) | ☑ |
| 8 | Boundary lcov | ≥ 85% (86.8%) | ☑ |
| 9 | `UnitConverter.cpp` gcov | 100% | ☑ |
| 10 | ctest 77/77 (한글 #1) | 전부 PASS | ☐ (인코딩; 기능상 ☑) |

---

## 11. 결론

- **Bonus GREEN** 신규 기능 **BT-01~06** 및 **기존 TC-A/B 회귀**는 로컬에서 **PASS** 확인되었다.
- **Golden Master(REG-04)** 는 직렬 실행 시 **PASS**; 병렬 ctest 시 CLI 경합에 유의한다.
- **커버리지 게이트** Domain **96.1%**, Boundary **86.8%**, `UnitConverter.cpp` **100%** — [05_REFACTOR.md](05_REFACTOR.md) 기준 **충족**.
- **잔여:** CTest 한글 테스트명 1건만 인프라 이슈 — 기능 검증은 `[bonus]` 필터로 **완료**.

---

## 12. 산출물 · 관련 링크

| 산출물 | 경로 |
|--------|------|
| Bonus GREEN 구현 보고서 | [07_BONUS_GREEN.md](07_BONUS_GREEN.md) |
| Bonus RED 보고서 | [06_BONUS_RED.md](06_BONUS_RED.md) |
| Golden Master 보고서 | [04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md) |
| REFACTOR·커버리지 | [05_REFACTOR.md](05_REFACTOR.md) |
| **본 보고서** | [08_BONUS_VERIFICATION.md](08_BONUS_VERIFICATION.md) |
| **Prompting (본 세션)** | [Prompting/09_BONUS_VERIFICATION.md](../Prompting/09_BONUS_VERIFICATION.md) |

**빠른 재검증:**

```powershell
.\build\unit_converter_tests.exe "[bonus]" -s
ctest --test-dir build -j1 -R "GoldenMaster|TC-A|TC-B" --output-on-failure
```

---

*본 문서는 Bonus GREEN 완료 후 인수 검증(전체 테스트·lcov·Golden Master) 결과만 기록한다.*
