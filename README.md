# Utility Toolkit

<img width="1536" height="618" alt="banner-smaller" src="https://github.com/user-attachments/assets/96190cb0-3385-431f-8e89-93d74c7689b2" />

## Overview
A modular suite of C++ developer tools leveraging the STL and Boost for lightweight JSON handling and scripting support. Built to streamline test infrastructure, automate project workflows, and support future extensions for engineering and diagnostic workflows.

## Development Status
🚧 This project is under active development. Expect breaking changes and incomplete modules until v1.0.0 is tagged.

## To-do
- Create usage documentation and UML documentation of individual tools
- Interoperability layer for surface level tools and lib features
- UTK::Logger(Remaining logger types)
- UTK::ScriptEngine
- UTK::Peripheral(hardware comms)
- UTK::OPCUA(Wrapper around open62541 or using boost::beats to create OPC servers)

## Considerations
- UTK::Client(Simple web client for fetching data)
- UTK::Caching(LRUCache and other caching options or similar data structures for use in creating advanced tools)
- UTK::Progressbar(For Livelier CLI apps)
- Hasing/Code generation tools:
  - UTK::Barcode(or QR-code generation)
  - UTK::Hash(For quick cryptography. NOT RECOMMENDED DATA SECURITY UNTIL SUFFICIENT TESTING)
  - UTK::UUID(UUID/GUID generation for item tracking and tieing in A numbers etc.)
  - UTK::Random

## Features
- Modular tool-based architecture
- Logger utility (initial module)
- JSON configuration and scripting support (planned)
- Build and run scripts within C/C++ envs
- Integration-ready with CMake based systems
- Interoperability layer for C

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
- C Interop layer for top layer of API. To enable core tooling usage in C 

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
```


