# Merlin (The Utility Toolkit)

---

## Overview
**Merlin** is a modular systems utility toolkit designed for embedded control, data automation, and infrastructure engineering. Built on a foundation of modern C++ and leveraging the STL and Boost, it streamlines testing infrastructure, scripting workflows, and future diagnostic extensions.

At its core, **Merlin** is implemented as the `UtilityToolkit` — or **UTK** — a clean and efficient C++ library offering robust, low-level tools including logging, caching, scripting engines, and industrial protocol support.

> *"Merlin shapes systems from within and empowers engineers without."*

Whether you know it by its functional name (`UTK`) or its identity (`Merlin`), this toolkit serves as your tome of utilities — a repository of engineering wisdom, precision abstractions, and system design fundamentals.

## Development Status
🚧 This project is under active development. Expect breaking changes and incomplete modules until v1.0.0 is tagged.

## To-do
- Create usage documentation and UML documentation of individual tools  
- Interoperability layer for surface level tools and lib features  
- `UTK::Logger` (Remaining logger types)  
- `UTK::ScriptEngine`  
- `UTK::Peripheral` (For hardware comms)  
- `UTK::OPCUA` (For IoT, or similar data historization and gathering tools)

## Considerations
- `UTK::Client` (Simple web client for fetching data)  
- `UTK::Caching` (LRUCache and other caching options or similar data structures for use in creating advanced tools)  

---

**Hashing/Code generation tools:**
- `UTK::Barcode` (or QR-code generation)  
- `UTK::Hash` (For quick cryptography. **Not recommended for data security until sufficient testing**)  
- `UTK::UUID` (UUID/GUID generation for item tracking and tying in A-numbers etc.)  
- `UTK::Random`

## Features
- Modular tool-based architecture  
- Logger utility (initial module)  
- JSON configuration and scripting support (planned)  
- Build and run scripts within C/C++ envs  
- Integration-ready with CMake based systems  
- Interoperability layer for C  

## Project Structure
| Location       | Description                                                              |
|----------------|--------------------------------------------------------------------------|
| `examples`     | Example usage of toolkit modules and build setups for reference          |
| `include`      | Toolkit headers, organized by tool (e.g., logger, core, types)           |
| `scripts`      | Python and CMake utility scripts to support automation and project setup |
| `src`          | Core implementation source files, organized by tool                      |
| `tests`        | Contains GoogleTest-based test sources for validating toolkit modules    |
| `third-party`  | External libraries and precompiled dependencies used by the toolkit      |

## Planned Modules
- OPC-UA tooling  
- Script engine integration  
- Enhanced test utilities (auto-generators, stubs)  
- Settings/configuration interface  
- C Interop layer for top layer of API. To enable core tooling usage in C  

## Getting Started
Utility Toolkit uses a CMake-based build system and will be built automatically via CI/CD pipelines.

✅ Builds and tests are automatically run through CI on each commit.  
🛠️ Use the following steps only if you want to build locally.

---

### Local Build Instructions
**Prerequisites**: CMake, Python 3.10+, Boost (minimal), a C++20-compatible compiler

```sh
git clone https://github.com/yourname/utility-toolkit.git
cd utility-toolkit
cmake -S . -B build
cmake --build build
