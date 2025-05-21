# Utility Toolkit

## Overview
A modular suite of C++ developer tools leveraging the STL and Boost for lightweight JSON handling and scripting support. Built to streamline test infrastructure, automate project workflows, and support future extensions for engineering and diagnostic workflows.

## Development Status
🚧 This project is under active development. Expect breaking changes and incomplete modules until v1.0.0 is tagged.

## Features
- Modular tool-based architecture
- Logger utility (initial module)
- JSON configuration and scripting support (planned)
- Build and automation scripts via Python
- Integration-ready with CMake-based systems

## Project Structure
| Location    | Description                                                                 |
|-------------|-----------------------------------------------------------------------------|
| examples    | Example usage of toolkit modules and build setups for reference             |
| include     | Toolkit headers, organized by tool (e.g., logger, core, types)              |
| scripts     | Python and CMake utility scripts to support automation and project setup    |
| src         | Core implementation source files, organized by tool                         |
| tests       | Contains GoogleTest-based test sources for validating toolkit modules       |
| third-party | External libraries and precompiled dependencies used by the toolkit         |

## Planned Modules
- OPC-UA tooling
- Script engine integration
- Enhanced test utilities (auto-generators, stubs)
- Settings/configuration interface

## Getting Started
Utility Toolkit uses a CMake-based build system and will be built automatically via CI/CD pipelines.

> ✅ Builds and tests are automatically run through CI on each commit.  
> 🛠️ Use the following steps only if you want to build locally.

### Local Build Instructions

- Prerequisites: CMake, Python 3.10+, Boost (minimal), a C++20-compatible compiler

```bash
git clone https://github.com/yourname/utility-toolkit.git
cd utility-toolkit
cmake -S . -B build
cmake --build build
