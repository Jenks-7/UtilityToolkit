# src/utklogger/CMakeLists.txt
# Tool level build file, added conditionally by src/CMakeLists.txt
# defines the 'utklogger' module target, its sources, and settings

## Glob source files
glob_sources(LOGGER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")

# Create library target
add_library(utklogger ${LOGGER_SOURCES})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(utklogger
        PRIVATE UTK_BUILD_EXPORT
        INTERFACE UTK_BUILD_IMPORT
    )
endif()

# Include directories — accessible to consumers
target_include_directories(utklogger
    PUBLIC
        $<BUILD_INTERFACE:${UTK_HEADERS}>
        $<INSTALL_INTERFACE:include>
)