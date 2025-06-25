# ==================================================================================================
# @file        get_project_info.py
# @brief       Gathers and summarizes metadata about the UTK project configuration and structure.
# author       Jac Jenkins
# @date        25/05/2025
#
# @description
#   Retrieves project-level information such as project name, version, last build date, and
#   active tools. This script supports dashboard displays and status reporting tools.
# ==================================================================================================


import sys, os, platform, subprocess
from datetime import datetime
from pathlib import Path
from typing import Dict

def get_project_info() -> Dict[str, dict]:
    return {
        "local": get_local_info(),
        "git": get_git_info(),
        "python": get_py_info(),
        "platform": get_platform_info()
    }

def get_local_info() -> dict:

    def get_last_build_timestamp(build_dir_type: str) -> str:
        build_marker = Path(__file__).resolve().parents[2] / "lib" / build_dir_type.title()

        if not build_marker.exists():
            return f"Lib directory 'lib/{build_dir_type}' not present"
    
        return datetime.fromtimestamp(build_marker.stat().st_mtime).strftime("%Y-%m-%d %I:%M")

    return {
        "project_name": "Utility Toolkit",
        "last_release": get_last_build_timestamp("Release"),
    }

def get_git_info() -> dict:
    
    def run(cmd: list[str]) -> str:
        try:
            result = subprocess.run(["git"] + cmd, capture_output=True, text=True, check=True)
            return result.stdout.strip()
        except subprocess.CalledProcessError:
            return "Unknown"

    if not (Path(__file__).resolve().parents[2] / ".git").exists():
        return {
            "branch": "Local copy uninitialized. Make sure to create one via 'git clone'!",
            "last_hash": None,
            "last_commit": None,
            "changes": False,
            "tag": None,
            "remote_url": None
        }

    return {
        "branch": run(["rev-parse", "--abbrev-ref", "HEAD"]),
        "last_hash": run(["rev-parse", "--short", "HEAD"]),
        "last_commit": run(["log", "-1", "--pretty=%s"]),
        "changes": bool(run(["status", "--porcelain"])),   # True if there are uncommitted changes
        "tag": run(["describe", "--tags", "--abbrev=0"]),
        "remote_url": run(["config", "--get", "remote.origin.url"])
    }

def get_py_info() -> dict:
    return {
        "version": platform.python_version(),
        "implementation": platform.python_implementation(),
        "compiler": platform.python_compiler(),
        "build": platform.python_build()[0],
        "path": Path(sys.executable).resolve().parent
    }

def get_platform_info() -> dict:
    return {
        "os": platform.system(),
        "os_version": platform.version(),
        "arch": platform.machine(),
        "platform": platform.platform(),
        "shell": os.environ.get("COMSPEC") if platform.system() == "Windows" else os.environ.get("SHELL", "Unknown")
    }

if __name__ == "__main__":
    info_json = get_project_info()

    def print_header(title: str) -> None:
        print(f"{title} info:\n".title() + "-" * (len(title) + 6))

    print()
    
    for key, value in info_json.items():
        print_header(key)

        for subkey, subvalue in value.items():
            print(f"  -{subkey}: {subvalue}")
        
        print()