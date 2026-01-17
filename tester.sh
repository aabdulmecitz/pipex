#!/bin/bash

# --- COLORS & ICONS ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

CHECK="✅"
CROSS="❌"
INFO="ℹ️"
GEAR="⚙️"
ROCKET="🚀"

# --- CONFIGURATION ---
INFILE="test_infile"
OUT_PIPEX="test_out_px"
OUT_SHELL="test_out_sh"
TOTAL=0
PASSED=0

# --- HELPER FUNCTIONS ---
print_header() {
    clear
    echo -e "${CYAN}${BOLD}"
    echo "  ____  _                      _____         _             "
    echo " |  _ \(_)_ __   _____  __    |_   _|__  ___| |_ ___ _ __  "
    echo " | |_) | | '_ \ / _ \ \/ /      | |/ _ \/ __| __/ _ \ '__| "
    echo " |  __/| | |_) |  __/>  <       | |  __/\__ \ ||  __/ |    "
    echo " |_|   |_| .__/ \___/_/\_\      |_|\___||___/\__\___|_|    "
    echo "         |_|                                               "
    echo -e "${RESET}"
    echo -e "${MAGENTA}=======================================================${RESET}"
    echo -e "          ${BOLD}AOZKAYA'S PIPEX TEST SUITE${RESET}"
    echo -e "${MAGENTA}=======================================================${RESET}\n"
}

setup_files() {
    echo -e "${INFO} ${BOLD}Preparing test files...${RESET}"
    echo "Pipex test file content line 1" > $INFILE
    echo "Pipex test file content line 2" >> $INFILE
    echo "Special characters: !@#$%^&*()" >> $INFILE
}

compile_project() {
    echo -e "${GEAR} ${BOLD}Compiling binaries...${RESET}"
    
    # Compile Mandatory
    printf "  %-55s" "Building [Mandatory]..."
    make re > .make_log 2>&1
    if [ ! -f "./pipex" ]; then
        echo -e "${RED}${CROSS} FAILED${RESET}"
        cat .make_log
        exit 1
    fi
    echo -e "${GREEN}${CHECK} OK${RESET}"

    # Compile Bonus
    printf "  %-55s" "Building [Bonus]..."
    make bonus >> .make_log 2>&1
    if [ ! -f "./pipex_bonus" ]; then
        echo -e "${RED}${CROSS} FAILED${RESET}"
        cat .make_log
        exit 1
    fi
    echo -e "${GREEN}${CHECK} OK${RESET}"
    rm -f .make_log
    echo ""
}

run_test() {
    local category=$1
    local name=$2
    local px_cmd=$3
    local sh_cmd=$4

    ((TOTAL++))
    printf "  %-55s" "[$category] $name"
    
    # Execute commands
    eval $sh_cmd 2>/dev/null
    eval $px_cmd 2>/dev/null
    
    # Verification
    if [ ! -f $OUT_PIPEX ]; then
        echo -e "${RED}${CROSS} NO OUTPUT${RESET}"
        return
    fi

    diff $OUT_PIPEX $OUT_SHELL > .test_diff
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}${CHECK} PASSED${RESET}"
        ((PASSED++))
        rm -f .test_diff
    else
        echo -e "${RED}${CROSS} FAILED${RESET}"
        echo -e "${RED}${BOLD}--- DIFF ---${RESET}"
        cat .test_diff
        echo -e "${RED}${BOLD}------------${RESET}"
        rm -f .test_diff
    fi
    rm -f $OUT_PIPEX $OUT_SHELL
}

# --- EXECUTION ---

print_header
setup_files
compile_project

# --- MANDATORY SECTION ---
echo -e "${BLUE}${BOLD}[MANDATORY TESTS]${RESET}"
run_test "MAND" "Simple cat | wc" \
    "./pipex $INFILE 'cat' 'wc' $OUT_PIPEX" \
    "< $INFILE cat | wc > $OUT_SHELL"

run_test "MAND" "Simple grep | cat" \
    "./pipex $INFILE 'grep Pipex' 'cat' $OUT_PIPEX" \
    "< $INFILE grep Pipex | cat > $OUT_SHELL"

run_test "MAND" "Complex command flags" \
    "./pipex $INFILE 'tr -d a' 'grep e' $OUT_PIPEX" \
    "< $INFILE tr -d a | grep e > $OUT_SHELL"

# --- BONUS SECTION ---
echo -e "\n${BLUE}${BOLD}[BONUS TESTS: MULTIPLE PIPES]${RESET}"
run_test "MULT" "3 Commands: cat | cat | cat" \
    "./pipex_bonus $INFILE 'cat' 'cat' 'cat' $OUT_PIPEX" \
    "< $INFILE cat | cat | cat > $OUT_SHELL"

run_test "MULT" "5 Commands: cat | grep | head | wc" \
    "./pipex_bonus $INFILE 'cat' 'cat' 'grep content' 'head -n 5' 'wc -l' $OUT_PIPEX" \
    "< $INFILE cat | cat | grep content | head -n 5 | wc -l > $OUT_SHELL"

# --- HERE_DOC SECTION ---
echo -e "\n${BLUE}${BOLD}[BONUS TESTS: HERE_DOC]${RESET}"
((TOTAL++))
printf "  %-55s" "[HDOC] Simple heredoc flow"
echo "test line" > .hd_in
echo "LIMITER" >> .hd_in
./pipex_bonus here_doc LIMITER "cat" "wc -l" $OUT_PIPEX < .hd_in > /dev/null 2>&1
cat << LIMITER | wc -l > $OUT_SHELL
test line
LIMITER
diff $OUT_PIPEX $OUT_SHELL > /dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}${CHECK} PASSED${RESET}"
    ((PASSED++))
else
    echo -e "${RED}${CROSS} FAILED${RESET}"
fi
rm -f $OUT_PIPEX $OUT_SHELL .hd_in

# --- VALGRIND ---
if command -v valgrind &> /dev/null; then
    echo -e "\n${BLUE}${BOLD}[MEMORY CHECK]${RESET}"
    printf "  %-55s" "[LEAK] Running Valgrind..."
    valgrind --leak-check=full --error-exitcode=1 ./pipex_bonus $INFILE "cat" "head -n 1" $OUT_PIPEX > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}${CHECK} CLEAN${RESET}"
    else
        echo -e "${RED}${CROSS} LEAKS FOUND${RESET}"
    fi
    rm -f $OUT_PIPEX
fi

# --- SUMMARY ---
echo -e "\n${MAGENTA}=======================================================${RESET}"
printf "  ${BOLD}FINAL RESULT: %d/%d passed${RESET}\n" $PASSED $TOTAL
if [ $PASSED -eq $TOTAL ]; then
    echo -e "  ${GREEN}${ROCKET} EXCELLENT! PROJECT IS PERFECT.${RESET}"
else
    echo -e "  ${RED}${CROSS} SOME TESTS FAILED. CHECK LOGS.${RESET}"
fi
echo -e "${MAGENTA}=======================================================${RESET}\n"

# Cleanup
rm -f $INFILE
