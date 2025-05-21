# Scripts

This directory contains Python and CMake utility scripts used for project setup, automation, and maintenance tasks.

These tools are **not part of the core Utility Toolkit runtime**, but assist in development and infrastructure management.

---

## Structure

| Directory | Purpose                                                  |
|-----------|----------------------------------------------------------|
| `py`      | Python scripts for module generation, JSON editing, etc. |
| `cmake`   | CMake helper scripts for globs, custom logic, etc.       |

## Python Scripts (`py/`)
Run Python scripts using: `python3 scripts/py/<script_name>.py` or through `utk_dashboard.py`

## CMake Scripts (`cmake/`)

These are `.cmake` files used by the build system to:
- Automatically glob headers and sources
- Inject toolchain configs or reusable macros
- Simplify repetitive CMake logic

They are included by the top-level `CMakeLists.txt` as needed.

## Notes
- Python scripts require Python 3.10+
- These scripts are not required to build the project but simplify tasks such as:
  - Artifact cleanup
  - Providing toolkit diagnostics
  - Adding new tools/modules to the toolkit
  - Edit cmake settings without needing to trawl through the settings file
