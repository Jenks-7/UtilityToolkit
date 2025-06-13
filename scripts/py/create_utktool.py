"""

"""

from typing import Optional
from datetime import date
from pathlib import Path
import shutil

PROJECT_ROOT = Path(__file__).parents[2]
HEADER_DIR = PROJECT_ROOT / "include"
SOURCE_DIR = PROJECT_ROOT / "src" 

hdr_template = \
"""//===================================================================================================================================
// @file    {FILENAME}.hpp
// @author  {AUTHOR}
// @date    {DATE}
// 
// @brief   {DESCRIPTION}
//===================================================================================================================================

#pragma once

// TODO: Add your headers here

namespace {NAMESPACE_SEQUENCE}

"""

def create_list_file() -> None:
    return None

def create_utktool(tool_name: str, existing_tool: Optional[str] = None) -> None:

    if existing_tool:
        TOOL_HDR_DIR = HEADER_DIR / existing_tool / tool_name
        TOOL_SRC_DIR = SOURCE_DIR / existing_tool / ("utk" + tool_name)

        if TOOL_SRC_DIR.exists() or TOOL_HDR_DIR.exists():
            raise FileExistsError(f"Subtool '{tool_name}' already exists under '{existing_tool}'.")

        TOOL_HDR_DIR.mkdir(parents=True)
        TOOL_SRC_DIR.mkdir(parents=True)
    else:
        TOOL_HDR_DIR = HEADER_DIR / tool_name
        TOOL_SRC_DIR = SOURCE_DIR / ("utk" + tool_name)

        if TOOL_SRC_DIR.exists() or TOOL_HDR_DIR.exists():
            raise FileExistsError(f"Tool '{tool_name}' already exists.")

        #TOOL_HDR_DIR.mkdir(parents=True)
        #TOOL_SRC_DIR.mkdir(parents=True)

    hdr_path = TOOL_HDR_DIR / f"{tool_name}.hpp"
    src_path = TOOL_SRC_DIR / f"{tool_name}.cpp"

    namespace_sequence = f"UTK::{existing_tool}::{tool_name}" if existing_tool else f"UTK::{tool_name}"
    namespace_sequence += (" {\n\tTODO: Add tool implementation here\n}")

    hdr_content = hdr_template.format(
        FILENAME=tool_name,
        AUTHOR=tool_name,
        DATE=date.today().strftime("%d/%m/%Y"),
        DESCRIPTION="Temp description",
        NAMESPACE_SEQUENCE=namespace_sequence
    )

    #with  open(f"{hdr_path}", "w") as hdr_file, open(f"{src_path}.", "w") as src_file:
    try:
        print(hdr_content)
        #hdr_file.write(hdr_template)
    except OSError:
        print("Failed to read from file")

if __name__ == "__main__":
    import  argparse

    parser = argparse.ArgumentParser(
        description="Create a new tool or add a subtool to an existing tool."
                    "Provide just a tool name to create a new tool."
                    "Use --existing-tool to add a subtool."
    )
    parser.add_argument(
        "tool_name",
        type=str,
        help="Name of the tool to create (or the subtool to add)"
    )
    parser.add_argument(
        "--existing-tool",
        type=str,
        default=None,
        help="Name of the existing tool to add this as a subtool"
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Enable verbose output"
    )
     
    args = parser.parse_args()
     
    ## Input sanitize here
    new_name = str(args.tool_name).lower()
    existing_tool = str(args.existing_tool or "").lower()

    tool_name = ''.join(c for c in new_name if c.isalpha())
    existing_tool = ''.join(c for c in existing_tool if c.isalpha())

    if not tool_name:
        raise ValueError("Tool name must contain at least one valid alphabetic character")

    create_utktool(tool_name, existing_tool if existing_tool else None)

