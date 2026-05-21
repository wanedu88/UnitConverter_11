# UnitConverter_11 — 결함 목록 (Defect List)

| 문서 ID | DL-UC11-001 |
| 작성 | QA 리드 |
| 기준 | [test_plan.md](test_plan.md), Catch2 36건, baseline `UnitConverter.cpp` |
| 최종 검증 | `ctest` 36/36 PASS (`uc11_entity` / `uc11_boundary`) |

**상태 범례:** `Open` = 미수정 · `Fixed` = Entity/테스트 경로 반영 · `Known` = legacy만 해당, 목표 구조는 통과

---

## 결함 표

| ID | Severity | 변환 타입 | 재현 절차 | 기대값 | 실제값 | 근본 원인 | 수정 요약 | 상태 |
|----|----------|-----------|-----------|--------|--------|-----------|-----------|------|
| DEF-001 | Critical | meter→feet | RED 초기: `convert("meter", 2.5, "feet")` 또는 `meter:2.5` CLI | 8.202100 (ε≤1e-5) | 0.000000 | `meterValue`가 0.0 초기값 유지 — `unit=="meter"` 분기 누락·미실행 (`UnitConverter.cpp` if-else) | `if (unit == "meter") meterValue = value;` 확인 후 `inFeet = meterValue * 3.28084` | Fixed |
| DEF-002 | Critical | meter→feet | Entity 미구현 상태에서 `ctest` 실행 | TC-B-01 PASS | 링크/심볼 실패 또는 0 | 테스트는 `uc11::convert`인데 구현이 baseline `main()`만 존재 | `src/entity/UnitConverter.cpp` + `UnitRegistry::withBuiltins()` 도입, CMake `uc11_entity` 링크 | Fixed |
| DEF-003 | Major | 입력 검증 (전 단위) | legacy: `meter:-1.0` 입력 후 실행 | `std::invalid_argument` / exit 1, stdout 빈 (POL-NEG) | exit 0, 음수 feet·yard 출력 | `UnitConverter.cpp`에 `value < 0` 검증 없음 | `stod` 직후 `if (value < 0)` → stderr `Negative value not allowed: …`, return 1 | Open |
| DEF-004 | Major | meter→feet (표시) | legacy: `meter:2.5` stdout feet 행 확인 | `8.2021` (4자리 half-up, PRD §6.1) | `8.2021` 또는 `8.202099999` 등 raw `double` | legacy가 `roundHalfUp4` 미적용, iostream 기본 정밀도 | `OutputFormatter::formatTableLine` 또는 legacy에 4자리 half-up 적용 | Open (legacy) / Fixed (Boundary) |
| DEF-005 | Major | feet→meter (역변환) | `convert("feet", 1.0, "meter")` | 0.30480 (1÷3.28084) | 분기 누락 시 0.0 | feet 분기 `meterValue = value / 3.28084` 누락 | `else if (unit == "feet")` 분기 유지·검증 | Fixed |
| DEF-006 | Major | meter→yard | `convert("meter", 1.0, "yard")` | 1.09361 | 0.0 또는 오차 초과 | yard 분기 누락 또는 yard 독립 비율 하드코딩 | meter 허브: `meterValue*1.09361` 또는 Registry `factor_to_meter` | Fixed |
| DEF-007 | Major | 경계값 value=0 | `meter:0` → feet | 0.0 | (DEF-001 동시 발생 시) 0.0이 오탐 | meter 분기 실패와 0 허용 혼동 | 0은 정상; 양수만 DEF-001 경로 점검 | Fixed |
| DEF-008 | Major | 없는 단위 | `parsec:1.0` / `lightyear:1` | `Unknown unit: {unit}`, exit 1 | legacy: stderr 후 return 1 (일치) · Catch2: `invalid_argument` | Domain/Boundary 예외 매핑 불일치 가능 | `UnitRegistry::factorToMeter` → `Unknown unit:` throw (Entity) | Fixed |
| DEF-009 | Major | 입력 형식 | `meter2.5` (콜론 없음) | `Invalid format…`, exit 1 | legacy: 일치 · 파서 미연동 시 미검출 | `InputParser::parseInputLine` 미호출 | `find(':')==npos` → `invalid_argument` (Boundary) | Fixed |
| DEF-010 | Major | 파싱 | `meter:abc` | `Invalid number: abc` | legacy: 일치 (`catch(...)`) | `catch(...)`로 오류 타입 불명확 (PRD) | `stod` 실패 시 명시 `invalid_argument` 메시지; `catch(...)` 제거 | Open (legacy) / Fixed (Boundary) |
| DEF-011 | Major | 파싱 | `feet:1.2.3` | `Invalid number: 1.2.3` | `stod`가 `1.2`만 소비 시 잘못 통과 가능 | 부분 소비 검증 없음 | `consumed != valueStr.size()` 검사 (`InputParser.cpp`) | Fixed |
| DEF-012 | Minor | meter→feet (POL-OUT) | `meter:2.5` table 좌측 | 모든 줄 `2.5 meter = …` | legacy: 좌측은 일치 · 우측만 정밀도 이슈 | Formatter/lex 순서 미적용 | `formatTableLine` + `convertAll` lex 순서 | Fixed (Boundary) |
| DEF-013 | Minor | 단위명 | `"meter "` / `"Meter:2.5"` | 정규화 또는 거부 1정책 | `Unknown unit` (공백·대소문자) | unit 토큰 trim/lower 미적용 | trim 또는 `tolower` 정책 추가 (최소 변경) | Open |
| DEF-014 | Minor | 동적 등록 | `registerUnit` 후 `meter:2.5` golden | feet≈8.20210 불변 | builtin golden 깨짐 | Registry 교체 시 builtin 비율 덮어씀 | 등록은 추가만; builtin factor 유지 검증 | Fixed |
| DEF-015 | Minor | 설정 로드 | `loadConfig(없는 경로)` | 기본 3.28084 / 1.09361 (TC-B-07) | PRD 일부: Config load failed | 파일 없음 시 실패 vs 기본값 정책 이원 | `ConfigLoader`: 빈 파일 → `withBuiltins()` (문서화된 TC-B-07) | Fixed |
| DEF-016 | Minor | 설정 로드 | `tests/fixtures/bad.json` | load 실패 | — | JSON 파서 단순·깨진 스키마 | `parseUnitsFromContent` 실패 시 `Config load failed` | Fixed |
| DEF-017 | Info | 빌드 | Windows: `cmake -S . -B build` (기본 NMake) | Configure 성공 | `nmake` / `CMAKE_CXX_COMPILER not set` | Generator·컴파일러 미지정 | `cmake -G Ninja` + g++ (`CMakeLists.txt` Win32 g++ 탐지) | Known |
| DEF-018 | Info | 빌드 | Clang + MSVC `kernel32.lib` 링크 | 링크 성공 | `could not open kernel32.lib` | LLVM이 MSVC 런타임 링크 시도 | MinGW `g++` / Ninja 사용 | Known |

---

## Catch2 실패와의 매핑 (RED 단계)

| 실패 메시지 (예) | 결함 ID | 관련 테스트 |
|------------------|---------|-------------|
| `expected 8.202100 but got 0.000000` | DEF-001, DEF-002 | `test_meter_to_feet_value_25_returns_82021` |
| `WithinAbs(8.20210, …) failed` (미세 오차) | DEF-004 | `test_meter_to_feet_*`, `test_format_*` |
| `std::invalid_argument` 미발생 | DEF-003 | `test_validate_meter_neg1_*` |
| `Unknown unit` / `Invalid format` 불일치 | DEF-008, DEF-009 | `test_exceptions.cpp`, `test_boundary_contract.cpp` |
| `Config load failed` vs 기본값 | DEF-015 | `test_load_json_missing_file_defaults` |

---

## 회귀 확인 (Entity/Boundary 경로)

```powershell
cmake -G Ninja -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

| 범위 | 결과 | 비고 |
|------|------|------|
| Catch2 36건 | **PASS** | DEF-001, 002, 005~012, 014~016 **Fixed** 확인 |
| `unit_converter_legacy` | **Open** | DEF-003, 004, 010, 013 — baseline CLI 수동 검증 필요 |

---

## 변경 이력

| 날짜 | 변경 |
|------|------|
| 2026-05-21 | 초안 작성 — RED/QA 분석·ctest GREEN 기준 정리 |
