# UnitConverter_11 — Bonus RED (감쇠 단위) 보고서

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_11 — Dual-Track UI + Logic TDD |
| **작성일** | 2026-05-21 |
| **단계** | **Bonus RED** — `DecayingUnit` 동적 등록 · 실패 assertion 고정 |
| **선행 보고서** | [02_RED.md](02_RED.md) (Dual-Track RED 48/48), [03_GREEN.md](03_GREEN.md) (Entity GREEN) |
| **후속** | Bonus GREEN — `src/DecayingUnit.cpp` 구현 (본 문서 범위 외) |
| **관련 문서** | [README.md](../README.md), [docs/test_plan.md](../docs/test_plan.md) |

---

## 1. 요약 (Executive Summary)

메인 TDD 트랙(Dual-Track RED/GREEN)과 **분리된 보너스 트랙**으로, **감쇠(Decaying) 단위** — 런타임 `registerUnit` + meter 허브 환산 — 요구를 Catch2로 고정했다.

| 지표 | 목표 | 실측 (2026-05-21) |
|------|------|-------------------|
| **테스트 케이스** | 1 `TEST_CASE` × 6 `SECTION` | **1 / 1 FAIL** |
| **assertion** | 6 FAIL (RED) | **6 / 6 FAIL** |
| **태그** | `[bonus]` | `감쇠 단위 변환` |
| **구현** | 스텁만 (GREEN 금지) | `convert` → `0.0`, `convertAll` → `{}` |
| **기존 Entity GREEN** | 영향 없음 | `uc11_require_green_phase()` **미사용** |

**RED 판정:** `unit_converter_tests.exe "[bonus]"` 실행 시 **0% passed**가 정상이다. 본 트랙은 `UC11_GREEN_PHASE`와 무관하게 **실제 `REQUIRE`** 가 항상 실행된다.

---

## 2. 기능 요구 (Bonus Spec)

### 2.1 API (목표 계약)

| API | 시그니처 (요지) | 역할 |
|-----|-----------------|------|
| `registerUnit` | `("cubit", 0.4572)` | 1 cubit = 0.4572 meter 등록 |
| `convert` | `(from, value, to) → double` | meter 허브 환산 |
| `convertAll` | `(from, value) → vector` | Registry **전 단위** 1행씩 |

모듈: [`src/DecayingUnit.hpp`](../src/DecayingUnit.hpp), [`src/DecayingUnit.cpp`](../src/DecayingUnit.cpp)  
네임스페이스: `uc11::DecayingUnit`

### 2.2 수치 계약 (ε ≤ 1e-5)

| 변환 | 입력 | 기대값 (근사) |
|------|------|----------------|
| cubit → meter | `convert("cubit", 1.0, "meter")` | **0.4572** |
| meter → cubit | `convert("meter", 1.0, "cubit")` | **1 / 0.4572 ≈ 2.1872266** |
| cubit → feet | `convert("cubit", 1.0, "feet")` | **0.4572 × 3.28084 ≈ 1.500000048** |

비율 상수 (테스트·PRD 정합):

- **1 meter = 3.28084 feet**
- feet ↔ yard는 **meter 허브만** (독립 feet↔yard 비율 금지)

### 2.3 제약 (구현 시 준수)

| ID | 제약 |
|----|------|
| C-01 | legacy **`Item` 구조체** 및 `UnitConverter.cpp` 기존 if-else 변환 테이블 **수정 금지** |
| C-02 | `registerUnit` — **factorToMeter > 0** 강제; 음수·0 → `std::invalid_argument` |
| C-03 | cubit 등록 후에도 builtin **meter → feet** = 3.28084 **불변** (회귀 BT-06) |
| C-04 | `convertAll` — 등록된 **모든** 단위 포함 (builtin 3 + cubit = **4행**, BT-05) |

### 2.4 범위 외

- CLI `register:unit:factor` 통합 (Boundary/Control)
- Golden Master E2E에 cubit 시나리오 추가 ([04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md) §2.3 제외 항목)
- Entity `UnitRegistry` / `UnitConverter` 코드 변경 (이미 GREEN; 보너스는 **별도** `DecayingUnit`)

---

## 3. 테스트 명세 (BT-01 ~ BT-06)

정본: [`tests/test_decaying_unit.cpp`](../tests/test_decaying_unit.cpp)

```cpp
TEST_CASE("감쇠 단위 변환", "[bonus]") { /* SECTION [BT-01] … [BT-06] */ }
```

| ID | SECTION | Given | When | Then |
|----|---------|-------|------|------|
| **BT-01** | cubit → meter | `registerUnit("cubit", 0.4572)` | `convert("cubit", 1.0, "meter")` | ≈ **0.4572** (ε 1e-5) |
| **BT-02** | meter → cubit | 동일 등록 | `convert("meter", 1.0, "cubit")` | ≈ **2.1872266** |
| **BT-03** | cubit → feet | 동일 등록 | `convert("cubit", 1.0, "feet")` | ≈ **1.500000048** |
| **BT-04** | 음수 비율 | 빈 converter | `registerUnit("badunit", -0.4572)` | `std::invalid_argument` |
| **BT-05** | convertAll 전 단위 | cubit 등록 후 | `convertAll("cubit", 1.0)` | **size == 4**, meter/feet/yard/cubit 포함 |
| **BT-06** | builtin 회귀 | cubit 등록 후 | `convert("meter", 1.0, "feet")` | ≈ **3.28084** (cubit 영향 없음) |

---

## 4. RED 스텁 및 빌드

### 4.1 스텁 동작 (현재)

[`src/DecayingUnit.cpp`](../src/DecayingUnit.cpp):

| 메서드 | RED 동작 | BT 실패 원인 |
|--------|----------|--------------|
| `registerUnit` | no-op (검증 없음) | BT-04: 예외 미발생 |
| `convert` | 항상 `0.0` | BT-01, 02, 03, 06: `0.0` vs `Approx(...)` |
| `convertAll` | 빈 `vector` | BT-05: `0 == 4` |

### 4.2 CMake

| 타깃 | 소스 | 링크 |
|------|------|------|
| `uc11_bonus` | `src/DecayingUnit.cpp` | — |
| `unit_converter_tests` | `tests/test_decaying_unit.cpp` | `uc11_bonus` + 기존 라이브러리 |

메인 GREEN 테스트(36+건)와 **동일 실행 파일**에 보너스 1건이 추가된다. `-DUC11_GREEN_PHASE`는 보너스 테스트에 **영향 없음**.

### 4.3 실행 명령

```powershell
cd c:\DEV\UnitConverter_11
cmake -S . -B build2 -G "MinGW Makefiles"
cmake --build build2 --target unit_converter_tests

# Bonus RED만 (권장)
.\build2\unit_converter_tests.exe "[bonus]"

# 상세 실패 메시지
.\build2\unit_converter_tests.exe "[bonus]" -s
```

**기대 출력 (RED 정상):**

```text
test cases: 1 | 1 failed
assertions: 6 | 6 failed
```

종료 코드: **0이 아님** (예: `6`).

### 4.4 실측 실패 요약 (2026-05-21)

| BT | 실패 메시지 (요지) |
|----|-------------------|
| BT-01 | `0.0 == Approx(0.4572)` |
| BT-02 | `0.0 == Approx(2.1872265967)` |
| BT-03 | `0.0 == Approx(1.500000048)` |
| BT-04 | `REQUIRE_THROWS_AS` — 예외 없음 |
| BT-05 | `0 == 4` |
| BT-06 | `0.0 == Approx(3.28084)` |

---

## 5. Dual-Track RED와의 관계

| 구분 | Dual-Track RED ([02_RED.md](02_RED.md)) | Bonus RED (본 문서) |
|------|----------------------------------------|---------------------|
| 테스트 파일 | `red_track_*.cpp`, `test_*.cpp` (가드) | `test_decaying_unit.cpp` |
| 실패 방식 | `FAIL("RED")` 또는 `uc11_require_green_phase` | **실제 `REQUIRE` / `REQUIRE_THROWS_AS`** |
| 필터 | `[red]`, 전체 ctest 48+ | `[bonus]` |
| 구현 모듈 | Entity / Boundary / Control | **`DecayingUnit` only** |
| Item 테이블 | GREEN에서 대체·위임 | **절대 수정 금지** (C-01) |

Entity 레이어의 `UnitRegistry::registerUnit` / `convert`는 이미 GREEN([03_GREEN.md](03_GREEN.md))이다. 보너스 트랙은 **동일 개념을 독립 클래스로 재검증**하며, legacy `Item` 경로와 분리한다.

---

## 6. GREEN 진입 조건 (다음 작업)

| 순서 | 작업 | 완료 기준 |
|------|------|-----------|
| 1 | `DecayingUnit` ctor에 builtin meter/feet/yard 등록 | BT-06 선행 |
| 2 | `registerUnit` — 중복·빈 이름·factor ≤ 0 검증 | BT-04 PASS |
| 3 | meter 허브 `convert` | BT-01, 02, 03 PASS |
| 4 | `convertAll` — `listUnits` 정렬·전 행 반환 | BT-05 PASS |
| 5 | `.\build2\unit_converter_tests.exe "[bonus]"` | **6/6 assertion PASS** |
| 6 | 기존 `unit_converter_tests` (non-bonus) | 회귀 PASS (`UC11_GREEN_PHASE=ON` 시) |

**금지:** 테스트 ε 완화만으로 통과 · `Item` / legacy 분기 직접 수정 · 스텁 `return 0.0` 유지한 채 GREEN 선언.

---

## 7. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| API 헤더 | [src/DecayingUnit.hpp](../src/DecayingUnit.hpp) | RED (선언만) |
| RED 스텁 | [src/DecayingUnit.cpp](../src/DecayingUnit.cpp) | RED |
| Catch2 테스트 | [tests/test_decaying_unit.cpp](../tests/test_decaying_unit.cpp) | RED (6 FAIL) |
| CMake `uc11_bonus` | [CMakeLists.txt](../CMakeLists.txt) | 완료 |
| 본 보고서 | [Report/06_BONUS_RED.md](06_BONUS_RED.md) | 완료 |

---

## 8. 검증 체크리스트 (QA 서명용)

| # | 항목 | 기대 | 확인 |
|---|------|------|------|
| 1 | `"[bonus]"` 필터 1 test case | 1 failed | ☐ |
| 2 | assertion 6건 | 6 failed | ☐ |
| 3 | 스텁 `convert` ≠ GREEN 값 | `0.0` 반환 확인 | ☐ |
| 4 | BT-04 예외 | `invalid_argument` 미구현 시 FAIL | ☐ |
| 5 | non-bonus 테스트 | 기존 GREEN 회귀 유지 | ☐ |
| 6 | Item / legacy 테이블 | diff 없음 | ☐ |

---

## 9. 결론

- **Bonus RED**는 `DecayingUnit` 모듈에 대해 **6개 assertion**이 실패하는 상태로 고정되었다.
- Dual-Track RED의 `FAIL("RED")` 패턴과 달리, 보너스는 **의미 있는 `REQUIRE`** 로 계약 수치를 명시한다.
- GREEN 시 **meter 허브 + 독립 Registry**를 `DecayingUnit` 내부에만 구현하며, **legacy Item 변환 로직은 건드리지 않는다**.
- 검증: `unit_converter_tests.exe "[bonus]"` → **test cases: 1 failed, assertions: 6 failed**.

---

*본 문서는 Bonus RED(감쇠 단위) 단계만 기술한다. Bonus GREEN 인수는 후속 보고서에서 다룬다.*
