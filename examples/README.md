# Examples

This directory contains usage examples and test applications for the Utility Toolkit modules.

Each example demonstrates how to integrate and use a specific feature or tool in isolation or combination.

## Available Examples

- **logger_example.cpp** – Demonstrates basic usage of the logging utility

## Building

All examples are built as part of the main CMake project when `BUILD_EXAMPLES` is enabled (if applicable).

```bash
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build
```
