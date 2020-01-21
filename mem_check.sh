#!/usr/bin/bash

VALGRIND_OPS="--leak-check=full --error-exitcode=15 --show-leak-kinds=all --errors-for-leak-kinds=all"

if (($# < 1)); then
  echo "usage: ${0} <executable>"
  exit 1
fi

if ! test -f "memory.in"; then
  echo "memory.in needed as input"
  exit 1
fi

executable="${1}"
not_reached=$((0))
not_reached_limit=$((6))
tests_number=$((1000000))
out_file="/tmp/mem_check.out"

valgrind $VALGRIND_OPS "${executable}" <memory.in &>"${out_file}"
exitcode=$?
if ((exitcode != 0)); then
  echo "Doesn't work with mallocs working, try again."
  exit 1
fi

function test_once() {
  valgrind $VALGRIND_OPS "${executable}" "${1}" $2 <memory.in &>"${out_file}"
  exitcode=$?
  if ((exitcode != 0)); then
    echo "Program failed with exitcode $exitcode. Command:"
    echo valgrind $VALGRIND_OPS "${executable}" "${1}" $2 '<'memory.in '&>'"${out_file}"
    echo "press Enter to continue"
    read -r # comment this to not pause
  fi

  grep "MOCK ALLOC REACHED" "${out_file}" &>/dev/null
  reached=$?
  if ((reached != 0)); then
    echo "Mock alloc not reached."
    not_reached=$((not_reached + 1))
  fi
}

for i in $(seq 0 $tests_number); do
  echo "Test: ${i}."

  test_once "${i}" ""
  test_once "${i}" "--fail-all-after"

  if ((not_reached >= not_reached_limit)); then
    break
  fi
done

echo "Done ${i} tests and in ${not_reached} tests (mock alloc was not reached == all mallocs were covered)."
