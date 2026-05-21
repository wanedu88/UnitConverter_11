# UnitConverter_11 — 대화 세션 기록 (Bonus RED / 감쇠 단위)
_Exported on 2026-05-21 from Cursor agent session (UnitConverter_11 workspace)_

**워크스페이스:** `c:\DEV\UnitConverter_11`  
**선행 문서:** [02_RED.md](02_RED.md), [03_GREEN.md](03_GREEN.md), [04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md)  
**본 세션 주제:** Bonus RED — `DecayingUnit` 실패 테스트 · RED 실행 명령 · Report · Prompting 저장  
**최종 갱신:** 2026-05-21 (Prompting 전체 저장 — 현재 턴)

---

## 세션 요약

| 단계 | 사용자 요청 (요지) | 산출물 | 비고 |
|------|-------------------|--------|------|
| 1 | 신규 기능 **Bonus RED** 테스트 작성 (구현 금지) | `src/DecayingUnit.hpp/.cpp`, `tests/test_decaying_unit.cpp`, CMake `uc11_bonus` | BT-01~06, `[bonus]` |
| 2 | RED 실행 확인 명령어 안내 | PowerShell 명령 응답 | `unit_converter_tests.exe "[bonus]"` |
| 3 | **Report** 보너스 RED 보고서 | [Report/06_BONUS_RED.md](../Report/06_BONUS_RED.md) | |
| 4 | **Prompting 저장** (현재 대화) | 본 파일 | Turn 1~4 |

---

## 1. 사용자 프롬프트 원문

### Turn 1 — Bonus RED 테스트 (구현 금지)

```
프로젝트: c:\DEV\UnitConverter_11

신규 기능 RED 테스트를 작성하라. (구현 금지)

기능 설명:
  registerUnit("cubit", 0.4572)  // 1 cubit = 0.4572 meter
  convert("cubit", 1.0, "meter") == 0.4572 (오차 1e-5)
  convert("meter", 1.0, "cubit") == 1.0/0.4572 ≈ 2.1872 (오차 1e-5)
  convert("cubit", 1.0, "feet")  == 0.4572 * 3.28084 ≈ 1.4997 (오차 1e-5)

제약:
  - Item 구조체(UnitConverter.cpp의 기존 변환 로직) 수정 금지
  - 음수 비율 등록 시 예외 발생 (비율 > 0 강제)

테스트 케이스 (최소 5개):
  BT-01: registerUnit("cubit", 0.4572) 후 cubit→meter 변환
  BT-02: registerUnit 후 meter→cubit 역변환
  BT-03: registerUnit 후 cubit→feet 교차 변환
  BT-04: 음수 비율 등록 → std::invalid_argument 발생
  BT-05: registerUnit 후 convertAll("cubit", 1.0) → 전 단위 반환
  BT-06: 기존 meter→feet 결과 불변 (회귀 보호)

테스트 마킹: TEST_CASE("감쇠 단위 변환", "[bonus]") { REQUIRE(...); }

산출물: src/DecayingUnit.cpp + tests/test_decaying_unit.cpp (실패 상태)
```

### Turn 2 — RED 실행 명령

```
red를 실행서 확인할 수 있는 명령어를 알려줘
```

### Turn 3 — Report 작성

```
Report에 보너스 레드 보고서를 작성해줘
```

### Turn 4 — Prompting 저장

```
Prompting에 현재 대화 내용을 저장해줘
```

---

## 2. Turn 1 — 에이전트 수행 (Bonus RED)

### 2.1 설계 결정

| 항목 | 결정 |
|------|------|
| 모듈 | `uc11::DecayingUnit` — Entity `UnitRegistry`와 **분리** (legacy Item 미수정) |
| RED 패턴 | Dual-Track `FAIL("RED")` **아님** — 실제 `REQUIRE` / `REQUIRE_THROWS_AS` (항상 실행) |
| GREEN 가드 | `uc11_require_green_phase()` **미사용** |
| 헤더 | `src/DecayingUnit.hpp` — 테스트 컴파일·API 선언 (산출물에 `.cpp`만 명시되었으나 헤더 필수) |
| 스텁 | `convert` → `0.0`, `convertAll` → `{}`, `registerUnit` no-op (음수 검증 없음) |

### 2.2 생성·수정 파일

| 경로 | 역할 |
|------|------|
| [src/DecayingUnit.hpp](../src/DecayingUnit.hpp) | `DecayingUnit`, `DecayingConversionResult` |
| [src/DecayingUnit.cpp](../src/DecayingUnit.cpp) | RED 스텁 |
| [tests/test_decaying_unit.cpp](../tests/test_decaying_unit.cpp) | `TEST_CASE("감쇠 단위 변환", "[bonus]")` + SECTION BT-01~06 |
| [CMakeLists.txt](../CMakeLists.txt) | `uc11_bonus` 라이브러리, `unit_converter_tests` 링크 |

### 2.3 테스트 구조 (요지)

```cpp
TEST_CASE("감쇠 단위 변환", "[bonus]") {
    SECTION("[BT-01] ...") { /* cubit → meter 0.4572 */ }
    SECTION("[BT-02] ...") { /* meter → cubit 1/0.4572 */ }
    SECTION("[BT-03] ...") { /* cubit → feet 0.4572*3.28084 */ }
    SECTION("[BT-04] ...") { REQUIRE_THROWS_AS(..., std::invalid_argument); }
    SECTION("[BT-05] ...") { convertAll size==4, meter/feet/yard/cubit */ }
    SECTION("[BT-06] ...") { meter→feet 3.28084 after cubit register */ }
}
```

상수: `kCubitToMeter = 0.4572`, `kMeterToFeet = 3.28084`, `kBonusEpsilon = 1e-5`

### 2.4 RED 검증 결과 (빌드 `build2`)

```text
.\build2\unit_converter_tests.exe "[bonus]"

test cases: 1 | 1 failed
assertions: 6 | 6 failed
exit code: 6
```

| BT | 실패 요지 |
|----|-----------|
| BT-01 | `0.0 == Approx(0.4572)` |
| BT-02 | `0.0 == Approx(2.1872265967)` |
| BT-03 | `0.0 == Approx(1.500000048)` |
| BT-04 | 예외 미발생 |
| BT-05 | `0 == 4` |
| BT-06 | `0.0 == Approx(3.28084)` |

---

## 3. Turn 2 — RED 실행 명령 (응답 요지)

```powershell
cd c:\DEV\UnitConverter_11
cmake -S . -B build2 -G "MinGW Makefiles"
cmake --build build2 --target unit_converter_tests

# Bonus RED만 (권장)
.\build2\unit_converter_tests.exe "[bonus]"

# 상세 출력
.\build2\unit_converter_tests.exe "[bonus]" -s
```

**판정:** 종료 코드 0이 아니고 `6 failed` assertions → RED 정상.

**참고:** Dual-Track `unit_converter_red_tests`는 본 보너스와 별개.

---

## 4. Turn 3 — Report

| 문서 | 경로 |
|------|------|
| Bonus RED 보고서 | [Report/06_BONUS_RED.md](../Report/06_BONUS_RED.md) |

포함: Executive Summary, BT-01~06 표, 스텁·CMake, Dual-Track RED 비교, GREEN 진입 조건, QA 체크리스트.

---

## 5. Dual-Track / Entity GREEN과의 관계

| 구분 | 메인 RED/GREEN | Bonus RED (본 세션) |
|------|----------------|---------------------|
| 동적 등록 | `UnitRegistry::registerUnit` (GREEN) | `DecayingUnit::registerUnit` (RED) |
| 테스트 | `test_dynamic_registration.cpp` 등 | `test_decaying_unit.cpp` `[bonus]` |
| legacy Item | GREEN에서 허브·Registry 위임 | **수정 금지** — `DecayingUnit` 내부만 GREEN 예정 |
| 실패 방식 | `TestPhase.hpp` / `FAIL("RED")` | 실제 수치 `REQUIRE` |

Entity 레이어 cubit 시나리오는 이미 GREEN([03_GREEN.md](03_GREEN.md), `test_dynamic_registration.cpp`)이나, 보너스는 **독립 모듈**로 계약을 재고정한다.

---

## 6. Bonus GREEN 진입 조건 (다음 세션용)

1. `DecayingUnit` ctor — builtin meter/feet/yard (factor to meter)
2. `registerUnit` — factor > 0, `std::invalid_argument` (BT-04)
3. meter 허브 `convert` (BT-01~03, BT-06)
4. `convertAll` — 등록 단위 전부 1행 (BT-05)
5. `"[bonus]"` → 6/6 PASS
6. non-bonus `unit_converter_tests` 회귀 PASS

**금지:** Item/legacy if-else 테이블 수정 · ε만 완화 · 스텁 유지 채 GREEN 선언.

---

## 7. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| API 헤더 | [src/DecayingUnit.hpp](../src/DecayingUnit.hpp) | RED |
| RED 스텁 | [src/DecayingUnit.cpp](../src/DecayingUnit.cpp) | RED |
| Catch2 테스트 | [tests/test_decaying_unit.cpp](../tests/test_decaying_unit.cpp) | RED (6 FAIL) |
| CMake | [CMakeLists.txt](../CMakeLists.txt) | 완료 |
| Report | [Report/06_BONUS_RED.md](../Report/06_BONUS_RED.md) | 완료 |
| Prompting | [Prompting/07_BONUS_RED.md](07_BONUS_RED.md) | 완료 (본 파일) |

---

## 8. 검증 체크리스트

| # | 항목 | 기대 |
|---|------|------|
| 1 | `[bonus]` 1 test case | 1 failed |
| 2 | assertions | 6 failed |
| 3 | Item / legacy diff | 없음 |
| 4 | Report 06_BONUS_RED | 작성 완료 |
| 5 | Prompting 07_BONUS_RED | 작성 완료 |

---

*본 문서는 Bonus RED 세션(테스트·명령·Report·Prompting) 전체를 기록한다. Bonus GREEN은 [08_BONUS_GREEN.md](08_BONUS_GREEN.md)에서 다룬다.*
