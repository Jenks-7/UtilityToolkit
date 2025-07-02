# ==================================================================================================
# @file        create_tool.py
# @brief       Command-line tool for generating UTK header and source file structures.
# @author      Jac Jenkins
# @date        25/06/2025
# 
# @description
#   This script creates the basic structure for a new UTK tool, including its header and
#   source files, under the appropriate directory tree. Optionally supports nesting under
#   an existing tool as a subtool. File names, namespaces, and headers are automatically
#   populated.
# ==================================================================================================

from typing import Optional
from datetime import date
from pathlib import Path
import shutil

# Global variables 
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

src_template = \
"""//===================================================================================================================================
// @file    {FILENAME}.cpp
// @author  {AUTHOR}
// @date    {DATE}
// 
// @brief   {DESCRIPTION}
//===================================================================================================================================

// TODO: Add your headers here

// TODO: Begin code implementation here
"""

# Helper functions to format the templates string above
def _create_header_template(tool_name: str, author_name: str, namespace_sequence: str) -> str:
    return hdr_template.format(
        FILENAME="utk" + tool_name,
        AUTHOR=author_name,
        DATE=date.today().strftime("%d/%m/%Y"),
        DESCRIPTION="ADD IN DESCRIPTION",
        NAMESPACE_SEQUENCE=namespace_sequence
    )

def _create_source_template(tool_name: str, author_name: str) -> str:
    return src_template.format(
        FILENAME="utk" + tool_name,
        AUTHOR=author_name,
        DATE=date.today().strftime("%d/%m/%Y"),
        DESCRIPTION="ADD IN DECRIPTION",
    )

def create_list_file() -> None:
    return None


def create_utktool(tool_name: str, author_name: str, existing_tool: Optional[str] = None) -> None:
    """
    Creates the header and source file layout for a new UTK tool.

    If an existing tool is specified, the new tool is added as a subtool.
    Otherwise, it is treated as a standalone top-level module. Tool directories
    and initial template files are generated, with protection against overwriting
    existing files.

    Parameters:
        tool_name (str): Name of the tool to create.
        author_name (str): Name of the author for use in generated file headers.
        existing_tool (Optional[str]): Parent tool name if creating a subtool.
    Raises:
        FileExistsError: If a tool or subtool with the same name already exists.
    Returns:
        None
    """

    # The tool is added as a sub tool of an existing tool, otherwise it is treated as a top level tool
    if existing_tool:
        
        # Create subtool path
        TOOL_HDR_DIR = HEADER_DIR / existing_tool / tool_name
        TOOL_SRC_DIR = SOURCE_DIR / existing_tool / ("utk" + tool_name)

        # To prevent overwriting of existing tools
        if TOOL_SRC_DIR.exists() or TOOL_HDR_DIR.exists():
            raise FileExistsError(f"Subtool '{tool_name}' already exists under '{existing_tool}'.")

        # Add subtool to an existing utktool path
        TOOL_HDR_DIR.mkdir(parents=True)
        TOOL_SRC_DIR.mkdir(parents=True)
    else:
        
        # Create utktool path
        TOOL_HDR_DIR = HEADER_DIR / tool_name
        TOOL_SRC_DIR = SOURCE_DIR / ("utk" + tool_name)

        # To prevent overwriting of existing tools
        if TOOL_SRC_DIR.exists() or TOOL_HDR_DIR.exists():
            raise FileExistsError(f"Tool '{tool_name}' already exists.")

        # Add tool directories to project path
        #TOOL_HDR_DIR.mkdir(parents=True)
        #TOOL_SRC_DIR.mkdir(parents=True)

    # Add files to path object
    hdr_path = TOOL_HDR_DIR / f"{tool_name}.hpp"
    src_path = TOOL_SRC_DIR / f"{tool_name}.cpp"

    # Create UTK namespace string to add code definitions to in the header file
    namespace_sequence = f"UTK::{existing_tool}::{tool_name}" if existing_tool else f"UTK::{tool_name}"
    namespace_sequence += ("{\n\tTODO: Add tool implementation here\n}")

    # Fill in hdr template
    hdr_content = _create_header_template(tool_name, author_name, namespace_sequence)
    src_content = _create_source_template(tool_name, author_name)

    # Make this a function
    #with  open(f"{hdr_path}", "w") as hdr_file, open(f"{src_path}.", "w") as src_file:
    try:
        if args.verbose:
            print("📄 \033[4mHeader Layout\033[0m:\n" + hdr_content)
            print("🧩 \033[4mSource Layout\033[0m:\n" + src_content)
        #hdr_file.write(hdr_template)
    except OSError:
        print("Failed to read from file")

if __name__ == "__main__":
    import  argparse

    # Create tool description and argument input
    parser = argparse.ArgumentParser(
        description="Create a new tool or add a subtool to an existing tool."
                    "Provide just a tool name to create a new tool."
                    "Use --existing-tool to add a subtool."
    )
    
    # Create tool name input arg
    parser.add_argument(
        "tool_name",
        type=str,
        help="Name of the utktool to create or the subtool to add"
    )
    
    # Create author name input arg
    parser.add_argument(
        "--author_first",
        type=str,
        help="Name of author to include in the tools file headers"
    )

    # Create author first surname input arg
    parser.add_argument(
        "--author_last",
        type=str,
        help="Lastname of author to include in the tools file headers"
    )
    
    # Option to specify an existing utktool, the new tool can be placed under
    parser.add_argument(
        "--existing_tool",
        type=str,
        default=None,
        help="Name of the existing utktool to add subtool under"
    )
    
    # Option to enable verbose output
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Enable verbose output"
    )
     
    args = parser.parse_args()
     
    ## Input sanitize here
    new_tool_name = str(args.tool_name).strip().capitalize()
    existing_tool = str(args.existing_tool or "").strip().capitalize()
    author_first = str(args.author_first or "").strip().capitalize()
    author_lastname = str(args.author_last or "").strip().capitalize()

    # Create author full name from options
    author_name = f"{author_first} {author_lastname}".strip()

    # Extra step to clear up any invalid toolname characters
    tool_name = ''.join(c for c in new_tool_name if c.isalpha())
    existing_tool = ''.join(c for c in existing_tool if c.isalpha())

    if not tool_name:
        raise ValueError("Tool name must contain at least one valid alphabetic character")

    create_utktool(tool_name, author_name, existing_tool if existing_tool else None)

