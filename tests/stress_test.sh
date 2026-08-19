#!/bin/bash
# ==============================================================================
# tests/stress_test.sh
# Bo kiem thu tu dong cho bai toan Tong khoang cach tren cay (Tree DP & Re-rooting)
#
# Bon nhom test theo dung yeu cau cua chuyen de:
#   1) TEST CO BAN       - cay nho, ket qua co the doi chieu bang tay
#   2) TEST BIEN         - N=1, N=2, cay N=200.000 (chong tran ngan xep/tran so)
#   3) TEST DOI KHANG    - du lieu khong hop le, phai bi InputValidator tu choi
#   4) TEST NGAU NHIEN   - sinh ngau nhien 100 testcase, doi chieu TreeDPSolver
#                          voi BaselineSolver bang lenh diff
#
# Cach chay (tu thu muc goc cua du an):
#   chmod +x tests/stress_test.sh
#   ./tests/stress_test.sh
#
# Toan bo nhat ky (khong ma mau) duoc luu tu dong tai: results/stress_test.log
# ==============================================================================

set -u
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR"

SOLVER=./tests/solver
GEN=./tests/generator
DATA=./tests
RESULTS=./results
mkdir -p "$RESULTS"
LOG_FILE="$RESULTS/stress_test.log"

exec > >(tee >(sed -r "s/\x1B\[[0-9;]*[a-zA-Z]//g" > "$LOG_FILE")) 2>&1

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; NC='\033[0m'

TOTAL=0; PASSED=0; FAILED=0
FAILED_NAMES=()

section() {
    echo
    echo -e "${BOLD}${CYAN}================================================================${NC}"
    echo -e "${BOLD}${CYAN}  $1${NC}"
    echo -e "${BOLD}${CYAN}================================================================${NC}"
}
pass() { TOTAL=$((TOTAL+1)); PASSED=$((PASSED+1)); echo -e "  ${GREEN}[PASS]${NC} $1"; }
fail() { TOTAL=$((TOTAL+1)); FAILED=$((FAILED+1)); FAILED_NAMES+=("$1"); echo -e "  ${RED}[FAIL]${NC} $1  ${YELLOW}($2)${NC}"; }

if [ ! -x "$SOLVER" ] || [ ! -x "$GEN" ]; then
    echo -e "${RED}Chua build chuong trinh. Chay:${NC}"
    echo "  g++ -O3 -std=c++17 src/main.cpp -o tests/solver"
    echo "  g++ -O3 -std=c++17 src/generator.cpp -o tests/generator"
    exit 1
fi

echo -e "${BOLD}Nhat ky kiem thu duoc ghi dong thoi tai: $LOG_FILE${NC}"
START_TIME=$(date +%s)

# ==============================================================================
# 1) TEST CO BAN
# ==============================================================================
section "1) TEST CO BAN (Basic Tests)"
for f in "$DATA"/basic/*.txt; do
    name=$(basename "$f")
    out_dp=$("$SOLVER" < "$f" 2>/dev/null)
    out_bl=$("$SOLVER" --baseline < "$f" 2>/dev/null)
    if [ "$out_dp" == "$out_bl" ]; then
        pass "$name  (TreeDPSolver == BaselineSolver)"
    else
        fail "$name" "ket qua khong khop voi BaselineSolver"
    fi
done

echo
echo -e "  ${YELLOW}Doi chieu tay cho basic_01_tree8.txt (cay chuan 8 dinh):${NC}"
echo "  Cay: 1-2,1-3,2-4,2-5,3-6,3-7,7-8 ; tinh tay tu dinh 1 = 0+1+1+2+2+2+2+3 = 13"
actual_ans1=$("$SOLVER" < "$DATA/basic/basic_01_tree8.txt" | awk '$1==1{print $2}')
if [ "$actual_ans1" == "13" ]; then
    pass "basic_01_tree8.txt: ans[1]=13 khop voi tinh tay"
else
    fail "basic_01_tree8.txt: ans[1]" "ky vong 13, thuc te $actual_ans1"
fi

# ==============================================================================
# 2) TEST BIEN
# ==============================================================================
section "2) TEST BIEN (Edge Case Tests)"

out=$("$SOLVER" < "$DATA/edge/edge_01_n1_default_label.txt" 2>&1)
[ "$out" == "1 0" ] && pass "edge_01_n1_default_label.txt -> '1 0'" || fail "edge_01_n1_default_label.txt" "output='$out'"

out=$("$SOLVER" < "$DATA/edge/edge_02_n1_custom_label.txt" 2>&1)
[ "$out" == "Hanoi 0" ] && pass "edge_02_n1_custom_label.txt -> 'Hanoi 0'" || fail "edge_02_n1_custom_label.txt" "output='$out'"

out=$("$SOLVER" < "$DATA/edge/edge_03_n2.txt" 2>&1)
expected=$'a 1\nb 1'
[ "$out" == "$expected" ] && pass "edge_03_n2.txt -> khoang cach = 1" || fail "edge_03_n2.txt" "output khong dung"

echo
echo -e "  ${YELLOW}--- Regression test: chong tran ngan xep (cay duong thang N=200.000) ---${NC}"
t0=$(date +%s.%N)
out_default=$("$SOLVER" < "$DATA/edge/edge_04_line_200000.txt" 2>&1); rc_default=$?
t1=$(date +%s.%N)
if [ $rc_default -eq 0 ]; then
    pass "edge_04_line_200000.txt chay xong voi stack mac dinh ($(echo "$t1 - $t0" | bc)s)"
else
    fail "edge_04_line_200000.txt (stack mac dinh)" "return code $rc_default"
fi

out_256kb=$(bash -c "ulimit -s 256; '$SOLVER' < '$DATA/edge/edge_04_line_200000.txt'" 2>&1); rc_256kb=$?
if [ $rc_256kb -eq 0 ] && [ "$out_256kb" == "$out_default" ]; then
    pass "edge_04_line_200000.txt VAN CHAY DUNG voi stack 256KB (truoc day se Segmentation fault)"
elif [ $rc_256kb -eq 139 ]; then
    fail "edge_04_line_200000.txt (stack 256KB)" "SEGMENTATION FAULT - loi tran ngan xep chua duoc sua!"
else
    fail "edge_04_line_200000.txt (stack 256KB)" "return code $rc_256kb hoac ket qua khac"
fi

t0=$(date +%s.%N)
"$SOLVER" < "$DATA/edge/edge_05_star_200000.txt" > /dev/null 2>&1; rc=$?
t1=$(date +%s.%N)
[ $rc -eq 0 ] && pass "edge_05_star_200000.txt chay xong ($(echo "$t1 - $t0" | bc)s)" || fail "edge_05_star_200000.txt" "return code $rc"

neg_count=$("$SOLVER" < "$DATA/edge/edge_06_overflow_check_line200000.txt" | awk '$2<0' | wc -l)
max_val=$("$SOLVER" < "$DATA/edge/edge_06_overflow_check_line200000.txt" | awk '{print $2}' | sort -n | tail -1)
[ "$neg_count" -eq 0 ] && pass "edge_06_overflow_check: khong co gia tri am (max=$max_val)" || fail "edge_06_overflow_check" "phat hien $neg_count gia tri am -> TRAN SO!"

# ==============================================================================
# 3) TEST DOI KHANG
# ==============================================================================
section "3) TEST DOI KHANG (Adversarial Tests)"
while IFS=':' read -r fname expected_rc desc; do
    [ -z "$fname" ] && continue
    f="$DATA/adversarial/$fname"
    "$SOLVER" < "$f" > /dev/null 2>&1; rc=$?
    if [ "$rc" -eq 139 ] || [ "$rc" -eq 134 ]; then
        fail "$fname" "CRASH (Segmentation fault/abort) - $desc"
    elif [ "$rc" -eq "$expected_rc" ]; then
        pass "$fname  [$desc]"
    else
        fail "$fname" "return code $rc, ky vong $expected_rc - $desc"
    fi
done < "$DATA/adversarial/expected.txt"

# ==============================================================================
# 4) TEST NGAU NHIEN DOI CHIEU - 100 TESTCASE
# ==============================================================================
section "4) TEST NGAU NHIEN DOI CHIEU (100 testcase)"

TYPES=("line" "star" "balanced" "random")
SIZES=(1 2 3 5 8 10 15 20 30 50 75 100 150 200 300 500 750 1000 1500 2000 3000 4000 6000 8000 12000)

RANDOM_LOG="$RESULTS/random_stress_log.csv"
echo "loai_cay,N,ket_qua,thoi_gian_treedp_s,thoi_gian_baseline_s" > "$RANDOM_LOG"

case_no=0
for type in "${TYPES[@]}"; do
    for n in "${SIZES[@]}"; do
        case_no=$((case_no+1))
        tmpfile=$(mktemp)
        "$GEN" "$n" "$type" > "$tmpfile"

        t0=$(date +%s.%N); out_dp=$("$SOLVER" < "$tmpfile"); t1=$(date +%s.%N)
        time_dp=$(echo "$t1 - $t0" | bc)

        t0=$(date +%s.%N); out_bl=$("$SOLVER" --baseline < "$tmpfile"); t1=$(date +%s.%N)
        time_bl=$(echo "$t1 - $t0" | bc)

        if [ "$out_dp" == "$out_bl" ]; then
            pass "[$case_no/100] loai=$type N=$n (TreeDP:${time_dp}s Baseline:${time_bl}s)"
            echo "$type,$n,MATCH,$time_dp,$time_bl" >> "$RANDOM_LOG"
        else
            fail "[$case_no/100] loai=$type N=$n" "KET QUA KHONG KHOP"
            echo "$type,$n,MISMATCH,$time_dp,$time_bl" >> "$RANDOM_LOG"
        fi
        rm -f "$tmpfile"
    done
done
echo
echo -e "  ${YELLOW}Nhat ky chi tiet 100 testcase da luu tai: $RANDOM_LOG${NC}"

# ==============================================================================
# 5) MINH HOA HIEU NANG O(N) vs O(N^2)
# ==============================================================================
section "5) MINH HOA HIEU NANG: O(N) so voi O(N^2) tren N=200.000"
echo -e "  ${YELLOW}Dang chay TreeDPSolver (O(N))...${NC}"
t0=$(date +%s.%N)
"$SOLVER" < "$DATA/edge/edge_04_line_200000.txt" > /dev/null
t1=$(date +%s.%N)
echo -e "  ${GREEN}=> TreeDPSolver hoan thanh trong $(echo "$t1 - $t0" | bc) giay${NC}"

echo
echo -e "  ${YELLOW}Dang chay BaselineSolver (O(N^2)) voi gioi han 10 giay...${NC}"
timeout 10 "$SOLVER" --baseline < "$DATA/edge/edge_04_line_200000.txt" > /dev/null; rc=$?
if [ $rc -eq 124 ]; then
    echo -e "  ${RED}=> BaselineSolver KHONG hoan thanh sau 10 giay (bi timeout)${NC}"
else
    echo -e "  ${GREEN}=> BaselineSolver hoan thanh${NC}"
fi

# ==============================================================================
# TONG KET
# ==============================================================================
END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))
section "TONG KET"
echo -e "  Tong so test da chay : ${BOLD}$TOTAL${NC}"
echo -e "  ${GREEN}Dat (PASS)           : $PASSED${NC}"
echo -e "  ${RED}Khong dat (FAIL)     : $FAILED${NC}"
echo -e "  Thoi gian chay toan bo bo test: ${DURATION}s"

if [ $FAILED -gt 0 ]; then
    echo
    echo -e "  ${RED}Danh sach test khong dat:${NC}"
    for n in "${FAILED_NAMES[@]}"; do echo -e "    - $n"; done
    exit 1
else
    echo
    echo -e "  ${GREEN}${BOLD}TAT CA TEST DEU DAT.${NC}"
    exit 0
fi
