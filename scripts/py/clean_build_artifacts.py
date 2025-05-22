import shutil
from pathlib import Path

def clean_build_artifacts() -> None:
    dirs = ["bin", "lib"]
    project_root = Path(__file__).resolve().parents[2]

    for d in dirs:
        dir_path = project_root / d
        if dir_path.exists() and dir_path.is_dir():
            shutil.rmtree(dir_path)
            print(f"Deleted: {dir_path}")
        else:
            print(f"{dir_path} does not exist")

if __name__ == "__main__":
    clean_build_artifacts()
    print()