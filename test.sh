#! /bin/bash

set -euo pipefail

# Rebuild project
cmake --build build

# Run tests with output
ctest --test-dir build --output-on-failure