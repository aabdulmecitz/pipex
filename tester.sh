#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RESET='\033[0m'

# Files
INFILE="test_infile"
OUTFILE_PIPEX="test_outfile_pipex"
OUTFILE_SHELL="test_outfile_shell"

# Preparations
echo "lorem ipsum" > $INFILE
echo "dolor sit amet" >> $INFILE
echo "consectetur adipiscing elit" >> $INFILE
echo "sed do eiusmod tempor" >> $INFILE

# Header
echo -e "${BLUE}=======================================${RESET}"
echo -e "${BLUE}        PIPEX COMPREHENSIVE TESTER      ${RESET}"
echo -e "${BLUE}=======================================${RESET}"

# Compilation
echo -e "${YELLOW}Compiling...${RESET}"
make re > /dev/null
make bonus_re > /dev/null
if [ ! -f "./pipex" ] || [ ! -f "./pipex_bonus" ]; then
    echo -e "${RED}Compilation failed! Make sure your Makefile has all and bonus rules.${RESET}"
    exit 1
fi
echo -e "${GREEN}Compilation successful.${RESET}\n"

# Test counter
TOTAL=0
PASSED=0

check_test() {
    ((TOTAL++))
    local test_name=$1
    local executable=$2
    local cmd_pipex=$3
    local cmd_shell=$4
    
    echo -e "${YELLOW}Testing: ${test_name}${RESET}"
    
    # Run Shell
    eval $cmd_shell 2> /dev/null
    # Run Pipex
    eval $cmd_pipex 2> /dev/null
    
    # Compare
    if [ ! -f $OUTFILE_PIPEX ]; then
        echo -e "${RED}[FAIL]${RESET} Pipex did not create $OUTFILE_PIPEX"
        return
    fi
    diff $OUTFILE_PIPEX $OUTFILE_SHELL > /dev/null
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[PASS]${RESET} Output matches."
        ((PASSED++))
    else
        echo -e "${RED}[FAIL]${RESET} Output differs!"
    fi
    rm -f $OUTFILE_PIPEX $OUTFILE_SHELL
}

# --- MANDATORY TESTS ---
echo -e "${BLUE}--- Mandatory Part Tests (./pipex) ---${RESET}"
check_test "Basic cat | wc -l" \
    "./pipex" \
    "./pipex $INFILE \"cat\" \"wc -l\" $OUTFILE_PIPEX" \
    "< $INFILE cat | wc -l > $OUTFILE_SHELL"

check_test "grep lorem | wc -w" \
    "./pipex" \
    "./pipex $INFILE \"grep lorem\" \"wc -w\" $OUTFILE_PIPEX" \
    "< $INFILE grep lorem | wc -w > $OUTFILE_SHELL"

# --- BONUS TESTS: MULTIPLE PIPES ---
echo -e "\n${BLUE}--- Bonus Part Tests: Multiple Pipes (./pipex_bonus) ---${RESET}"
check_test "3 Commands: cat | grep i | wc -l" \
    "./pipex_bonus" \
    "./pipex_bonus $INFILE \"cat\" \"grep i\" \"wc -l\" $OUTFILE_PIPEX" \
    "< $INFILE cat | grep i | wc -l > $OUTFILE_SHELL"

check_test "5 Commands: cat | cat | cat | cat | wc -l" \
    "./pipex_bonus" \
    "./pipex_bonus $INFILE \"cat\" \"cat\" \"cat\" \"cat\" \"wc -l\" $OUTFILE_PIPEX" \
    "< $INFILE cat | cat | cat | cat | wc -l > $OUTFILE_SHELL"

# --- BONUS TESTS: HERE_DOC ---
echo -e "\n${BLUE}--- Bonus Part Tests: Here_doc ---${RESET}"
echo "line1" > here_doc_input
echo "line2" >> here_doc_input
echo "LIMITER" >> here_doc_input

echo -e "${YELLOW}Testing: here_doc cat | wc -l${RESET}"
((TOTAL++))
./pipex_bonus here_doc LIMITER "cat" "wc -l" $OUTFILE_PIPEX < here_doc_input > /dev/null
cat << LIMITER | wc -l > $OUTFILE_SHELL
line1
line2
LIMITER

diff $OUTFILE_PIPEX $OUTFILE_SHELL > /dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[PASS]${RESET} here_doc output matches."
    ((PASSED++))
else
    echo -e "${RED}[FAIL]${RESET} here_doc output differs!"
fi
rm -f $OUTFILE_PIPEX $OUTFILE_SHELL here_doc_input

# --- ERROR HANDLING ---
echo -e "\n${BLUE}--- Error Handling Tests (Visual Check) ---${RESET}"
echo -e "${YELLOW}Non-existent infile (Pipex):${RESET}"
./pipex non_existent "cat" "wc" $OUTFILE_PIPEX
echo -e "${YELLOW}Shell equivalent:${RESET}"
< non_existent cat | wc > $OUTFILE_SHELL

# --- MEMORY LEAK CHECK ---
if command -v valgrind &> /dev/null; then
    echo -e "\n${BLUE}--- Memory Leak Check (./pipex_bonus) ---${RESET}"
    valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./pipex_bonus $INFILE "ls" "wc" $OUTFILE_PIPEX 2> valgrind_log
    if grep -q "no leaks are possible" valgrind_log; then
        echo -e "${GREEN}[PASS]${RESET} No memory leaks detected."
    else
        echo -e "${RED}[FAIL]${RESET} Memory leaks found! Check valgrind_log"
    fi
    rm -f valgrind_log
fi

# Summary
echo -e "\n${BLUE}=======================================${RESET}"
echo -e "Summary: ${PASSED}/${TOTAL} Tests Passed"
if [ $PASSED -eq $TOTAL ]; then
    echo -e "${GREEN}PERFECT!${RESET}"
else
    echo -e "${RED}NEEDS WORK!${RESET}"
fi
echo -e "${BLUE}=======================================${RESET}"

# Cleanup
rm -f $INFILE $OUTFILE_PIPEX $OUTFILE_SHELL
