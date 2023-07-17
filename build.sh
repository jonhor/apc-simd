#!/usr/bin/env sh

FLAGS="--std=c++17 -O2 -mavx2"

SCRIPT_DIR=$(dirname $(realpath "$0"))
SRC_DIR="${SCRIPT_DIR}/src"
OUT_DIR="${SCRIPT_DIR}/out"


mkdir -p "$OUT_DIR"

g++ $FLAGS "${SRC_DIR}/asm/add.cpp" -o "${OUT_DIR}/add_inline_assembly"
g++ $FLAGS "${SRC_DIR}/intrinsics/bench.cpp" "${SRC_DIR}/intrinsics/find.cpp" -o "${OUT_DIR}/find_benchmark"
g++ $FLAGS -c "${SRC_DIR}/autovec/add.cpp" -o "${OUT_DIR}/add.o"
objdump -drwC "${OUT_DIR}/add.o" > "${OUT_DIR}/add_autovec.asm"
g++ $FLAGS "${OUT_DIR}/add.o" -o "${OUT_DIR}/add_autovec"
rm "${OUT_DIR}/add.o"
