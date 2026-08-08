#!/usr/bin/env bash
set -e

cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++