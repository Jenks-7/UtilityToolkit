# Third-Party Dependencies

## Overview
This directory contains **external tools and libraries** that are either:

- Difficult to integrate directly via package managers
- Provided as **pre-built binaries** for convenience
- Intended to be built separately and optionally injected into the project

These dependencies are **not required to build Utility Toolkit by default**, but may be used to extend or enhance functionality (e.g., protocol libraries, diagnostic backends, middleware tools).

---

## Structure
| Subdirectory | Purpose                                                                              |
|--------------|--------------------------------------------------------------------------------------|
| `binaries`   | Pre-compiled third-party libraries and tools used optionally by the project          |
| `source`     | Full or partial source trees of external libraries that may be rebuilt or referenced |
| `docs`       | Setup and integration instructions for specific third-party packages                 |

---

## Intended Use
This directory is **not for package-managed libraries** like Boost or Python, which are handled via the toolchain (e.g., `vcpkg`, environment detection, etc.).

Instead, it is used for:
- **Loosely coupled external tools**
- Libraries that require **manual build or configuration** outside CMake
- Tools that may not follow CMake conventions or are meant to be used as standalone binaries

---

## Notes
- This directory is excluded from version control except for installation guides and helper scripts.
- Users may place platform-specific binaries or source trees here for local builds.
- Ensure that any included binaries or source files comply with their respective licenses.
