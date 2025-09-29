# src/utkdispatch/CMakeLists.txt
# Tool level build file, added conditionally by src/CMakeLists.txt
# defines the 'utkdisptach' tool target, its sources, and settings

## Glob source files
glob_sources(DISPATCH_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")

# Create library target
add_library(utkdispatch ${DISPATCH_SOURCES})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(utkdispatch
        PRIVATE UTK_BUILD_EXPORT
        INTERFACE UTK_BUILD_IMPORT
    )
endif()

# Include directories — accessible to consumers
target_include_directories(utkdispatch
    PUBLIC
        $<BUILD_INTERFACE:${UTK_HEADERS}>
        $<INSTALL_INTERFACE:include>
)