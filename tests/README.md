# Tests
This directory contains source files for testing Utility Toolkit components.

All tests are written using **GoogleTest** and are integrated into the CMake build system.

## Structure
Each test corresponds to a specific module or component and follows a one-to-one naming convention for clarity.

Example:
- `logger_test.cpp` – Tests for the logging utility

## Running Tests
Tests are registered with **CTest**. After building the project, you can run them via:

```bash
ctest --output-on-failure
```
