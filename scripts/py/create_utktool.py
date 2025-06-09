from pathlib import Path
import shutil

PROJECT_ROOT = Path(__file__).parents[2]
HEADER_DIR = PROJECT_ROOT / "include"
SOURCE_DIR = PROJECT_ROOT / "src"

def create_utktool(tool_name: str) -> None:
    TOOL_HDR_DIR = HEADER_DIR / tool_name
    TOOL_SRC_DIR = SOURCE_DIR / tool_name

    TOOL_HDR_DIR.mkdir(parents=True, exist_ok=True)
    TOOL_SRC_DIR.mkdir(parents=True, exist_ok=True)

    #with open(f"{tool_name}.", "w") as hdr_file:


if __name__ == "__main__":
    import  argparse

    parser = argparse.ArgumentParser(description="Create a new tool")
    parser.add_argument("tool_name", type=str, help="Name of tool to create")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose output")

    args = parser.parse_args()

    ## Input sanitize here

    create_utktool(str(args.tool_name))

