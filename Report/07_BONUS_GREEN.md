# UnitConverter_11 — Bonus GREEN (감쇠 단위) 보고서

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_11 — Dual-Track UI + Logic TDD |
| **작성일** | 2026-05-21 |
| **단계** | **Bonus GREEN** — `DecayingUnit` 최소 구현 · BT-01~06 PASS |
| **선행 보고서** | [06_BONUS_RED.md](06_BONUS_RED.md) (Bonus RED 6/6 FAIL), [03_GREEN.md](03_GREEN.md) (Entity GREEN) |
| **관련 문서** | [README.md](../README.md), [docs/test_plan.md](../docs/test_plan.md) |

---

## 1. 요약 (Executive Summary)

Bonus RED에서 고정한 **`DecayingUnit` 동적 등록 + meter 허브 환산** 계약을, **OCP(기존 if-else·Item 테이블 미수정)** 방식으로 최소 구현하여 **BT-01~06 전부 통과**했다. REFACTOR는 수행하지 않았다.

| 지표 | RED (이전) | GREEN (본 단계) |
|------|------------|-----------------|
| **테스트 케이스** | 1 / 1 FAIL | **1 / 1 PASS** |
| **assertion** | 6 / 6 FAIL | **10 / 10 PASS** |
| **태그** | `[bonus]` | `감쇠 단위 변환` |
| **구현** | 스텁 (`0.0`, `{}`) | `UnitRegistry` + Entity `convert` 위임 |
| **회귀** | non-bonus PASS 유지 | **38 + 38 test cases PASS** |
| **커밋** | — | `db5e32d` `feat(feature): add registerUnit with OCP registry` |

**GREEN 판정:** `unit_converter_tests.exe "[bonus]"` → **All tests passed (10 assertions in 1 test case)**.

---

## 2. 구현 접근 (OCP · 최소 GREEN)

### 2.1 설계 원칙

| 원칙 | 적용 |
|------|------|
| **OCP** | legacy `Item` / `UnitConverter.cpp` if-else **수정 없음** |
| **허브** | 모든 환산은 `factorToMeter` → meter → target |
| **재사용** | Entity `UnitRegistry`, `uc11::convert`, `uc11::convertAll` **위임만** |
| **독립 모듈** | `DecayingUnit`은 보너스 전용; Entity 코드 변경 없음 |

### 2.2 클래스 구조

```
DecayingUnit
  └── UnitRegistry registry_     // ctor: withBuiltins() → meter, feet, yard
        ├── registerUnit()       // Map에 name → factorToMeter 저장
        ├── convert()            // uc11::convert(registry_, …)
        └── convertAll()         // uc11::convertAll(registry_, …) → DecayingConversionResult
```

### 2.3 핵심 코드 (요지)

**생성자 — builtin 등록 (BT-06 선행):**

```cpp
DecayingUnit::DecayingUnit() : registry_(UnitRegistry::withBuiltins()) {}
```

**등록 — Registry에만 추가 (기존 분기 수정 없음):**

```cpp
void DecayingUnit::registerUnit(const std::string& name, double factorToMeter) {
    registry_.registerUnit(name, factorToMeter);
}
```

**환산 — meter 허브 공식 (Entity와 동일):**

```cpp
// value * factor(from) / factor(to)
return uc11::convert(registry_, fromUnit, value, toUnit);
```

Entity [`UnitConverter.cpp`](../src/entity/UnitConverter.cpp):

```cpp
const double meters = value * registry.factorToMeter(fromUnit);
return meters / registry.factorToMeter(toUnit);
```

### 2.4 제약 준수 (C-01 ~ C-04)

| ID | 제약 | GREEN 결과 |
|----|------|------------|
| C-01 | legacy Item / if-else **수정 금지** | **diff 없음** — `DecayingUnit`만 변경 |
| C-02 | factorToMeter > 0 | `UnitRegistry::registerUnit` 위임 → BT-04 PASS |
| C-03 | cubit 등록 후 meter→feet 불변 | BT-06: **3.28084** |
| C-04 | convertAll 전 단위 4행 | BT-05: meter/feet/yard/cubit |

---

## 3. 테스트 검증 (BT-01 ~ BT-06)

정본: [`tests/test_decaying_unit.cpp`](../tests/test_decaying_unit.cpp)

| ID | 시나리오 | 기대 | 결과 |
|----|----------|------|------|
| **BT-01** | cubit → meter | ≈ 0.4572 | **PASS** |
| **BT-02** | meter → cubit | ≈ 2.1872266 | **PASS** |
| **BT-03** | cubit → feet | ≈ 1.500000048 | **PASS** |
| **BT-04** | 음수 비율 등록 | `std::invalid_argument` | **PASS** |
| **BT-05** | convertAll 4단위 | size==4, 4 unit 포함 | **PASS** |
| **BT-06** | meter→feet 회귀 | ≈ 3.28084 | **PASS** |

### 3.1 RED → GREEN 대비

| BT | RED 실패 | GREEN 통과값 |
|----|----------|--------------|
| BT-01 | `0.0` vs `0.4572` | `0.4572` |
| BT-02 | `0.0` vs `2.1872265967` | `2.1872265967` |
| BT-03 | `0.0` vs `1.500000048` | `1.500000048` |
| BT-04 | 예외 없음 | `invalid_argument` throw |
| BT-05 | `0 == 4` | `4 == 4` |
| BT-06 | `0.0` vs `3.28084` | `3.28084` |

---

## 4. 빌드 및 실행

### 4.1 CMake 변경

| 타깃 | 변경 |
|------|------|
| `uc11_bonus` | `target_link_libraries(uc11_bonus PUBLIC uc11_entity)` 추가 |
| `unit_converter_tests` | 기존과 동일 (`test_decaying_unit.cpp` + `uc11_bonus`) |

### 4.2 실행 명령

```powershell
cd c:\DEV\UnitConverter_11
cmake -S . -B build
cmake --build build

# Bonus GREEN만 (권장)
.\build\unit_converter_tests.exe "[bonus]"
.\build\unit_converter_tests.exe "[bonus]" -s

# 전체 회귀 (실행 파일)
.\build\unit_converter_tests.exe
.\build\unit_converter_red_tests.exe
```

**기대 출력 (GREEN):**

```text
test cases: 1 | 1 passed
assertions: 10 | 10 passed
All tests passed (10 assertions in 1 test case)
```

### 4.3 회귀 테스트 (2026-05-21 실측)

| 실행 대상 | 결과 |
|-----------|------|
| `unit_converter_tests.exe` (전체) | **38 test cases, 69 assertions — ALL PASS** |
| `unit_converter_red_tests.exe` (전체) | **38 test cases, 76 assertions — ALL PASS** |
| `ctest --test-dir build` | **76/77 PASS** (Test #1: CTest 한글 테스트명 인코딩 필터 불일치 — exe 직접 실행 시 정상) |

TC-A / TC-B / Golden Master / 동적 등록(`test_dynamic_registration.cpp`) 등 **기존 GREEN 회귀 유지** 확인.

---

## 5. 커밋 및 변경 파일

| 항목 | 내용 |
|------|------|
| **브랜치** | `feature/new_feature` |
| **커밋** | `db5e32d` |
| **메시지** | `feat(feature): add registerUnit with OCP registry` |

| 파일 | 변경 요지 |
|------|-----------|
| [src/DecayingUnit.hpp](../src/DecayingUnit.hpp) | `UnitRegistry registry_` 멤버 |
| [src/DecayingUnit.cpp](../src/DecayingUnit.cpp) | RED 스텁 → Registry 위임 구현 |
| [CMakeLists.txt](../CMakeLists.txt) | `uc11_bonus` → `uc11_entity` 링크 |

**미변경 (의도적):**

- [`UnitConverter.cpp`](../UnitConverter.cpp) (legacy) — Item/if-else
- [`src/entity/UnitRegistry.cpp`](../src/entity/UnitRegistry.cpp) — 기존 GREEN 로직 재사용만
- [`tests/test_decaying_unit.cpp`](../tests/test_decaying_unit.cpp) — 테스트 수정 없음

---

## 6. Bonus RED와의 관계

| 구분 | Bonus RED ([06_BONUS_RED.md](06_BONUS_RED.md)) | Bonus GREEN (본 문서) |
|------|-----------------------------------------------|------------------------|
| `convert` | 항상 `0.0` | `uc11::convert(registry_, …)` |
| `registerUnit` | no-op | `registry_.registerUnit` |
| `convertAll` | `{}` | `uc11::convertAll` + DTO 매핑 |
| assertion | 6 FAIL | **10 PASS** |
| Entity 코드 | 변경 없음 | **변경 없음** (위임만) |

Entity `UnitRegistry`는 [03_GREEN.md](03_GREEN.md)에서 이미 구현되었다. 보너스 GREEN은 **동일 허브·Registry 패턴을 `DecayingUnit`에 composition**으로 적용하여, legacy 경로와 분리된 계약을 재검증한다.

---

## 7. 범위 외 (본 단계 미수행)

| 항목 | 비고 |
|------|------|
| **REFACTOR** | 헤더 주석 정리, 중복 제거 등 — **의도적 생략** |
| CLI `register:unit:factor` | Boundary 통합 |
| Golden Master cubit 시나리오 | [04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md) 범위 외 |
| `DecayingUnit` ↔ Entity 단일화 | 보너스·메인 트랙 분리 유지 |

---

## 8. 산출물 목록

| 산출물 | 경로 | 상태 |
|--------|------|------|
| API 헤더 | [src/DecayingUnit.hpp](../src/DecayingUnit.hpp) | GREEN |
| 구현 | [src/DecayingUnit.cpp](../src/DecayingUnit.cpp) | GREEN |
| Catch2 테스트 | [tests/test_decaying_unit.cpp](../tests/test_decaying_unit.cpp) | GREEN (10 PASS) |
| CMake `uc11_bonus` | [CMakeLists.txt](../CMakeLists.txt) | `uc11_entity` 링크 |
| Bonus RED 보고서 | [Report/06_BONUS_RED.md](06_BONUS_RED.md) | 완료 |
| **본 보고서** | [Report/07_BONUS_GREEN.md](07_BONUS_GREEN.md) | 완료 |
| **인수 검증 보고서** | [Report/08_BONUS_VERIFICATION.md](08_BONUS_VERIFICATION.md) | 완료 |

---

## 9. 검증 체크리스트 (QA 서명용)

| # | 항목 | 기대 | 확인 |
|---|------|------|------|
| 1 | `"[bonus]"` 1 test case | 1 passed | ☑ |
| 2 | assertion | 10 passed | ☑ |
| 3 | BT-01~06 | 전부 PASS | ☑ |
| 4 | BT-04 음수 비율 | `invalid_argument` | ☑ |
| 5 | non-bonus `unit_converter_tests` | 회귀 PASS | ☑ |
| 6 | `unit_converter_red_tests` | 회귀 PASS | ☑ |
| 7 | Item / legacy 테이블 | diff 없음 | ☑ |
| 8 | REFACTOR | 미수행 (GREEN만) | ☑ |

---

## 10. 결론

- **Bonus GREEN**은 `DecayingUnit`에 `UnitRegistry::withBuiltins()` + `registerUnit` 위임 + Entity meter 허브 `convert`/`convertAll`로 **BT-01~06(10 assertion) 전부 통과**했다.
- **OCP:** 기존 if-else·Item 테이블을 수정하지 않고, Registry에 **cubit만 추가**하는 방식으로 동적 등록을 만족했다.
- **회귀:** 메인 TDD 트랙(TC-A/B, RED track, Golden Master) **PASS 유지**.
- 검증: `unit_converter_tests.exe "[bonus]"` → **All tests passed**.
- **인수 검증(전체 테스트·lcov·Golden Master):** [08_BONUS_VERIFICATION.md](08_BONUS_VERIFICATION.md)

---

*본 문서는 Bonus GREEN(감쇠 단위) 단계만 기술한다. REFACTOR·CLI 통합은 후속 작업에서 다룬다. 인수 검증 상세는 [08_BONUS_VERIFICATION.md](08_BONUS_VERIFICATION.md).*
