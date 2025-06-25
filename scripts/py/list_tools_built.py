# ==================================================================================================
# @file        list_tools_built.py
# @brief       Lists all currently built tools or modules in the UTK project.
# @author      Jac Jenkins
# @date        11/06/2025
#
# @description
#   Scans the include/ and src/ directories to find defined UTK tools and generates a list
#   for display or export. Supports integration with project dashboard or summaries.
# ==================================================================================================

from typing import List, Tuple, Optional
from pathlib import Path

def get_built_modules_only() -> List[str]:
    """
    Return a list of built module names (without the '.lib' extension).

    Checks for built libraries in either the 'Debug' or 'Release' subdirectories
    under the 'lib' directory located two levels above the current file.

    Returns:
        List[str]: List of module names found in the first existing output directory.
                   Returns an empty list if no modules are found.
    """
    
    lib_dir = Path(__file__).resolve().parents[2] / "lib"
    output_dirs = ("Debug", "Release")

    for subdir in output_dirs:
        full_path = lib_dir / subdir
        if full_path.exists():
            return [file.name.removesuffix(".lib") for file in full_path.iterdir() if file.is_file() and file.suffix == ".lib"]
    return []

def get_built_modules() -> Tuple[List[str], Optional[str]]:
    """
    Return a tuple containing a list of built module names and the build type directory.

    Checks for built libraries in the 'Debug' or 'Release' subdirectories under the
    'lib' directory two levels above the current file.

    Returns:
        Tuple[List[str], Optional[str]]: A tuple where the first element is a list of
        module names (without the '.lib' extension), and the second element is the
        build type string ('Debug' or 'Release') where modules were found. Returns
        ([], None) if no modules are found.
    """

    lib_dir = Path(__file__).resolve().parents[2] / "lib"
    output_dirs = ("Debug", "Release")

    for subdir in output_dirs:
        full_path = lib_dir / subdir
        if full_path.exists():
            return [file.name.removesuffix(".lib") for file in full_path.iterdir() if file.is_file() and file.suffix == ".lib"], subdir
    return [], None

def print_built_modules(tools: List[str], build_type: Optional[str]) -> None:
    """
    Print a list of built module names and their build type.

    Args:
        tools (List[str]): List of built module names.
        build_type (Optional[str]): Build type directory name ('Debug' or 'Release').
                                   Can be None if no modules are found.

    Returns:
        None
    """

    if not tools:
        print("No tools found! Rebuild or check build location.")
    else:
        print(f"Tools built in '{build_type}':")
        for name in tools:
            print(f"  - {name}")

if __name__ == "__main__":
    modules, build_type = get_built_modules()
    print_built_modules(modules, build_type)