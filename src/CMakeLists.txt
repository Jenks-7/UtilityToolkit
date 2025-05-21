# src/CMakeLists.txt
# Top-level source directory CMake file
# Conditionally include modules based on options or variables

# Modules enabled via options (make sure options are defined in root CMakeLists)
if(DEFINED UTK_LOGGER)
    list(APPEND UTK_TOOLS "utklogger")
else()
    message(STATUS "UTK_LOGGER module disabled")
endif()

## Apply common compiler flags
set_common_flags()

if(UTK_TOOLS)
    message(STATUS "ENABLED TOOLS: ${UTK_TOOLS}")
    foreach(TOOL IN LISTS UTK_TOOLS)
        add_subdirectory(${TOOL})
    endforeach()
else()
    message(WARNING "No tools enabled in build")
endif()

# Ensure changes are pushed back to parent scope
set(UTK_TOOLS ${UTK_TOOLS} PARENT_SCOPE)