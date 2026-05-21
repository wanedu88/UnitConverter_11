# UnitConverter_11 — 대화 세션 기록 (Golden Master / REG-04 / CI)
_Exported on 2026-05-21 from Cursor agent session (UnitConverter_11 workspace)_

**워크스페이스:** `c:\DEV\UnitConverter_11`  
**선행 문서:** [03_GREEN.md](03_GREEN.md) (GREEN 71/71 · lcov 게이트)  
**본 세션 주제:** Golden Master(Approval) 기준 파일 · Catch2 E2E · 생성 스크립트 · README GM 체크리스트 · CI workflow · Report  
**최종 갱신:** 2026-05-21 (Prompting 전체 저장 — 현재 턴)

---

## 세션 요약

| 단계 | 사용자 요청 (요지) | 산출물 | 비고 |
|------|-------------------|--------|------|
| 1 | Golden Master 설계·기준 파일·approve 패턴·4시나리오 | `tests/golden_master_*`, `test_golden_master.cpp`, 스크립트, CMake | REG-04 E2E |
| 2 | README `## Golden Master 회귀 안전장치` 삽입 | [README.md](../README.md) GM-01~09 | RED To-Do 아래 |
| 3 | **진행** — GM 체크리스트·CI·git add | `.github/workflows/golden_master.yml`, `add_test GoldenMaster`, `[x]` 갱신 | GM-08 수동 |
| 4 | **Report** 보고서 작성 | [Report/04_GOLDEN_MASTER.md](../Report/04_GOLDEN_MASTER.md) | |
| 5 | **Prompting 저장** (현재 대화 전부) | 본 파일 | Turn 1~5 |

---

## 1. 사용자 프롬프트 원문

### Turn 1 — Golden Master 설계·구현

```
[P] 회귀 테스트(Approval/Golden Master) 설계 전문가입니다.
[C] C++17, CMake, Catch2
[T] UnitConverter 출력 기반 Golden Master 기준 파일을 생성해줘.

    1) 기준 출력(tests/golden_master_expected.txt) 생성/보관 전략
       입력 시나리오 4개:
         "meter:2.5" / "feet:1.0" / "yard:1.0" / "meter:0.0"
       캡처 방식: stdout 리디렉션: ./UnitConverter < input.txt > actual.txt
       생성 후: git add tests/golden_master_expected.txt  (버전 관리 필수)

    2) approve 패턴 적용:
       - 기준 파일 없으면 → 현재 출력을 기준으로 자동 생성
       - 기준 파일 있으면 → actual vs expected 문자열 비교
       - 불일치 시: diff 출력 후 테스트 FAIL

    3) 기준 파일 구조:
       [meter:2.5]
       2.5 meter = 8.202100 feet
       2.5 meter = 2.734025 yard
       ---

[F] tests/golden_master_expected.txt 파일 내용 + 생성 스크립트
```

### Turn 2 — README 섹션 추가

```
@c:\DEV\UnitConverter_11/README.md 파일에 아래 섹션을 추가해줘.
"## RED 단계 To-Do 리스트" 아래에 새 섹션으로 삽입.

## Golden Master 회귀 안전장치
> Refactoring 시작 전 구축. GREEN 완료 후 즉시 적용.
### 기준 파일 생성
- [ ] GM-01: golden_master_expected.txt 생성 (meter:2.5 기준 출력)
... (GM-02~09 전문)
```

### Turn 3 — 진행

```
진행
```

_(에이전트: GM-01~07·09 완료, README `[x]`, CI workflow, `git add`, GM-08 안내만 `[ ]`)_

### Turn 4 — Report

```
Report에 보고서 작성해줘
```

### Turn 5 — Prompting 저장

```
Prompting에 현재 대화 내용을 전부 저장해줘
```

---

## 2. 에이전트 응답·구현 요약 (Turn 1)

### 2.1 조사·실측

- legacy 실행 파일: `unit_converter_legacy` → `OUTPUT_NAME UnitConverter`
- `meter:2.5` 실측 stdout (identity 포함 3줄):

```text
Insert value for converting (ex: meter:2.5): 2.5 meter = 8.2021 feet
2.5 meter = 2.5000 meter
2.5 meter = 2.7340 yard
```

- 사용자 예시 `8.202100`(6자리) ≠ 현재 `OutputFormatter` **4자리 half-up** → **실측값으로 baseline 고정**

### 2.2 생성·수정 파일

| 파일 | 역할 |
|------|------|
| [tests/golden_master_expected.txt](../tests/golden_master_expected.txt) | 4시나리오 기준 (identity 제외) |
| [tests/golden_master_helper.hpp](../tests/golden_master_helper.hpp) | CLI 캡처, 프롬프트 strip, identity 필터, diff |
| [tests/test_golden_master.cpp](../tests/test_golden_master.cpp) | Catch2 approve `[integration][golden][REG-04]` |
| [tests/scripts/generate_golden_master.ps1](../tests/scripts/generate_golden_master.ps1) | Windows 재생성 |
| [tests/scripts/generate_golden_master.sh](../tests/scripts/generate_golden_master.sh) | Linux 재생성 |
| [CMakeLists.txt](../CMakeLists.txt) | `test_golden_master.cpp`, `OUTPUT_NAME UnitConverter`, `UC11_LEGACY_EXE` |

### 2.3 확정 baseline (`golden_master_expected.txt`)

```text
[meter:2.5]
2.5 meter = 8.2021 feet
2.5 meter = 2.7340 yard
---
[feet:1.0]
1 feet = 0.3048 meter
1 feet = 0.3333 yard
---
[yard:1.0]
1 yard = 3.0000 feet
1 yard = 0.9144 meter
---
[meter:0.0]
0 meter = 0.0000 feet
0 meter = 0.0000 yard
```

### 2.4 캡처 규칙 (구현)

| 규칙 | 구현 위치 |
|------|-----------|
| stdin 1줄 → CLI | `runCliCaptureStdout` (`cmd /c` redirect / shell redirect) |
| 프롬프트 제거 | `kCliPrompt` strip |
| identity 줄 제외 | `isIdentityConversionLine` |
| 섹션 헤더 `[scenario]` + `---` | `buildGoldenDocument` |

### 2.5 Approve 패턴 (`test_golden_master.cpp`)

```cpp
// 1) actual = buildGoldenDocument(exe)
// 2) expectedPath 없음 → writeFile + FAIL("review, git add")
// 3) expected != actual → diffLines + FAIL
// 4) 일치 → PASS
```

### 2.6 검증 (Turn 1 말미)

```text
golden_master_cli_stdout_approval — PASS
ctest -R golden_master — PASS
```

---

## 3. 에이전트 응답·구현 요약 (Turn 2)

- [README.md](../README.md) 목차에 `Golden Master 회귀 안전장치` 링크 추가
- `## RED 단계 To-Do 리스트` 직후 GM-01~09 체크리스트 삽입 (초기 전부 `[ ]`)
- 사용자에게 완료 항목 `[x]` 반영 여부 질문 → Turn 3에서 진행

---

## 4. 에이전트 응답·구현 요약 (Turn 3 — 진행)

### 4.1 README GM 상태

| ID | 상태 |
|----|------|
| GM-01~07, GM-09 | `[x]` |
| GM-08 | `[ ]` + **GM-08: GitHub required check** 절 (Branch protection 절차) |

### 4.2 CMake `GoldenMaster`

```cmake
add_test(
    NAME GoldenMaster
    COMMAND unit_converter_tests "golden_master_cli_stdout_approval"
)
set_tests_properties(GoldenMaster PROPERTIES LABELS "golden;REG-04;integration")
```

- `ctest -R GoldenMaster` → **PASS** (Test #73)
- discover: `golden_master_cli_stdout_approval` (Test #1) — 동일 assertion 이중 등록

### 4.3 CI

- [.github/workflows/golden_master.yml](../.github/workflows/golden_master.yml)
- Job name: **`Golden Master (REG-04)`** (GM-08 required check 이름)
- Build: `unit_converter_legacy` + `unit_converter_tests` (타깃명; 산출 `UnitConverter`)
- Run: `ctest -R GoldenMaster`
- 실패 시: `generate_golden_master.sh` + `git diff` 진단

### 4.4 `git add` (스테이징, 커밋 없음)

```
.github/workflows/golden_master.yml
CMakeLists.txt
README.md
tests/golden_master_expected.txt
tests/golden_master_helper.hpp
tests/test_golden_master.cpp
tests/scripts/generate_golden_master.ps1
tests/scripts/generate_golden_master.sh
```

### 4.5 스크립트 수정

- `generate_golden_master.ps1`: `Join-Path` 3인자 → 중첩 2회 (PowerShell 호환)

---

## 5. 에이전트 응답·구현 요약 (Turn 4 — Report)

- [Report/04_GOLDEN_MASTER.md](../Report/04_GOLDEN_MASTER.md) 작성
- 구조: 03_GREEN.md와 동일 (요약, 배경, 기준 파일, 구현, 검증, PRD 매핑, 운영, REFACTOR 조건, QA, 결론)
- ctest 규모: GREEN 71 → discover 72 + `GoldenMaster` = **Total 73**

---

## 6. 기술 결정·이슈 기록

| 항목 | 결정 |
|------|------|
| 6자리 vs 4자리 | PRD POL-OUT **4자리** 실측 사용; 6자리는 RED ε 예시 |
| identity 줄 | golden에서 제외 (사용자 예시 2줄/feet·yard만) |
| 실행 파일명 | CMake `OUTPUT_NAME UnitConverter`; 타깃은 `unit_converter_legacy` |
| GM-06 README 문구 | `UnitConverter_test` → 실제 `unit_converter_tests` + `GoldenMaster` |
| GM-08 | 에이전트가 GitHub UI 설정 불가 → README·Report에 수동 절차 |
| 커밋 | 사용자 미요청 — **스테이징만** |

---

## 7. 검증 명령 (세션에서 실행됨)

```powershell
cmake -S . -B build -DUC11_GREEN_PHASE=ON
cmake --build build
.\unit_converter_tests.exe "golden_master_cli_stdout_approval"
ctest -R GoldenMaster --output-on-failure
.\tests\scripts\generate_golden_master.ps1 -BuildDir build
```

```text
GoldenMaster — Passed (~0.25s)
generate_golden_master.ps1 — Wrote tests/golden_master_expected.txt (diff 0)
```

---

## 8. PRD · test_plan · REG 매핑

| ID | Golden Master 대응 |
|----|-------------------|
| **REG-04** | `meter:2.5` E2E → 4시나리오 확장 |
| **T-INT-01** | CLI table stdout golden |
| **I-01 / I-03** | diff 0 |
| **GH-01** | happy path 출력 고정 |
| **test_plan §9.2** | 3줄 예시; identity 제외 2줄로 정합 |
| **REFACTOR 전** | README: "Refactoring 시작 전 구축" |

---

## 9. 미완료 / 후속

| # | 항목 | 담당 |
|---|------|------|
| 1 | `git commit` + `push` (스테이징된 GM 파일) | 사용자 |
| 2 | GM-08 GitHub Branch protection | 저장소 관리자 |
| 3 | CI workflow 첫 Green (원격 push 후) | Actions |
| 4 | REFACTOR 후 `ctest -R GoldenMaster` (REG-05) | REFACTOR PR |
| 5 | [Report/03_GREEN.md](../Report/03_GREEN.md) §11에 `04_GOLDEN_MASTER` 링크 (선택) | 사용자 요청 시 |
| 6 | ERR-* stderr golden (REG-02) | 별도 마일스톤 |
| 7 | JSON/CSV E2E (T-INT-02) | 별도 마일스톤 |

---

## 10. 사용자 규칙·제약 (세션 준수)

- Git **commit** — 사용자 명시 요청 없음 → 스테이징만 (`git add`)
- 최소 scope — Golden Master 관련 파일만
- Catch2 + Given-When-Then 스타일 테스트
- 기존 `UC11_GREEN_PHASE` 가드 유지

---

## 11. 관련 링크 (저장소)

| 문서 | 경로 |
|------|------|
| Golden Master 보고서 | [Report/04_GOLDEN_MASTER.md](../Report/04_GOLDEN_MASTER.md) |
| GREEN 보고서 | [Report/03_GREEN.md](../Report/03_GREEN.md) |
| RED 세션 Prompting | [02_RED.md](02_RED.md) |
| GREEN 세션 Prompting | [03_GREEN.md](03_GREEN.md) |
| test_plan §9 | [docs/test_plan.md](../docs/test_plan.md) |
| PRD REG-04 | [docs/PRD.md](../docs/PRD.md) |

---

## 12. Prompting 파일 안내

| 파일 | 용도 |
|------|------|
| [01_spec.md](01_spec.md) | Phase 4~5 문제정의·설계 |
| [02_RED.md](02_RED.md) | RED/TDD/QA 세션 |
| [03_GREEN.md](03_GREEN.md) | GREEN TDD 세션 |
| **04_GOLDEN_MASTER.md** (본 파일) | Golden Master·CI·Report 세션 (Turn 1~5) |

---

## 13. 대화 턴 타임라인 (전체)

```
Turn 1  User: Golden Master 설계 [P][C][T] + [F]
        Agent: helper, test, expected.txt, ps1/sh, CMake OUTPUT_NAME
               실측 4자리 baseline, identity 제외, PASS

Turn 2  User: README GM 섹션 삽입 (GM-01~09 원문)
        Agent: README + TOC, 체크리스트 [ ]

Turn 3  User: 진행
        Agent: [x] GM-01~07,09, add_test GoldenMaster, workflow yml
               git add, GM-08 절차, ps1 Join-Path fix

Turn 4  User: Report 작성
        Agent: Report/04_GOLDEN_MASTER.md

Turn 5  User: Prompting에 현재 대화 전부 저장
        Agent: Prompting/04_GOLDEN_MASTER.md (본 파일)
```

---

*본 파일은 Cursor 에이전트 세션 Turn 1~5의 사용자 프롬프트·에이전트 산출·검증·미완료를 기록한다. Golden Master 실행: `ctest -R GoldenMaster`. 정본 baseline: [tests/golden_master_expected.txt](../tests/golden_master_expected.txt).*
