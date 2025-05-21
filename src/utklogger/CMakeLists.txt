# src/utklogger/CMakeLists.txt
# Tool level build file, added conditionally by src/CMakeLists.txt
# defines the 'utklogger' module target, its sources, and settings

## Glob source files
glob_sources(LOGGER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")

## Create the target with globbed source files
if(BUILD_SHARED OR BUILD_IMPORT)
	add_library(utklogger SHARED ${LOGGER_SOURCES})
else()
	add_library(utklogger STATIC ${LOGGER_SOURCES})
endif()

target_include_directories(utklogger PUBLIC "${UTK_HEADERS}")

target_compile_definitions(utklogger PUBLIC UTK_LOGGER)