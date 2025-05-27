import sys, os, platform
from pathlib import Path
from typing import Dict

def get_local_project_info() -> Dict[str, dict]:
    return {
        "project_info": __get_project_info(),
        "git_info": __get_git_info(),
        "python": __get_py_info(),
        "platform": __get_platform_info()
    }

def __get_project_info() -> dict:
    return {
        
    }

def __get_git_info() -> dict:
    return {
        
    }

def __get_py_info() -> dict:
    return {
        
    }

def __get_platform_info() -> dict:
    return {
        "os": platform.system(),
        "os_version": platform.version(),
        "arch": platform.machine(),
        "platform": platform.platform(),
        "shell": os.environ.get("COMSPEC") if platform.system() == "Windows" else os.environ.get("SHELL", "Unknown")
    }

if __name__ == "__main__":
    import json
    print(json.dumps(get_local_project_info(), indent = 2))