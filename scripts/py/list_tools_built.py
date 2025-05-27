from pathlib import Path
from typing import List, Tuple, Optional

def get_built_modules_only() -> List[str]:
    lib_dir = Path(__file__).resolve().parents[2] / "lib"
    output_dirs = ("Debug", "Release")

    for subdir in output_dirs:
        full_path = lib_dir / subdir
        if full_path.exists():
            return [file.name.removesuffix(".lib") for file in full_path.iterdir() if file.is_file() and file.suffix == ".lib"]
    return []

def get_built_modules() -> Tuple[List[str], Optional[str]]:
    lib_dir = Path(__file__).resolve().parents[2] / "lib"
    output_dirs = ("Debug", "Release")

    for subdir in output_dirs:
        full_path = lib_dir / subdir
        if full_path.exists():
            return [file.name.removesuffix(".lib") for file in full_path.iterdir() if file.is_file() and file.suffix == ".lib"], subdir
    return [], None

def print_built_modules(tools: List[str], build_type: Optional[str]) -> None:
    if not tools:
        print("No tools found! Rebuild or check build location.")
    else:
        print(f"Tools built in '{build_type}':")
        for name in tools:
            print(f"  - {name}")

if __name__ == "__main__":
    modules, build_type = get_built_modules()
    print_built_modules(modules, build_type)