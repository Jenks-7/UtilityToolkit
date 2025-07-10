# CMakeLists.txt
# Root directory CMake file
# Create build definitions and locate dependencies

cmake_minimum_required(VERSION 3.24)

## Set standard project variables
set(UTK "UtilityToolkit")
set(UTK_TEST "UtilityToolkit_Test")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(UTK_HEADERS "${CMAKE_SOURCE_DIR}/include")
set(UTK_CMAKE_SCRIPTS "${CMAKE_SOURCE_DIR}/scripts/cmake")
set(UTK_TOOLS "")

project(${UTK} LANGUAGES CXX C)

## Include required scripts
list(APPEND CMAKE_MODULE_PATH "${UTK_CMAKE_SCRIPTS}")
include(PythonInfo)
include(CommonFlags)
include(GlobFiles)

if(NOT DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND WIN32)
	set(CMAKE_INSTALL_PREFIX "C:\\${UTK}" CACHE PATH "Install path" FORCE)
endif()

## Options when added/decided upon
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(UTK_LOGGER "Add debug logger to the toolkit build output" ON)

## Check requirements
if(PYTHON_REQUIRED)
	find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
	print_python_info()
endif()

if(BOOST_REQUIRED)
	message(STATUS "[Necessary boost build script or steps here], either script or find_package & vcpkg or hardcoded on needs")
	### Update these in time to work with the later tools ###
	message(STATUS "BOOST ROOT: [Boost_ROOT]")
	message(STATUS "BOOST FOUND: [Boost_VERSION]")
	message(STATUS "BOOST LIBRARIES: [Boost_LIBRARIES]")
	message(STATUS "BOOST DIRECTORIES [Boost_INCLUDE_DIRS]")
	message(STATUS "BOOST LIBRARIES DIR: [Boost_LIBRARY_DIRS]")
	message(STATUS "BOOST BINARIES: [Boost_LIBRARY_DIR_RELEASE]")
endif()

if(MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

## Project Variables
message(STATUS "PROJECT_NAME: ${UTK}")
message(STATUS "TEST_NAME: ${UTK_TEST}")

if(WIN32)
	message(STATUS "TOOLCHAIN: ${WINDOWS_DEPENDENCY_TOOLCHAIN}")
endif()

message(STATUS "CMAKE_SYSTEM_PREFIX_PATH: ${CMAKE_SYSTEM_PREFIX_PATH}")
message(STATUS "SYSTEM_MODULE_PATH: ${SYSTEM_MODULE_PATH}")
message(STATUS "CMAKE_GENERATOR_PLATFORM: ${CMAKE_GENERATOR_PLATFORM}")
message(STATUS "GTEST REPOSITORY: ${GTEST_REPOSITORY}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

## Include sub-projects to build individual tools/modules and then test executable seperately(eventually).
add_subdirectory(src)

## Install logic for headers and binaries
install(
    DIRECTORY ${UTK_HEADERS}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ${UTK_TOOLS}
        EXPORT UtilityToolkitTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)