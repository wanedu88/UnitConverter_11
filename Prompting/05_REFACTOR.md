# UnitConverter_11 — 대화 세션 기록 (REFACTOR / BCE / Dual-Track)
_Exported on 2026-05-21 from Cursor agent session (UnitConverter_11 workspace)_

**워크스페이스:** `c:\DEV\UnitConverter_11`  
**선행 문서:** [04_GOLDEN_MASTER.md](04_GOLDEN_MASTER.md) (GM REG-04 · 71/71), [03_GREEN.md](03_GREEN.md) (GREEN · lcov)  
**본 세션 주제:** 코드 스멜·ECB 분석 · 리팩토링 계획 · Dual-Track REFACTOR 4커밋 · 검증 · Report  
**최종 갱신:** 2026-05-21 (Prompting 전체 저장 — 현재 턴)

---

## 세션 요약

| 단계 | 사용자 요청 (요지) | 산출물 | 비고 |
|------|-------------------|--------|------|
| 1 | `UnitConverter.cpp` 테스트 존재·리팩토링 전 테스트 필요 이유 | 분석 응답 (Ask) | `test_*.cpp` 11개, GM 있음 |
| 2 | 코드 스멜 표 (수정 금지) | SRP·예외·긴 함수 표 | if-else·매직넘버는 이미 이전됨 |
| 3 | ECB(BCE) 분석 — main 혼재·이동 대상·OCP 구조 | 분석 응답 | Control 공백 지적 |
| 4 | 리팩토링 계획서 (수정 금지) | 대상·후보·테스트·검증 명령 표 | Phase 0~4 |
| 5 | 계획서 **완료 항목** 현황 | R-C1/B1/U2/C2/L1 상태 표 | |
| 6 | 미완료 항목 **순차 진행** (Agent) | 4 REFACTOR 커밋 | 76/76 PASS |
| 7 | 전체 테스트·커버리지 확인 | 검증 응답 | Entity 96.1%, Boundary 86.8% |
| 8 | **Report** `05_REFACTOR.md` | [Report/05_REFACTOR.md](../Report/05_REFACTOR.md) | |
| 9 | **Prompting 저장** (현재 대화) | 본 파일 | Turn 1~9 |

---

## 1. 사용자 프롬프트 원문 요약

### Turn 1 — 테스트·리팩토링 전제 (Ask, 코드 수정 금지)

```
@UnitConverter.cpp 를 보고
1. test_*.cpp 존재 여부, 없으면 어떤 파일 테스트 필요한지
2. 테스트 없이 리팩토링 시작하면 안 되는 이유 (한 줄)
```

**에이전트 결론:**

- `tests/`에 `test_*.cpp` **11개** 존재 (GREEN·RED·GM 포함).
- 루트 `UnitConverter.cpp`는 **얇은 CLI**; entity/boundary로 이미 분리.
- legacy E2E는 `test_golden_master.cpp`가 담당.
- 한 줄: *동작 변경 여부를 자동으로 알 수 없어 리팩토링과 기능 변경을 구분할 수 없기 때문.*

---

### Turn 2 — 코드 스멜 분석 (Ask)

```
점검 항목:
- main() SRP (파싱·검증·변환·출력 혼재)
- if-else 체인, 매직 넘버 3.28084/1.09361
- 예외 처리, 긴 함수(20줄+), 미사용 변수

표: | 파일명 | 줄번호 | 스멜 종류 | 문제 설명 | 우선순위 |
```

**에이전트 결론 (현재 파일 기준):**

| 파일 | 줄 | 스멜 | 우선순위 |
|------|-----|------|----------|
| `UnitConverter.cpp` | 9–31 | SRP (오케스트레이션 잔존) | 중간 |
| `UnitConverter.cpp` | 9–32 | 긴 함수 (~24줄) | 낮음 |
| `UnitConverter.cpp` | 13–14, 26–28 | 불완전한 오류/예외 처리 | 낮음 |

- **if-else·매직 넘버:** 루트 `main()`에는 **해당 없음** → `ConversionConstants` / `UnitRegistry` / `UnitConverter::convert`로 이전 완료.

---

### Turn 3 — ECB(BCE) 분석 (Ask)

```
1. main()에서 Entity/Control/Boundary 혼재
2. 어느 코드를 어느 모듈로 이동
3. 새 단위 추가 시 변경 최소화 구조
```

**에이전트 결론:**

- **과거 baseline:** 비율·if-else·파싱이 `main()`에 있었음 → GREEN에서 제거됨.
- **현재 `main()`:** Boundary I/O + **Control 공백**(Entity 직접 `convertAll`).
- **이동 목표:** `CliApp`, `ConvertLengthUseCase`, (후속) `LoadConfigUseCase`.
- **OCP:** `registerUnit` / `config/units.json`만 추가, `convert` 공식 불변.

---

### Turn 4 — 리팩토링 계획서 (Ask)

```
## 리팩토링 대상 목록
## 리팩토링 후보 목록 (R-U1~R-L4, R-C1, R-B1)
## 테스트 선행 필요 항목
## 리팩토링 후 검증 방법
```

**핵심 계획:**

| 순번 | 대상 | 기법 |
|------|------|------|
| 1 | `ConvertLengthUseCase` | Extract Class |
| 2 | `CliApp` | Extract Class |
| 3 | ERR 상수화 (R-U2) | Introduce Error Handler |
| 4 | legacy DEF 정합 | Golden 재확인 |
| 5–6 | ConversionRule VO, LoadConfig (선택) | Replace Type Code |

**Phase:** 0 GM 고정 → 1 Control → 2 CliApp → 3 R-U2 → 4 Config/VO.

---

### Turn 5 — 완료 항목 현황

```
리팩토링 계획서에서 현재 완료된 항목이 뭐야
```

**에이전트 결론 (당시):**

- ✓ R-U1, R-U3, R-L2~L4 (GREEN), GM, Phase 0
- ✓ R-C1 (`e578d0d` 이후)
- ✗ R-B1, R-U2, R-C2, R-L1, CliApp, LoadConfig

---

### Turn 6 — 미완료 순차 REFACTOR (Agent)

```
Dual-Track REFACTOR만, 커밋 1개 단위
절대 규칙: 기능 추가 금지, 계약 불변, 테스트 완화 금지, GREEN 유지
출력: 목표 ID, Track 분리, 전후, 파일, 테스트, 위험, 커밋 메시지
```

**1차 커밋 — R-C1 (`e578d0d`):**

- `src/control/ConvertLengthUseCase.{hpp,cpp}`
- `tests/test_control_convert_length.cpp` (TC-C-01)
- `UnitConverter.cpp`에서 `convertAll` 위임
- **74/74 PASS**

**사용자:** `진행` → 커밋 완료.

**이후 3커밋 (동일 세션):**

| 해시 | 내용 |
|------|------|
| `be0446f` | R-B1 `CliApp`, `main()` 3줄 |
| `2382c03` | R-U2 `ErrMsg`/`DomainErr`/`ConfigErr`, CliApp 예외 보강 |
| `d43b3b3` | R-C2 `LoadConfigUseCase`, `RegisterUnitUseCase`, `ConversionRule`, `uc11_data` |

**최종:** **76/76 PASS**, Golden Master PASS.

---

### Turn 7 — 전체 테스트·커버리지 확인

```
cmake -S . -B build && cmake --build build
ctest --test-dir build -V
gcov / lcov

확인: TC-A/B, Golden Master, if-else 제거, 매직넘버, Domain/Boundary 분리,
      Domain ≥95%, Boundary ≥85%
```

**에이전트 실측 (clean lcov):**

| 항목 | 결과 |
|------|------|
| ctest | **76/76 PASS** |
| TC-A-01~07, TC-B-01~07 | PASS |
| Golden Master | PASS (REG-05) |
| Entity lcov | **96.1%** (49/51) ✓ |
| Boundary lcov | **86.8%** (66/76) ✓ |
| `src/` 인라인 3.28084/1.09361 | 없음 |
| if-else 단위 분기 | 없음 |

> stale `boundary/ConfigLoader.gcno` 시 Boundary **84%대** under-report — clean rebuild 필요.

---

### Turn 8 — Report 작성

```
Report 디렉토리에 보고서 작성
```

**산출물:** [Report/05_REFACTOR.md](../Report/05_REFACTOR.md)

---

### Turn 9 — Prompting 저장 (현재)

```
Prompting에 현재까지 대화를 저장해줘
```

**산출물:** 본 파일.

---

## 2. REFACTOR 커밋 상세 (4건)

### 2.1 `e578d0d` — R-C1 ConvertLengthUseCase

```text
refactor(control): extract ConvertLengthUseCase from legacy main
```

| 파일 | 작업 |
|------|------|
| `src/control/ConvertLengthUseCase.hpp` | 신규 |
| `src/control/ConvertLengthUseCase.cpp` | 신규 |
| `tests/test_control_convert_length.cpp` | TC-C-01 |
| `UnitConverter.cpp` | UseCase 위임 |
| `CMakeLists.txt` | `uc11_control` |

**의존:** Control → Entity only (Boundary 헤더 없음).

---

### 2.2 `be0446f` — R-B1 CliApp

```text
refactor(boundary): extract CliApp from legacy main
```

| 파일 | 작업 |
|------|------|
| `src/boundary/CliApp.hpp` | 신규 |
| `src/boundary/CliApp.cpp` | I/O + parse/validate/use case/format |
| `UnitConverter.cpp` | `return CliApp{}.run();` |
| `CMakeLists.txt` | boundary → control 링크, legacy는 boundary만 |

---

### 2.3 `2382c03` — R-U2 ERR 상수화

```text
refactor(errors): centralize ERR message constants (R-U2)
```

| 파일 | 작업 |
|------|------|
| `src/boundary/ErrorMessages.hpp` | ErrMsg |
| `src/entity/DomainErrors.hpp` | DomainErr |
| `src/boundary/ConfigErrors.hpp` | ConfigErr (이후 data로 이동) |
| `InputParser.cpp`, `InputValidator.cpp`, `UnitRegistry.cpp`, `ConfigLoader.cpp`, `CliApp.cpp` | 상수 사용 |

**계약:** `what()` 문자열 **동일** (리터럴 → constexpr).

**CliApp 보강:** `getline` 실패 stderr, `catch (std::exception&)`.

---

### 2.4 `d43b3b3` — R-C2, R-L1 Data·UseCase·VO

```text
refactor(control): add LoadConfig and RegisterUnit use cases (R-C2, R-L1)
```

| 파일 | 작업 |
|------|------|
| `src/data/ConfigLoader.{hpp,cpp}` | boundary에서 이전 |
| `src/boundary/ConfigLoader.hpp` | forward include |
| `src/entity/ConversionRule.hpp` | VO |
| `UnitRegistry::registerUnit(ConversionRule)` | 오버로드 |
| `LoadConfigUseCase`, `RegisterUnitUseCase` | Control |
| `tests/test_control_use_cases.cpp` | TC-C-02, TC-C-03 |
| `CMakeLists.txt` | `uc11_data` |

**순환 방지:** Control → Data → Entity (Control이 Boundary 직접 링크 안 함).

---

## 3. 최종 아키텍처 스냅샷

```text
main (UnitConverter.cpp)
  └── CliApp                    [Boundary]
        ├── parseInputLine      [Boundary]
        ├── validateInput       [Boundary]
        ├── ConvertLengthUseCase [Control]
        │     └── convertAll    [Entity]
        └── formatTableLine     [Boundary]

LoadConfigUseCase [Control] → loadConfig [Data] → UnitRegistry [Entity]
RegisterUnitUseCase [Control] → ConversionRule [Entity]
```

**CMake 라이브러리:** `uc11_entity` ← `uc11_data` ← `uc11_control` ← `uc11_boundary` ← `unit_converter_legacy`

---

## 4. 테스트·회귀 ID

| ID | 내용 | REFACTOR 후 |
|----|------|-------------|
| REG-04 | Golden Master 4시나리오 | PASS (불변) |
| REG-05 | REFACTOR 후 GM·TC 불변 | PASS |
| TC-C-01 | ConvertLengthUseCase | 신규 PASS |
| TC-C-02 | LoadConfig missing → builtins | 신규 PASS |
| TC-C-03 | RegisterUnit cubit | 신규 PASS |

**ctest:** 71 (GREEN+GM) → **76** (+3 Control).

---

## 5. 검증 명령 (재현)

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build -C Debug --output-on-failure
ctest --test-dir build -C Debug -R GoldenMaster --output-on-failure

# 커버리지
cmake -S . -B build -DUC11_COVERAGE=ON -DUC11_GREEN_PHASE=ON
cmake --build build
ctest --test-dir build -C Debug
lcov --capture --directory build --output-file build/coverage.info
lcov --remove build/coverage.info "*/catch2/*" "*/tests/*" "*/build/_deps/*" "/usr/*" "*/mingw64/*" `
     --output-file build/coverage.filtered.info
lcov --extract build/coverage.filtered.info "*/src/entity/*" --output-file build/entity.info
lcov --extract build/coverage.filtered.info "*/src/boundary/*" --output-file build/boundary.info
lcov --summary build/entity.info
lcov --summary build/boundary.info
```

---

## 6. 계획 대비 최종 완료표

| ID | 내용 | 상태 |
|----|------|------|
| R-U1 | InputParser | ✓ GREEN |
| R-U2 | ERR 상수화 | ✓ `2382c03` |
| R-U3 | OutputFormatter | ✓ GREEN |
| R-L1 | ConversionRule VO | ✓ `d43b3b3` |
| R-L2 | UnitRegistry | ✓ GREEN |
| R-L3 | ConversionConstants | ✓ GREEN |
| R-L4 | convert() 허브 | ✓ GREEN |
| R-C1 | ConvertLengthUseCase | ✓ `e578d0d` |
| R-C2 | LoadConfigUseCase | ✓ `d43b3b3` |
| R-B1 | CliApp | ✓ `be0446f` |
| — | RegisterUnitUseCase | ✓ `d43b3b3` |

**미완료 (범위 외):** DEF-013 단위명 정규화, GM-08 GitHub required check, Control 100% 커버리지 전용 게이트.

---

## 7. 생성·갱신 파일 인덱스

| 경로 | 유형 |
|------|------|
| [Report/05_REFACTOR.md](../Report/05_REFACTOR.md) | REFACTOR 보고서 |
| [Prompting/05_REFACTOR.md](05_REFACTOR.md) | 본 세션 기록 |
| `src/control/*` | Control 레이어 |
| `src/data/ConfigLoader.*` | Data 레이어 |
| `src/boundary/CliApp.*` | Boundary CLI |
| `src/boundary/ErrorMessages.hpp` | ERR 상수 |
| `src/entity/ConversionRule.hpp`, `DomainErrors.hpp` | Entity |
| `tests/test_control_*.cpp` | Control 테스트 |

---

## 8. 에이전트 운영 메모

| 규칙 | 세션 준수 |
|------|-----------|
| Ask 모드에서 코드 수정 금지 | Turn 1~4 분석만 |
| REFACTOR 전 테스트 GREEN | Step 0 73/74 → 최종 76/76 |
| 커밋 1목표 1커밋 | 4커밋 분리 |
| `build-cov/` 커밋 제외 | git add 시 제외 |
| Golden Master 게이트 | 매 커밋 후 PASS |

**브랜치:** `refactoring` — `origin/refactoring` 대비 REFACTOR 4커밋 ahead (GM+CI 커밋 포함 시 5+).

---

## 9. 대화 흐름 (한 줄 타임라인)

```text
분석(테스트·스멜·ECB) → 계획서 → 완료현황 질의 → R-C1 커밋 → 진행 요청
→ R-B1/R-U2/R-C2+L1 연속 커밋 → ctest+lcov 검증 → Report/05_REFACTOR.md
→ Prompting/05_REFACTOR.md (본 파일)
```

---

*본 문서는 REFACTOR 단계 Cursor 대화 전체를 보존한다. 공식 검증 수치·아키텍처는 [Report/05_REFACTOR.md](../Report/05_REFACTOR.md)를 정본으로 한다.*
