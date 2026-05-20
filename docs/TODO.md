# To-Do 리스트 — UnitConverter (C++)

**워크스페이스:** UnitConverter_11  
**기준 문서:** `docs/PRD.md` (Phase 5) v1.0  
**판정 원칙:** 학습자·CI가 Catch2·체크리스트로 검증했을 때만 완료

---

## 🔴 필수 (Must-Have) — v1.0 릴리스 차단 항목

| | 작업 설명 | 연관 PRD | 완료 기준 (누가 → 무엇 → 통과) |
|---|-----------|----------|--------------------------------|
| [ ] | **프로젝트 골격·CMake·Catch2** — `src/entity`, `src/control`, `src/boundary`, `tests/` 디렉터리와 빌드·테스트 타깃 정의 | §4.1, §4.2, F-06 | **학습자**가 `ctest` 또는 Catch2 실행 → 전 타깃 빌드 성공, 빈 테스트 0 failed |
| [ ] | **Entity RED→GREEN — Quantity·Registry·Converter** | F-02, F-03, §5.1, G-01 | **CI**가 `[entity]` 스위트 실행 → `1m→3.28084ft`, `1m→1.09361yd`, `2.5m` ε≤1×10⁻⁴, 왕복 ε 통과 |
| [ ] | **입력 파싱·검증 (F-01)** — `unit:value`, 콜론·숫자·단위명 | F-01, §3.3, AC-02 | **Catch2** `[boundary][parsing]` → `meter:2.5` 성공; `meter2.5`·`meter:abc`·`feet:1.2.3` exit 1 + stderr 전문 일치 |
| [ ] | **음수 거부·0 허용 (POL-NEG, F-05)** | POL-NEG, F-05, AC-03, GH-05, GH-08 | **Catch2** → `yard:-3` stderr `Negative value not allowed: -3`, stdout 빈; `meter:0` exit 0 |
| [ ] | **미지원 단위 처리** | F-02, AC-04, GH-07 | **통합 테스트** → `lightyear:1` exit 1, `Unknown unit: lightyear`, stdout 빈 |
| [ ] | **table 출력 (POL-OUT, F-04)** | F-04, POL-OUT, §6.1, AC-01 | **Catch2** → `meter:2.5` 3줄; 매 줄 좌측 `2.5 meter`; 우측 4자리 half-up; lex 순서 |
| [ ] | **Control 유스케이스 — ConvertLength** | §4.2, F-03 | **CI** → Control 커버리지 100%; parse→convertAll→format 경로 1회 이상 호출 테스트 GREEN |
| [ ] | **ERR-* 계약 8종 golden** | F-01, §3.2, G-02, AC-02~04 | **CI** → FORMAT·VALUE·UNIT·NEG·CONFIG·OUTPUT·REGISTER 각 ≥1 테스트; stderr·exit 1 스냅샷 diff 0 |
| [ ] | **Gherkin GH-01~08 매핑 테스트** | 부록 A, AC-01~04 | **학습자**가 Traceability 표 제출 → GH-01~08 각 Catch2 TEST_CASE ID 1개 이상, 전부 GREEN |
| [ ] | **BCE 의존 방향 정적 준수** | §4.2, G-03 | **리뷰어**가 entity 헤더 include 검사 → boundary/control 헤더 0건 |
| [ ] | **Entity 커버리지 ≥95%** | §4.3, AC-08, G-05 | **CI** 커버리지 리포트 → entity 라인 ≥95% |
| [ ] | **Boundary 커버리지 ≥85% + ERR 경로 100%** | §4.3, AC-08 | **CI** → boundary 라인 ≥85%, ERR-* 코드별 테스트 존재 8/8 |
| [ ] | **인수 AC-01~04, AC-08 v1.0** | §7.1 | **강사** 인수 → AC-01·02·03·04·08 체크박스 전부 ✓ |

---

## 🟡 권장 (Should-Have) — 품질 향상 항목

| | 작업 설명 | 연관 PRD | 완료 기준 (누가 → 무엇 → 통과) |
|---|-----------|----------|--------------------------------|
| [ ] | **JSON 출력 (F-07, §6.2)** | F-07, §6.2, AC-07 | **Catch2** `[serialization]` → schema·`conversions.length`=registry·`input` 원문 보존 GREEN |
| [ ] | **CSV 출력 (F-07, §6.3)** | F-07, §6.3, AC-07 | **Catch2** → 헤더 1행 고정, 데이터 행=단위 수, source_* 입력 고정 GREEN |
| [ ] | **table/json/csv 수치 parity** | §6.4, AC-07 | **Catch2** → 동일 `meter:2.5` 3포맷 converted value 집합 동일 (ε=0 on rounded) |
| [ ] | **config/units.json 로드 (F-08, §5.2)** | F-08, §5.2, §5.1 | **통합** → 유효 JSON 로드 후 3단위 환산 GH-BG 비율 일치 GREEN |
| [ ] | **설정 로드 실패 (AC-06)** | F-08, AC-06 | **통합** → 깨진 JSON·누락 파일·factor≤0 → `Config load failed:`, exit 1, stdout 빈 |
| [ ] | **Data 레이어 커버리지 ≥90%** | §4.3, G-05 | **CI** → data/ config 로드 라인 ≥90% |
| [ ] | **동적 등록 `register:unit:factor` (F-09, §5.3)** | F-09, §5.3, AC-05, G-04 | **통합** → `register:cubit:0.4572` 후 `cubit:1`→0.4572m; convertAll 4항목; `meter:2.5` golden 유지 |
| [ ] | **Registry OCP — duplicate·InvalidRatio** | §4.4, F-09, G-04 | **Catch2 entity** → duplicate 등록 실패; factor≤0 실패; cubit 추가 시 Converter if-else +0줄 |
| [ ] | **`.cursorrules` + clang-format 적용** | §4.1, tdd_rules | **리뷰어** → 8 최상위 키·tdd_rules 3 phase 존재; `clang-format` diff 0 on commit |
| [ ] | **README 갱신 — 빌드·실행·config·register·format** | SC-03, §2.2 | **학습자** PR → README에 v1.0 CLI 예시·에러 표·PRD 링크; **강사** 승인 |
| [ ] | **인수 AC-05~07** | §7.1 | **강사** → AC-05·06·07 ✓ |

---

## 🟢 선택 (Nice-to-Have) — v2.0 후보

| | 작업 설명 | 기대 가치 |
|---|-----------|-----------|
| [ ] | **YAML 설정 (F-10)** | 동일 스키마로 설정 형식 선택지 확대 |
| [ ] | **단위명 대소문자 정규화 정책 ON** | `Meter:2.5` 입력 허용 시 UX 개선 (계약 테스트 추가 필요) |
| [ ] | **CI GitHub Actions 워크플로** | PR마다 자동 회귀·커버리지 게이트 |
| [ ] | **커버리지 리포트 HTML 아티팩트** | 강사·학습자 시각적 미달 레이어 확인 |
| [ ] | **Property-based roundtrip 테스트 (선택)** | D-INV-6 대량 조합 검증으로 신뢰도 상승 |

---

## 🔵 기술 부채 (Tech Debt)

| | 문제 설명 | 발생 원인 | 해결 방향 |
|---|-----------|-----------|-----------|
| [ ] | **단일 `UnitConverter.cpp`에 파싱·환산·출력 결합** | 실습 시작 템플릿 | boundary/control/entity로 이전 후 main은 CliApp 위임만 |
| [ ] | **`catch(...)`로 모든 파싱 오류 동일 처리** | 템플릿 예제 | domain/boundary별 예외 타입·ERR-VALUE 매핑 |
| [ ] | **비율 상수 main 분기 하드코딩** | README 예제 직구현 | Registry + `config/units.json` 단일 출처 (REG-01) |
| [ ] | **음수·POL-NEG 미구현** | 초기 템플릿 범위 | F-05 + GH-05 테스트 선행 RED |
| [ ] | **README 표시값(8.2, 2.7) vs 4자리 half-up 불일치** | README 반올림 예시 단순화 | PRD §6.1 우선; README에 “테스트 기준 4자리” 명시 |
| [ ] | **테스트·config·docs 디렉터리 부재** | Phase 5 이전 | `docs/PRD.md`·본 TODO·tests/·config/ 추가 (문서는 진행 중) |

---

## ✅ 완료 항목 (Done)

| | 완료 내용 | 완료일 | 관련 커밋/PR 메시지 |
|---|-----------|--------|---------------------|
| [x] | **Phase 5 PRD 작성** (`docs/PRD.md`) — 기능·인수·회귀·Gherkin 인덱스 | 2026-05-20 | docs: add PRD v1.0 from Phase 4 traceability |
| [x] | **Phase 4 요구 패키지** — Epic/Journey/Story/Gherkin/Level5 (대화 산출) | 2026-05-20 | docs: requirements narrative (참조용) |
| [x] | **초기 단일 파일 변환기 동작** — meter/feet/yard table 출력 | (기존) | baseline: UnitConverter.cpp template |

---

## 📋 회귀 방지 체크리스트 (PRD §7.2)

**배포(v1.0 태그) 전 — CI 또는 학습자가 전항 실행 후 ✓**

| ✓ | 항목 | 검증자 | 통과 조건 |
|---|------|--------|-----------|
| ☐ | **REG-01** builtin 비율 | CI | `3.28084`·`1.09361` golden 테스트 0 failed |
| ☐ | **REG-02** ERR-* 전문·exit | CI | stderr golden 스냅샷 diff 0 (8종) |
| ☐ | **REG-03** POL-OUT 좌측 | CI | GH-01·06·08 assert GREEN |
| ☐ | **REG-04** `meter:2.5` E2E | CI | T-INT-01 stdout golden diff 0 |
| ☐ | **REG-05** refactor 게이트 | 리뷰어 | refactor PR 설명에 “전 테스트 GREEN” 증빙 |
| ☐ | **계약 테스트 전체** | CI | Catch2 전 스위트 0 failed |
| ☐ | **커버리지 목표** | CI | Entity≥95%, Boundary≥85%, Data≥90%, Control 100% |
| ☐ | **README 갱신** | 강사 | v1.0 사용법·PRD 링크·config/register/format 문서화 확인 |
| ☐ | **Traceability 표** | 학습자 | Epic→Story→GH→Test 제목 공백 0 |

---

## 🗓️ 마일스톤

| 마일스톤 | 포함 항목 (PRD) | 목표일 | 상태 |
|----------|-----------------|--------|------|
| **M0 — Baseline** | (출발) UnitConverter.cpp, README | — | Done |
| **M1 — 계약·테스트 골격** | F-06, F-01 RED, §4.1 CMake/Catch2, ERR-* 테스트 골격 | D+1 | Planned |
| **M2 — Entity v1** | F-02, F-03, §5.1, G-01, AC-01(수치), GH-01·08 | D+2 | Planned |
| **M3 — Boundary v1** | F-01, F-04, F-05, POL-NEG, POL-OUT, AC-02~04, GH-02~07 | D+3 | Planned |
| **M4 — v1.0 릴리스** | F-01~06, AC-01~04·08, G-02~05, REG-01~05, §7.1 Must | D+4 | Planned |
| **M5 — 확장(권장)** | F-07, F-08, F-09, AC-05~07, §6.2~6.4 | D+5~6 | Planned |
| **M6 — v2.0 후보** | F-10, Nice-to-Have | TBD | Backlog |

*D+n: 실습 일정 기준 상대일. 팀 캘린더에 맞게 조정.*

---

*코드 없음. 완료 판정은 주관적 “동작 확인”이 아니라 PRD §7.1·§7.2의 테스트·커버리지·golden 기준만 인정.*
