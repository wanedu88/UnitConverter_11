# Dual-Track RED Tests — UnitConverter_11

| 문서 | RED 명세 (구현·GREEN·REFACTOR 금지) |
| 프로젝트 | `c:\DEV\UnitConverter_11` |
| 대상 코드 상태 | `UnitConverter.cpp` 단일 `main()`, if-else 체인, `std::invalid_argument` 없음 |
| 정본 계약 | README, PRD §3.2, `docs/test_plan.md` |
| 비율 | **1 meter = 3.28084 feet** · **1 meter = 1.09361 yard** (meter 허브) |

**RED 판정:** 아래 테스트는 현재 baseline에서 **컴파일 실패**(API 부재) 또는 **런타임 실패**(예외 미발생·수치 0·포맷 불일치)가 정상이다.

---

# UI RED Tests — Test ID / Given/When/Then / Invariant

## Track A-01 — 정상 입력 Happy Path

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-A-01` / Catch2: `test_parse_meter_25_returns_conversion_result` |
| **Given** | 입력 문자열 `"meter:2.5"`; 비율 **1 meter = 3.28084 feet**, **1 meter = 1.09361 yard**; Boundary 파서·Domain 환산 API 존재(RED 시 **부재**) |
| **When** | `parseInputLine("meter:2.5")` → `validateInput` → `convert(registry, "meter", 2.5, "feet")` (또는 동등 UseCase) |
| **Then** | 예외 없음; feet 환산값 **8.202100** (ε ≤ **1e-5**); yard **2.734025** (ε ≤ 1e-5) |
| **보호 계약** | F-01 성공 DTO; F-02·F-03 환산; AC-01 / GH-01 |
| **Invariant** | `convert(meter, v, feet) = v × 3.28084` |
| **RED 실패 예상** | baseline: 파서/convert API 없음 → **링크 실패**; 있어도 `main()`만으로는 테스트 격리 불가 |

---

## Track A-02 — 콜론 없음 (잘못된 형식)

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-A-02` / `test_parse_no_colon_throws_invalid_argument` |
| **Given** | 입력 `"meter2.5"` (콜론 0개) |
| **When** | `parseInputLine("meter2.5")` |
| **Then** | `std::invalid_argument` 발생; `what()`에 `Invalid format. Use unit:value (ex: meter:2.5)` 포함 |
| **보호 계약** | F-01 ERR-FORMAT-001; GH-02; exit 1 (CLI 시) |
| **Invariant** | 형식 불일치 시 **환산 수행 0회** (stdout 변환 줄 없음) |
| **RED 실패 예상** | baseline: 예외 없이 `unit="meter2.5"` 분기 또는 **Unknown unit** / 엉뚱한 `stod` 경로 |

---

## Track A-03 — 음수 값

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-A-03` / `test_validate_meter_neg10_throws_invalid_argument` |
| **Given** | 입력 `"meter:-1.0"`; POL-NEG: `value >= 0`, 0 허용 |
| **When** | `parseInputLine` → `validateInput(parsed)` |
| **Then** | `std::invalid_argument`; `what()`에 `Negative value not allowed: -1` (또는 `-1.0`) |
| **보호 계약** | POL-NEG-01~04; F-05; GH-05; AC-03 |
| **Invariant** | 음수 입력 시 **어떤 단위로도 변환 결과 반환 금지** |
| **RED 실패 예상** | baseline: `stod` 성공 후 **음수 그대로 출력** (예외 없음, exit 0) |

---

## Track A-04 — 없는 단위

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-A-04` / `test_convert_parsec10_throws_unknown_unit` |
| **Given** | 입력 `"parsec:1.0"`; Registry에 `parsec` 없음 |
| **When** | 파싱·검증 성공 후 `convert(registry, "parsec", 1.0, "feet")` |
| **Then** | `std::invalid_argument`; `what()`에 `Unknown unit: parsec` |
| **보호 계약** | F-02 실패; ERR-UNIT-004; GH-07; AC-04 |
| **Invariant** | 미등록 단위는 **stderr/예외만**, stdout 변환 줄 없음 |
| **RED 실패 예상** | baseline: `main()`은 stderr 후 return 1 (**일치**) — Catch2는 **예외 API** 없어 RED-A-01~03과 동일하게 API 부재 가능 |

---

## Track A-05 — POL-OUT 좌측 보존

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-A-05` / `test_format_table_preserves_input_meter_25` |
| **Given** | 입력 unit=`meter`, value=`2.5`; **1 meter = 3.28084 feet** |
| **When** | `formatTableLine(2.5, "meter", convert(..., "feet"), "feet")` |
| **Then** | 문자열에 **`2.5 meter`** 좌측 보존; 우측 `= 8.2021 feet` (4자리 half-up) 또는 계약상 `8.202100` |
| **보호 계약** | POL-OUT-01~03; F-04; REG-03 |
| **Invariant** | 모든 출력 줄 좌측 = **사용자 입력 value·unit 원문** (trim/반올림 적용 안 함) |
| **RED 실패 예상** | baseline: `iostream` 직출력만 존재, `formatTableLine` **부재**; raw double로 `8.2021`/`8.202099` drift |

---

## Track A-06 — JSON 출력 스키마

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-A-06` / `test_format_json_meter_25_valid_schema` |
| **Given** | 입력 `"meter:2.5"`; `--format json` 또는 `formatJson(input, conversions)` |
| **When** | JSON 직렬화 실행 |
| **Then** | 최상위 `input: { "unit": "meter", "value": 2.5 }`; `conversions[]` 길이 = Registry 단위 수; 항목 `{ "unit", "value" }`; feet `value` ≈ **8.2021** (table과 동일 반올림) |
| **보호 계약** | F-07; §6.2; AC-07 (포맷 parity) |
| **Invariant** | JSON 수치 집합 = table half-up 4자리 집합 (ε=0 on rounded) |
| **RED 실패 예상** | baseline: JSON 포맷터 **미구현** → 컴파일/링크 실패 |

---

# Logic RED Tests — Test ID / Scenario / Invariant

## Track B-01 — meter → feet 정확도

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-B-01` / `test_convert_meter_to_feet_within_1e5` |
| **Scenario** | `convert("meter", 2.5, "feet")` — **1 meter = 3.28084 feet** |
| **Then (실패 조건)** | `abs(result - 8.202100) > 1e-5` → RED |
| **Invariant** | `convert(m,v,ft) = v * 3.28084`; feet↔feet 직접 비율 하드코딩 **금지** (meter 허브만) |
| **RED 실패 예상** | `meter` 분기 누락 시 **result = 0.0**; API 없으면 링크 실패 |

---

## Track B-02 — meter → yard 정확도

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-B-02` / `test_convert_meter_to_yard_ratio_109361` |
| **Scenario** | `convert("meter", 1.0, "yard")` — **1 meter = 1.09361 yard** |
| **Then** | `abs(result - 1.09361) <= 1e-4` (ratio golden) |
| **Invariant** | `factor_to_meter(yard) = 0.9144 = 1/1.09361` |
| **RED 실패 예상** | yard 분기 누락 또는 독립 yard/feet 비율 → **모순·0.0** |

---

## Track B-03 — convertAll 전 단위

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-B-03` / `test_convert_all_meter10_returns_three_units` |
| **Scenario** | builtin Registry; `convertAll("meter", 1.0)` |
| **Then** | 반환 3건; unit 집합 `{feet, meter, yard}`; 각 값 ε 이내; 순서 lex (선택: formatter 테스트) |
| **Invariant** | `|convertAll| = |Registry|`; 각 target에 대해 hub 공식 1회 |
| **RED 실패 예상** | `convertAll` **부재**; baseline은 3줄 고정 출력만 |

---

## Track B-04 — registerUnit 동적 등록

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-B-04` / `test_register_cubit_then_convert_to_meter` |
| **Scenario** | `registerUnit("cubit", 0.4572)` 후 `convert("cubit", 1.0, "meter")` |
| **Then** | `abs(result - 0.4572) <= 1e-5`; `convertAll` 크기 +1 |
| **Invariant** | 등록 후에도 builtin `meter:2.5` → feet golden **불변** (REG-04) |
| **RED 실패 예상** | Registry / `registerUnit` **부재** |

---

## Track B-05 — loadConfig JSON/YAML

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-B-05` / `test_load_config_json_applies_feet_factor` |
| **Scenario** | `loadConfig("config/units.json", registry)` 성공 |
| **Then** | `convert("meter", 1.0, "feet")` ≈ **3.28084**; YAML fixture 동일 스키마 시 동일 |
| **Invariant** | config `factor_to_meter(feet)=0.3048` ⟺ **1 m = 3.28084 ft** |
| **RED 실패 예상** | 파일 로드·Registry 갱신 **부재** |

---

## Track B-06 — loadConfig 없는 경로 → 기본값

| 항목 | 내용 |
|------|------|
| **Test ID** | `RED-B-06` / `test_load_config_missing_keeps_builtin_ratios` |
| **Scenario** | `loadConfig("config/not_exist.json", registry)` (TC-B-07) |
| **Then** | 성공(또는 정책상 true); `convert(meter→feet)` ≈ **3.28084**; `meter→yard` ≈ **1.09361** |
| **Invariant** | builtin 비율 **3.28084 / 1.09361** 회귀 불변 (REG-01) |
| **RED 실패 예상** | 실패 처리만 있고 fallback 없음 → convert 실패 또는 0 |

---

# RED ↔ README 체크리스트 매핑

| README RED | Track ID |
|------------|----------|
| TC-A-01 | RED-A-01 |
| TC-A-02 | RED-A-02 |
| TC-A-03 | RED-A-03 |
| TC-A-04 | RED-A-04 |
| TC-A-05 | RED-A-06 (JSON) + RED-A-05 (table) |
| TC-A-06 / A-07 | RED-A-05 / 경계 `meter:0` (별도 TC) |
| TC-B-01 | RED-B-01 |
| TC-B-02 | RED-B-02 |
| TC-B-03 | RED-B-03 (feet→meter는 B-01 역변환 보조) |
| TC-B-04 | RED-B-04 |
| TC-B-05 | RED-B-05 |
| TC-B-06 | RED-B-06 |

---

# Catch2 작성 시 (RED only) — 파일·태그 권장

| 파일 (신규 RED 전용) | 태그 |
|----------------------|------|
| `tests/red_track_a_boundary.cpp` | `[red][boundary]` |
| `tests/red_track_b_domain.cpp` | `[red][entity]` |

**금지:** `src/entity/*`, `src/boundary/*` 수정 · 기존 테스트를 GREEN 맞추기 위한 완화.

**RED 확인 명령 (baseline만 있을 때):**

```bash
cmake -G Ninja -S . -B build
cmake --build build
ctest --test-dir build -R red --output-on-failure   # 전부 FAIL 또는 빌드 FAIL 기대
```

---

*본 문서는 RED 명세만 포함한다. 구현·GREEN·REFACTOR는 후속 단계에서 수행한다.*
