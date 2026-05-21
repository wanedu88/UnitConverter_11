# UnitConverter_11 — 테스트 계획서

| 항목 | 내용 |
|------|------|
| **문서 ID** | TP-UC11-001 |
| **버전** | 1.0 |
| **작성 관점** | 시니어 QA 리드 |
| **기술 스택** | C++17, CMake, Catch2 v3.x |
| **정본 참조** | [README.md](../README.md), [docs/PRD.md](PRD.md), [docs/requirement.md](requirement.md) |
| **대표 샘플** | `meter:2.5` → **meter → feet** 변환 (`2.5 meter = 8.2021 feet`) |

---

## 1. 목적 및 범위

### 1.1 목적

본 계획서는 선택 샘플 **`meter → feet` 변환**을 중심으로, 길이 단위 변환 CLI의 **정확도·계약·회귀**를 Catch2 기반으로 검증하기 위한 테스트 범위·우선순위·경계·커버리지 측정 전략을 정의한다.

### 1.2 In Scope

| 영역 | 내용 |
|------|------|
| **Domain (Entity)** | meter 허브 환산, builtin 비율(`3.28084`, `1.09361`), `meter:2.5` golden |
| **Boundary** | 파싱·검증·포맷·stderr·exit code (ERR-* 8종) |
| **Control** | ConvertLengthUseCase 등 유스케이스 조율 |
| **Integration (E2E)** | stdin/stdout/stderr, `meter:2.5` 3줄 table golden |
| **회귀** | REG-01~05 (비율·ERR 전문·POL-OUT·`meter:2.5` E2E) |

### 1.3 Out of Scope (v1.0 계획 기준)

- YAML 설정(F-10)
- 성능·부하·동시성 테스트
- UI/웹 클라이언트

### 1.4 샘플 예제 고정값 (Golden)

| 항목 | 값 |
|------|-----|
| **입력** | `meter:2.5` |
| **비즈니스 규칙** | `1 meter = 3.28084 feet` |
| **feet 행 기대 (table)** | `2.5 meter = 8.2021 feet` |
| **ε (Entity ratio)** | ≤ 1×10⁻⁴ (비율 상수 검증) |
| **반올림 (표시)** | 우측 소수 **4자리 half-up** (PRD §6.1) |
| **요구사항 추적** | requirement.md **1번**(입출력), **4번**(정확도 테스트), PRD **AC-01**, **GH-01** |

---

## 2. 테스트 전략 개요

### 2.1 피라미드

```text
                    ┌─────────────────┐
                    │  Integration E2E │  meter:2.5, GH-01~08
                    ├─────────────────┤
                    │     Control      │  UseCase 100% 호출
                    ├─────────────────┤
                    │    Boundary      │  ERR-* 8종, 파싱·포맷
                    ├─────────────────┤
                    │     Entity       │  ratio golden, POL 불변식
                    └─────────────────┘
```

### 2.2 설계 원칙

| 원칙 | 테스트 적용 |
|------|-------------|
| **계약 우선** | stderr 전문·exit code·POL-OUT 좌측 패턴을 golden 스냅샷으로 고정 |
| **비율 분리** | feet↔yard 직접 비율 금지; meter 경유만 검증 (REG-01) |
| **레이어 격리** | Entity는 iostream 없음; Boundary는 Mock Entity |
| **TDD** | RED → GREEN → refactor; REG-05: refactor 전 전 테스트 GREEN |

### 2.3 Catch2 태그 체계 (권장)

| 태그 | 대상 |
|------|------|
| `[entity]` | UnitConverter, Registry, Quantity, ratio |
| `[boundary][parsing]` | InputParser, InputValidator |
| `[boundary][format]` | OutputFormatter table/json/csv |
| `[control]` | UseCase |
| `[integration]` | E2E CLI |
| `[golden]` | `meter:2.5`, builtin 비율, ERR 스냅샷 |
| `[regression]` | REG-01~05 |

---

## 3. Catch2 단위 테스트 — 범위 및 우선순위

### 3.1 우선순위 정의

| 우선순위 | 기준 | 실행 시점 |
|----------|------|-----------|
| **P0** | 릴리스·CI 게이트; 실패 시 merge 불가 | 매 PR, `ctest` |
| **P1** | 핵심 기능·AC 직결; sprint 내 GREEN 필수 | 일일 빌드 |
| **P2** | 권장 기능(F-07~09), 엣지·리팩터 보조 | 마일스톤 말 |

### 3.2 Entity 레이어 — P0 (Domain ≥ 95%)

**목표:** `meter → feet` 환산식 및 builtin 비율 불변식. **Mock/IO 없음.**

| ID | 테스트명 (Catch2) | 입력/조건 | 기대 | 우선순위 |
|----|-------------------|-----------|------|----------|
| E-01 | `1 meter converts to 3.28084 feet within epsilon` | `Quantity(1, meter)` → feet | `3.28084`, ε ≤ 1e-4 | P0 |
| E-02 | `2.5 meter converts to 8.2021 feet (rounded display rule)` | `Quantity(2.5, meter)` → feet | `8.2021` (half-up 4자리) | P0 |
| E-03 | `meter to feet uses factor_to_meter hub only` | feet factor = 0.3048 | `out = meters / 0.3048` | P0 |
| E-04 | `feet to meter roundtrip preserves value` | `3.28084 feet` → meter → feet | ±ε | P1 |
| E-05 | `feet to yard indirect equals meter-mediated` | feet→yard vs meter 경유 | 동일 ε | P1 |
| E-06 | `zero meter yields zero feet` | `0 meter` → feet | `0.0` | P0 |
| E-07 | `unknown unit throws UnknownUnit` | `parsec` | Domain 예외 | P1 |
| E-08 | `negative quantity rejected at domain (if exposed)` | value &lt; 0 | InvalidQuantity | P2 |

**샘플 예제 직결:** E-01, E-02, E-06이 `meter → feet` 핵심 검증이다.

### 3.3 Boundary 레이어 — P0/P1 (Boundary ≥ 85%, ERR-* 100%)

**목표:** 파싱·검증·stderr·exit. Entity는 **Mock** 또는 Fake Registry.

| ID | 테스트명 | 모듈 | 우선순위 |
|----|----------|------|----------|
| B-01 | `parse meter:2.5` → DTO | InputParser | P0 |
| B-02 | `parse meter2.5` → ERR-FORMAT | InputParser | P0 |
| B-03 | `parse meter:abc` → ERR-VALUE | InputParser/Validator | P0 |
| B-04 | `validate yard:-3` → ERR-NEG | InputValidator | P0 |
| B-05 | `validate meter:0` success | InputValidator | P0 |
| B-06 | `format table feet line POL-OUT` | OutputFormatter | P0 |
| B-07 | `unknown unit parsec:1.0` → ERR-UNIT | CliApp/UseCase | P0 |
| B-08 | `feet:1.2.3` malformed decimal | InputParser | P1 |
| B-09~B-16 | ERR-OUTPUT, ERR-CONFIG, ERR-REGISTER 각 1건 | 해당 Boundary | P1 |

### 3.4 Control 레이어 — P1 (100% 메서드 호출)

| ID | 테스트명 | 기대 | 우선순위 |
|----|----------|------|----------|
| C-01 | `ConvertLengthUseCase invokes converter and formatter` | Mock 호출 검증 | P1 |
| C-02 | `failure path does not call formatter` | stderr만, stdout 빈 | P1 |

### 3.5 Integration — P0

| ID | 테스트명 | 입력 | 기대 | 우선순위 |
|----|----------|------|------|----------|
| I-01 | `T-INT-01 meter:2.5 E2E table golden` | `meter:2.5` | 3줄, feet=`8.2021`, exit 0 | P0 |
| I-02 | `GH-08 meter:0 E2E` | `meter:0` | `0.0 meter = 0.0 feet`, exit 0 | P0 |
| I-03 | `REG-04 diff 0 on full stdout` | `meter:2.5` | 스냅샷 파일과 byte-equal | P0 |

### 3.6 우선순위 실행 순서 (권장 구현 순)

```text
1. E-01, E-02, E-06          (Domain ratio + sample + zero)
2. B-01, B-03, B-02, B-04    (parse/validate contracts)
3. I-01                       (E2E golden)
4. B-06, B-07                 (POL-OUT + unknown unit)
5. E-04~E-05, B-08, C-*, F-07~09 integration
```

---

## 4. 경계값 케이스 목록

아래는 **요청된 경계 시나리오**를 테스트 ID·입력·기대·레이어·우선순위로 구체화한 것이다.

### 4.1 변환 값 (value) 경계

| ID | 시나리오 | 입력 | 기대 (feet 행 또는 동등) | 레이어 | P |
|----|----------|------|--------------------------|--------|---|
| BV-01 | **영값 변환** | `meter:0` | `0.0 meter = 0.0 feet`; exit 0; stderr 빈 | E-06, I-02 | P0 |
| BV-02 | **매우 큰 수 (오버플로 위험)** | `meter:1e100` | 파싱 성공 시: `∞`/`inf` 표기 **금지**; 또는 사전 `InvalidQuantity`/`value too large` 정책 문서화 후 일관 적용 | E, I | P1 |
| BV-03 | **매우 큰 수 — 파싱 한계** | `meter:1e309` (또는 `1e400`) | `Invalid number: …` (stod 실패) **또는** 거부 정책 1종으로 고정 | B | P1 |
| BV-04 | **음수 입력** | `yard:-3` | stderr `Negative value not allowed: -3`; exit 1; **stdout 변환 줄 없음** | B, I | P0 |
| BV-05 | **음수 0 형태** | `meter:-0.0` | POL-NEG-02: 거부 권장; stderr에 `-0` 또는 `-0.0` 표기 **PRD 고정 후** golden | B | P1 |
| BV-06 | **최소 양수** | `meter:1e-10` | `0.0000 feet` (4자리 half-up) 또는 ε 정책 문서화 | E | P2 |

**BV-02 측정 메모:** `2.5 × 3.28084`는 안전하나, `value × factor`에서 `double` 오버플로 가능. QA는 **명시 상한**을 제품에 합의한다 (예: `|value| ≤ 1e15` → 초과 시 `InvalidQuantity`).

### 4.2 입력 형식·파싱 경계

| ID | 시나리오 | 입력 | stderr (golden) | exit | stdout | P |
|----|----------|------|-----------------|------|--------|---|
| BV-07 | **소수점 파싱 실패** | `meter:abc` | `Invalid number: abc` | 1 | 빈 | P0 |
| BV-08 | **콜론 없음** | `meter2.5` | `Invalid format. Use unit:value (ex: meter:2.5)` | 1 | 빈 | P0 |
| BV-09 | **다중 소수점** | `feet:1.2.3` | `Invalid number: 1.2.3` | 1 | 빈 | P1 |
| BV-10 | **빈 value** | `meter:` | `Invalid number: ` (빈 토큰) | 1 | 빈 | P1 |
| BV-11 | **빈 unit** | `:2.5` | FORMAT 또는 UNIT 정책 1종 | 1 | 빈 | P2 |

### 4.3 단위·Registry 경계

| ID | 시나리오 | 입력 | stderr | exit | stdout | P |
|----|----------|------|--------|------|--------|---|
| BV-12 | **없는 단위** | `parsec:1.0` | `Unknown unit: parsec` | 1 | 빈 | P0 |
| BV-13 | **PRD 표준 미지원** | `lightyear:1` | `Unknown unit: lightyear` | 1 | 빈 | P1 |
| BV-14 | **대소문자** | `Meter:2.5` | 정책 고정: normalize → `meter` 또는 거부 | — | P2 |

### 4.4 샘플 예제 회귀 (meter → feet)

| ID | 입력 | feet 행 golden | 비고 |
|----|------|----------------|------|
| BV-G01 | `meter:2.5` | `2.5 meter = 8.2021 feet` | REG-04, AC-01 |
| BV-G02 | `meter:1` (Entity only) | `3.28084` (ε) | REG-01 |
| BV-G03 | `feet:3.28084` | 좌측 `3.28084 feet`; meter 환산 후 feet ≈ 3.28084 | GH-06, POL-OUT |

---

## 5. 예외·특이 케이스 목록

### 5.1 ERR-* 계약 (8종 — 각 ≥1 테스트, stderr diff 0)

| 코드 | 조건 예시 | stderr 패턴 | exit |
|------|-----------|---------------|------|
| ERR-FORMAT-001 | `meter2.5` | `Invalid format. Use unit:value (ex: meter:2.5)` | 1 |
| ERR-VALUE-002 | `meter:abc`, `feet:1.2.3` | `Invalid number: {token}` | 1 |
| ERR-VALUE-003 | `yard:-3` | `Negative value not allowed: {value}` | 1 |
| ERR-UNIT-004 | `parsec:1.0` | `Unknown unit: {unit}` | 1 |
| ERR-UNIT-005 | `123unit:1` (정책 시) | `Invalid unit name: {unit}` | 1 |
| ERR-OUTPUT-006 | `--format xml` | `Unknown output format: {format}` | 1 |
| ERR-CONFIG-007 | config 누락/깨짐 | `Config load failed: {reason}` | 1 |
| ERR-REGISTER-008 | 중복 register | `Unit registration failed: {reason}` | 1 |

### 5.2 Domain 예외 → Boundary 매핑

| Domain 예외 | Boundary 처리 | 테스트 |
|-------------|---------------|--------|
| UnknownUnit | ERR-UNIT-004 | BV-12 |
| InvalidQuantity | ERR-VALUE-003 | BV-04 |
| DuplicateUnit / InvalidRatio | ERR-REGISTER-008 | P1 |
| EmptyRegistry | ERR-CONFIG-007 | P1 |

### 5.3 특이·비기능 케이스

| ID | 케이스 | 기대 |
|----|--------|------|
| EX-01 | 성공 시 stderr **완전 빈 문자열** | `meter:2.5` |
| EX-02 | POL-OUT: 모든 줄 좌측 `2.5 meter` 동일 | 3줄 table |
| EX-03 | 출력 순서 lex: `feet`, `meter`, `yard` | I-01 |
| EX-04 | baseline `catch(...)` → 명시 ERR-VALUE-002 매핑 | 리팩터 후 금지 |
| EX-05 | feet↔yard **독립 비율 코드 없음** | 정적 검토 + E-05 |
| EX-06 | cubit 등록 후 `meter:2.5` golden **불변** | AC-05 |
| EX-07 | table/json/csv **동일 rounded value** | AC-07 |
| EX-08 | NaN/Inf 입력 | `Invalid number` 또는 InvalidQuantity — **1정책** | P2 |

---

## 6. Gherkin ↔ 테스트 매핑

| Gherkin | AC | 대표 테스트 ID |
|---------|-----|----------------|
| GH-01 Happy `meter:2.5` | AC-01 | I-01, E-02, B-06 |
| GH-02 No colon | AC-02 | BV-08, B-02 |
| GH-03 `meter:abc` | AC-02 | BV-07, B-03 |
| GH-04 `feet:1.2.3` | AC-02 | BV-09 |
| GH-05 `yard:-3` | AC-03 | BV-04 |
| GH-06 Feet input | AC-01 | BV-G03 |
| GH-07 Unknown | AC-04 | BV-12 |
| GH-08 `meter:0` | AC-01 | BV-01, I-02 |

---

## 7. 커버리지 목표

### 7.1 레이어별 목표 (PRD §4.3)

| 레이어 | 라인 커버리지 | 오류 경로 | 게이트 |
|--------|---------------|-----------|--------|
| **Entity (Domain)** | **≥ 95%** | InvalidQuantity, UnknownUnit, DuplicateUnit, EmptyRegistry | CI 실패 |
| **Boundary** | **≥ 85%** | ERR-* 8종 **코드별 100%** (≥1 테스트/코드) | CI 실패 |
| **Data** | ≥ 90% | config 실패·factor≤0 | Milestone |
| **Control** | **100%** | 유스케이스 public 메서드 각 ≥1 호출 | CI 실패 |

### 7.2 샘플 예제 기준 최소 커버 파일

| 파일 (목표 구조) | 필수 커버 함수/분기 |
|------------------|---------------------|
| `src/entity/UnitConverter.cpp` | `convert`, meter 정규화, feet target |
| `src/entity/UnitRegistry.cpp` | builtin 3단위 lookup |
| `src/boundary/InputParser.cpp` | `:` 분리, stod 실패 |
| `src/boundary/InputValidator.cpp` | POL-NEG |
| `src/boundary/OutputFormatter.cpp` | 4자리 half-up, POL-OUT |

### 7.3 AC-08 인수 기준

- Entity ≥ 95% **AND** Boundary ≥ 85% **AND** ERR-* 8종 GREEN → **AC-08 충족**

---

## 8. gcov / lcov 측정 전략

### 8.1 단계별 대상

| 단계 | 측정 대상 | 목적 |
|------|-----------|------|
| **Phase A (현재)** | `UnitConverter.cpp` (baseline 단일 파일) | 학습·회귀 기준선; 파싱·3단위 분기 커버 |
| **Phase B (목표)** | `src/entity/*`, `src/boundary/*`, `src/control/*` | PRD 레이어별 95%/85% 게이트 |

### 8.2 CMake + gcov 설정 (권장)

```cmake
# 커버리지 빌드 예 (프로젝트 CMakeLists에 추가)
option(UC11_COVERAGE "Enable gcov coverage" OFF)
if(UC11_COVERAGE)
  add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
  add_link_options(--coverage)
endif()
```

```bash
# Debug + Coverage 빌드
cmake -B build-cov -DCMAKE_BUILD_TYPE=Debug -DUC11_COVERAGE=ON
cmake --build build-cov
ctest --test-dir build-cov --output-on-failure

# .gcda 생성을 위해 테스트 바이너리 실행 필수
./build-cov/tests/unit_converter_tests
```

### 8.3 gcov / lcov 실행 절차

```bash
# 1) gcov — 파일별 실행 카운트 (Phase A: 루트에서)
cd build-cov
gcov -b -c ../UnitConverter.cpp   # 또는 객체 경로에 맞게 조정

# 2) lcov — HTML 리포트 (권장 CI 아티팩트)
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/catch2/*' '*/tests/*' --output-file coverage.filtered.info
genhtml coverage.filtered.info --output-directory coverage-html

# 3) 임계값 검사 (예: lcov --summary + 스크립트)
#    Entity 라인 ≥ 95%, Boundary ≥ 85%
```

### 8.4 `UnitConverter.cpp` 전용 측정 매트릭스 (Phase A)

| 분기/라인 | 트리거 테스트 | BV/ID |
|-----------|---------------|-------|
| `find(':') == npos` | `meter2.5` | BV-08 |
| `stod` catch | `meter:abc` | BV-07 |
| `unit == meter` | `meter:2.5` | I-01 |
| `unit == feet` | `feet:3.28084` | BV-G03 |
| `unit == yard` | `yard:0` | P1 |
| `else` unknown | `parsec:1.0` | BV-12 |
| `inFeet = meterValue * 3.28084` | `meter:2.5` | E-02 / I-01 |

**Gap (baseline):** 음수 검증·4자리 반올림·lex 순서 없음 → Phase B에서 Boundary/Entity 테스트로 **의도적 RED 후 구현**.

### 8.5 레이어별 lcov 필터 (Phase B)

```bash
# Entity만
lcov --extract coverage.filtered.info '*/src/entity/*' -o entity.info
# Boundary만
lcov --extract coverage.filtered.info '*/src/boundary/*' -o boundary.info
```

| 리포트 | 경로 필터 | 목표 |
|--------|-----------|------|
| `entity.info` | `src/entity/` | line ≥ 95% |
| `boundary.info` | `src/boundary/` | line ≥ 85% |

### 8.6 CI 통합 (권장)

| Job | 단계 |
|-----|------|
| `test` | `ctest` + Catch2 tags `[golden]`, `[entity]`, `[boundary]` |
| `coverage` | `-DUC11_COVERAGE=ON` → lcov → `genhtml` → artifact upload |
| `gate` | 스크립트로 `entity` ≥ 95%, `boundary` ≥ 85% 미달 시 exit 1 |

### 8.7 측정 시 주의

| 항목 | 지침 |
|------|------|
| **최적화** | Coverage 빌드는 `-O0` 또는 `-Og`; `-O3`는 분기 왜곡 |
| **inline** | header-only 구현은 해당 TU 테스트로 실행 보장 |
| **Mock** | Boundary Mock 테스트는 **Boundary .cpp** 커버; Entity는 Fake Registry로 **실제 환산 코드** 실행 |
| **E2E** | Integration은 Boundary+Control+Entity 연쇄 커버에 기여하나, **Entity 95%는 Entity 단위 테스트로 충족** |

---

## 9. 테스트 데이터·아티팩트

### 9.1 Golden 파일 (권장 경로)

```text
tests/golden/
  meter_2_5_table.txt      # I-01 stdout
  stderr/
    err_format_meter2_5.txt
    err_value_meter_abc.txt
    err_neg_yard_-3.txt
    err_unit_parsec.txt
```

### 9.2 `meter:2.5` 전체 stdout golden

```text
2.5 meter = 2.5 meter
2.5 meter = 8.2021 feet
2.5 meter = 2.7340 yard
```

---

## 10. 일정·완료 기준

| 마일스톤 | 완료 조건 |
|----------|-----------|
| **M1 — Domain RED→GREEN** | E-01, E-02, E-06 GREEN; Entity lcov ≥ 95% |
| **M2 — Boundary 계약** | BV-01, 04, 07, 08, 12 GREEN; ERR 8/8; Boundary ≥ 85% |
| **M3 — E2E** | I-01, I-02 GREEN; REG-04 diff 0 |
| **M4 — 인수** | AC-01~08, GH-01~08 매핑 테스트 전부 GREEN |

---

## 11. 리스크 및 가정

| 리스크 | 완화 |
|--------|------|
| baseline에 음수·반올림 없음 | 테스트 먼저 RED; PRD 계약으로 구현 |
| `BV-02` 큰 수 정책 미정 | PO/아키와 상한·Inf 처리 1종 합의 후 golden 고정 |
| Windows vs Linux gcov 경로 | CI는 Linux 기준; 로컬은 WSL 또는 동일 도구체인 |
| README `8.2` vs PRD `8.2021` | **테스트는 PRD 4자리 half-up** (REG-04) |

---

## 12. 참조

| 문서 | 용도 |
|------|------|
| [README.md](../README.md) | 비율·POL-OUT·POL-NEG·예시 입출력 |
| [PRD.md](PRD.md) | AC-01~08, §4.3 커버리지, ERR 계약 |
| [TODO.md](TODO.md) | 마일스톤·회귀 체크리스트 |
| [requirement.md](requirement.md) | 기본 요구사항 1·4번 |

---

*문서 버전 1.0 — 구현 코드 없음. 테스트 코드 작성 시 본 계획서 ID(E-xx, BV-xx, I-xx)를 Catch2 `SECTION` 또는 태그에 매핑할 것.*
