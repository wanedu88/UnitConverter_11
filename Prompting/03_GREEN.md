# UnitConverter_11 — 대화 세션 기록 (GREEN / TDD / QA)
_Exported on 2026-05-21 from Cursor agent session (UnitConverter_11 workspace)_

**워크스페이스:** `c:\DEV\UnitConverter_11`  
**선행 문서:** [02_RED.md](02_RED.md) (RED 48/48 FAIL · Dual-Track 스켈레톤)  
**본 세션 주제:** TDD GREEN 커밋 순서 · TC-A/B 14건 · ctest 71/71 · lcov 게이트 · legacy 위임 · PR A_01 · Report  
**최종 갱신:** 2026-05-21

---

## 세션 요약

| 단계 | 사용자 요청 (요지) | 산출물 | 비고 |
|------|-------------------|--------|------|
| 1 | **TC-B-01 GREEN만** (REFACTOR 금지) | `test_converter.cpp`, `UnitConverter` meter→feet | `a09a333` |
| 2 | **커밋 순서표** 기억 (9단계) | 표 확정 | A/B 교차 |
| 3 | 커밋표 **2번** TC-A-02 | `test_parser.cpp`, colon 검증 | `b477af1` |
| 4 | 커밋표 **3~9번 반복** | 7개 feat + 1 chore | `d6c163c`~`aeaf7c0` |
| 5 | 전체 TC PASS 후 ctest·lcov·체크리스트 | 71/71, Entity 95.5%, Boundary 87.1% | 게이트 충족 |
| 6 | green → **A_01 PR** | `git push origin A_01` | gh CLI 미설치 |
| 7 | **Report/03_GREEN.md** | GREEN 단계 보고서 | |
| 8 | **Prompting 저장** | 본 파일 | 현재 턴 |

---

## 1. 커밋 순서표 (사용자 확정 · 세션 전반 고정)

| 순서 | Track | 대상 TC | 구현 내용 | 커밋 메시지 |
|------|-------|---------|-----------|-------------|
| 1 | B | TC-B-01 | `convert("meter",v,"feet")` 최소 구현 | `feat(green): meter to feet` |
| 2 | A | TC-A-02 | `":"` 없는 입력 → 예외 | `feat(green): validate missing colon` |
| 3 | B | TC-B-02 | `convert("meter",v,"yard")` | `feat(green): meter to yard` |
| 4 | A | TC-A-03 | 음수 입력 → 예외 | `feat(green): validate negative value` |
| 5 | B | TC-B-03 | `convert("feet",v,"meter")` 역변환 | `feat(green): feet to meter reverse` |
| 6 | A | TC-A-04 | 없는 단위 → 예외 | `feat(green): validate unknown unit` |
| 7 | B | TC-B-04~05 | `convertAll()` + `registerUnit()` | `feat(green): convertAll and registerUnit` |
| 8 | A | TC-A-01,06,07 | Happy path + 포맷 + 경계값 | `feat(green): boundary happy path` |
| 9 | B | TC-B-06~07 | `loadConfig()` 정상·실패 | `feat(green): loadConfig with fallback` |
| — | A+B | 게이트·완료 | TC-A-05, RED 교체, legacy, lcov | `chore(green): complete TC suite...` |

**원칙:** 한 커밋 = 한 TC 묶음 · REFACTOR 금지 · 비율 `kMeterToFeet`/`kMeterToYard`만 사용.

---

## 2. 사용자 프롬프트 원문 요약

### Turn 1 — TC-B-01 GREEN
```
[단계] TDD GREEN만, REFACTOR 금지
대상: TC-B-01, tests/test_converter.cpp
  REQUIRE(convert("meter", 2.5, "feet") == Approx(8.20210).epsilon(1e-5))
비율 상수 추출, 3.28084 인라인 금지
Step 1~5: cmake FAIL 확인 → 최소 구현 → PASS → 커밋 메시지 제안
```

### Turn 2 — 커밋 순서표 기억
```
9단계 Track A/B 교차 커밋표 — 이후 작업 시 참조
```

### Turn 3 — 동일 행동 1개 묶음 (에이전트 워크플로)
```
Step 1) 대상 TC 확인
Step 2) cmake build → FAIL 확인
Step 3) 대상 TC만 최소 구현
Step 4) rebuild → PASS, 나머지 영향 없음
Step 5) 커밋
출력: 목표 / 코드 범위 / PASS·FAIL / 커밋 메시지 / 다음 TC
→ 2번째: Track A, TC-A-02
```

### Turn 4 — 커밋표 마지막까지 반복
```
3~9번 일괄 수행 (meter to yard … loadConfig with fallback)
```

### Turn 5 — 전체 검증 + PR
```
모든 TC-A-01~07, TC-B-01~07 PASS 후:
  cmake && ctest -V
  gcov/lcov, Domain ≥95%, Boundary ≥85%
  비율 인라인 없음, main() 변환 로직 없음
이후: green → A_01 PR → 리뷰 → 머지
```

### Turn 6 — Report 작성
```
Report 디렉토리에 보고서 → Report/03_GREEN.md
```

### Turn 7 — Prompting 저장
```
Prompting에 대화 내용 저장 → 본 파일
```

---

## 3. 에이전트 수행 결과 (산출물·결정)

### 3.1 Turn 1 — TC-B-01

| Step | 결과 |
|------|------|
| FAIL | stub `return 0.0` → `0.0 == Approx(8.2021)` FAIL |
| 구현 | `if (meter && feet) return value * kMeterToFeet` |
| PASS | `[TC-B-01]` 1 assertion |
| 커밋 | `feat(green): meter to feet` (`a09a333`) |

**파일:** `tests/test_converter.cpp`, `src/entity/UnitConverter.cpp`, `CMakeLists.txt` (red_tests + `uc11_entity` 링크)

### 3.2 Turn 3 — TC-A-02

| Step | 결과 |
|------|------|
| FAIL | `InputParser` stub — 예외 미발생 |
| 구현 | 기존 `parseInputLine` colon 검증 (이미 존재) |
| PASS | `[TC-A-02]` |
| 커밋 | `feat(green): validate missing colon` (`b477af1`) |

**파일:** `tests/test_parser.cpp`, `uc11_boundary` 링크

### 3.3 Turn 4 — 커밋 3~9

| # | 커밋 | 핵심 diff |
|---|------|-----------|
| 3 | `meter to yard` | `meter→yard` 분기 + TC-B-02 |
| 4 | `validate negative value` | `test_parser` TC-A-03 (`InputValidator` 기존) |
| 5 | `feet to meter reverse` | `feet→meter` × `kFeetFactorToMeter` |
| 6 | `validate unknown unit` | `registry.hasUnit` + TC-A-04 |
| 7 | `convertAll and registerUnit` | **허브 공식** 전환 + TC-B-04/05 |
| 8 | `boundary happy path` | `test_boundary.cpp` A-01,06,07 |
| 9 | `loadConfig with fallback` | `test_config.cpp` B-06,07 |

### 3.4 Turn 5 — 게이트 완료 (`aeaf7c0`)

| 항목 | 조치 |
|------|------|
| TC-A-05 | `meter:abc`, `:2.5`, `feet:1.2.3`, NaN 검증 추가 |
| RED 12건 | `FAIL("RED")` → 실제 assertion |
| `formatJson()` | `OutputFormatter` 신규 |
| legacy | `UnitConverter.cpp` → Boundary/Entity 위임 |
| `UC11_GREEN_PHASE` | 기본 **ON** |
| `UC11_COVERAGE` | per-target `uc11_enable_coverage()` |
| lcov 보강 | fixtures: `empty_units.json`, `malformed_name.json`, `missing_factor_field.json` |

**ctest:** `71/71 PASS` (36 `unit_converter_tests` + 35 `unit_converter_red_tests`)

**lcov:**

| 레이어 | 실측 |
|--------|------|
| Entity | **95.5%** (42/44) |
| Boundary | **87.1%** (121/139) |

### 3.5 Turn 5 — PR

```text
branch A_01 pushed → origin/A_01
PR URL: https://github.com/wanedu88/UnitConverter_11/pull/new/A_01
gh CLI: 미설치 → 수동 PR 생성 안내
```

---

## 4. 코드 구조 (GREEN 완료 시점)

```text
src/entity/
  ConversionConstants.hpp   # kMeterToFeet, kMeterToYard, kFeetFactorToMeter
  UnitRegistry.hpp/cpp
  UnitConverter.hpp/cpp     # hub: meters = v * factor; out = m / factor

src/boundary/
  InputParser.hpp/cpp
  InputValidator.hpp/cpp
  ConfigLoader.hpp/cpp
  OutputFormatter.hpp/cpp   # formatTableLine, formatJson (신규)

tests/
  test_converter.cpp        # TC-B-01~05
  test_parser.cpp           # TC-A-02~05
  test_boundary.cpp         # TC-A-01,06,07
  test_config.cpp           # TC-B-06,07 (+ 보강)
  test_*.cpp (6)            # 36건, TestPhase 가드
  red_track_*.cpp           # 12건, 실 assertion

UnitConverter.cpp (legacy)  # parse → validate → convertAll → formatTableLine
```

### Domain 환산 (최종)

```cpp
const double meters = value * registry.factorToMeter(fromUnit);
return meters / registry.factorToMeter(toUnit);
```

---

## 5. 테스트·빌드 명령 (재현)

```powershell
# GREEN 기본
cmake -S . -B build -DUC11_GREEN_PHASE=ON
cmake --build build
ctest --test-dir build -V

# TC 14건 스팟
.\build\unit_converter_red_tests.exe "[TC-A-01]"
# … [TC-B-07]

# 커버리지
cmake -S . -B build -DUC11_COVERAGE=ON -DUC11_GREEN_PHASE=ON
cmake --build build
ctest --test-dir build
lcov --capture --directory build --output-file build/coverage.info
lcov --remove build/coverage.info "*/catch2/*" "*/tests/*" "*/build/_deps/*" "/usr/*" `
     --output-file build/coverage.filtered.info
lcov --extract build/coverage.filtered.info "*/src/entity/*" -o build/entity.info
lcov --extract build/coverage.filtered.info "*/src/boundary/*" -o build/boundary.info
lcov --summary build/entity.info
lcov --summary build/boundary.info
```

---

## 6. Track A/B TC ↔ 파일 매핑

| TC ID | 태그 | 테스트 파일 | RED ID |
|-------|------|-------------|--------|
| TC-A-01 | `[TC-A-01]` | `test_boundary.cpp` | RED-A-01 |
| TC-A-02 | `[TC-A-02]` | `test_parser.cpp` | RED-A-02 |
| TC-A-03 | `[TC-A-03]` | `test_parser.cpp` | RED-A-03 |
| TC-A-04 | `[TC-A-04]` | `test_parser.cpp` | RED-A-04 |
| TC-A-05 | `[TC-A-05]` | `test_parser.cpp` | (파싱 실패) |
| TC-A-06 | `[TC-A-06]` | `test_boundary.cpp` | RED-A-05 |
| TC-A-07 | `[TC-A-07]` | `test_boundary.cpp` | (경계 0) |
| TC-B-01 | `[TC-B-01]` | `test_converter.cpp` | RED-B-01 |
| TC-B-02 | `[TC-B-02]` | `test_converter.cpp` | RED-B-02 |
| TC-B-03 | `[TC-B-03]` | `test_converter.cpp` | RED-B-03 |
| TC-B-04 | `[TC-B-04]` | `test_converter.cpp` | RED-B-03 |
| TC-B-05 | `[TC-B-05]` | `test_converter.cpp` | RED-B-04 |
| TC-B-06 | `[TC-B-06]` | `test_config.cpp` | RED-B-05 |
| TC-B-07 | `[TC-B-07]` | `test_config.cpp` | RED-B-06 |

---

## 7. RED → GREEN 전환 요지

| 항목 | RED ([02_RED.md](02_RED.md)) | GREEN (본 세션) |
|------|-------------------------------|-----------------|
| `UC11_GREEN_PHASE` | OFF | **ON** |
| ctest | 48/48 FAIL | **71/71 PASS** |
| `red_track` | `FAIL("RED")` | 실제 REQUIRE |
| `convert()` | 없음 / 분기 only | meter 허브 |
| legacy main | if-else + 인라인 비율 | API 위임 |
| JSON | 미구현 | `formatJson()` |

---

## 8. 결함 갱신 (GREEN 후)

| ID | GREEN 후 |
|----|----------|
| DEF-001 | **Fixed** |
| DEF-002 | **Fixed** |
| DEF-003 | **Fixed** (Boundary + legacy 위임) |
| DEF-004 | **Fixed** (formatTableLine) |
| DEF-005 | **Fixed** |
| DEF-009 | **Fixed** (InputParser FORMAT) |

Open (REFACTOR/legacy): DEF-010 `catch(...)`, DEF-013 단위 trim 등 — [defect_list.md](../docs/defect_list.md) 참조.

---

## 9. 체크리스트 (사용자 요청 · Turn 5)

| # | 항목 | 결과 |
|---|------|------|
| 1 | 모든 TC PASS | ✓ 14/14 |
| 2 | ctest 0 failures | ✓ 71/71 |
| 3 | Domain ≥ 95% | ✓ 95.5% |
| 4 | Boundary ≥ 85% | ✓ 87.1% |
| 5 | 비율 인라인 없음 (`src/`, legacy) | ✓ |
| 6 | `main()` 변환 로직 없음 | ✓ Domain 위임 |

---

## 10. Git 이력 (green / A_01)

```text
aeaf7c0 chore(green): complete TC suite, coverage gates, and legacy domain delegation
71dbc01 feat(green): loadConfig with fallback
6e5fc88 feat(green): boundary happy path
dead2f3 feat(green): convertAll and registerUnit
8be6f20 feat(green): validate unknown unit
7a917e4 feat(green): feet to meter reverse
ad54624 feat(green): validate negative value
d6c163c feat(green): meter to yard
b477af1 feat(green): validate missing colon
a09a333 feat(green): meter to feet
```

**브랜치:** `A_01` (PR 대상) · 선행 `green` 동일 커밋

---

## 11. 미완료 / REFACTOR 진입 시

1. **PR `A_01` 머지** (`main` 반영)  
2. README RED To-Do 14 TC `[x]` 갱신  
3. CI lcov gate 스크립트 (`test_plan.md` §8.6)  
4. REFACTOR: Control 레이어, `ConfigLoader` 분기 정리, legacy E2E  
5. [Report/04_REFACTOR.md](../Report/04_REFACTOR.md) (예정)

**금지:** 테스트 완화만으로 GREEN · REFACTOR 중 TC 회귀 무시.

---

## 12. 사용자 규칙·제약 (세션 준수)

- Git commit — 사용자 요청·커밋표 단계 시만 (본 세션 10커밋 수행)  
- GREEN만, REFACTOR 금지 (커밋 1~9)  
- 비율 인라인 금지 → `ConversionConstants.hpp`  
- 최소 scope · Given-When-Then · Conventional Commit  

---

## 13. 관련 링크 (저장소)

| 문서 | 경로 |
|------|------|
| RED 세션 | [02_RED.md](02_RED.md) |
| Spec 세션 | [01_spec.md](01_spec.md) |
| RED 보고서 | [Report/02_RED.md](../Report/02_RED.md) |
| **GREEN 보고서** | [Report/03_GREEN.md](../Report/03_GREEN.md) |
| RED 명세 | [docs/red_tests.md](../docs/red_tests.md) |
| 결함 목록 | [docs/defect_list.md](../docs/defect_list.md) |

---

## 14. Prompting 파일 안내

| 파일 | 용도 |
|------|------|
| [01_spec.md](01_spec.md) | Phase 4~5 문제정의·설계 |
| [02_RED.md](02_RED.md) | RED/TDD Turn 1~14 |
| **03_GREEN.md** (본 파일) | GREEN TDD Turn 1~8, 커밋표, ctest/lcov, PR |

---

*본 파일은 Cursor 에이전트 세션 Turn 1~8( TC-B-01 → 커밋표 9단 → 게이트 → Report → Prompting )의 요청·응답·산출물 요약이다. 정본 인수: [Report/03_GREEN.md](../Report/03_GREEN.md). RED 재현: [02_RED.md](02_RED.md) · `UC11_GREEN_PHASE=OFF`.*
