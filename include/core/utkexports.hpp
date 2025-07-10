//===================================================================================================================================
// @file	utkexports.hpp
// @author	Jac Jenkins 
// @date	12/05/2025
// 
// @brief   Header file containing the export defs for Utility Toolkit
//===================================================================================================================================

#pragma once

#ifndef UTK_PLATFORM_CONFIG_H
#define UTK_PLATFORM_CONFIG_H

#define UTK_VER_MAJOR 1
#define UTK_VER_MINOR 0
#define UTK_VER_PATCH 0

#define UTK_VERSION ((UTK_VER_MAJOR * 10000) + (UTK_VER_MINOR * 100) + UTK_VER_PATCH)
#define UTK_VERSION_STRING "1.0.0"

//===================================================================================================================================
//														  ARCHITECTURE DETECTION
//===================================================================================================================================

#if defined(__x86_64) || defined(_M_X64) || defined(__amd64__)
	#define ARCH_x64
	#define NON_ARM_ARCH 1
	#define ARM_ARCH 0
#elif defined(__i386) || defined(_M_IX86)
	#define ARCH_x86
	#define NON_ARM_ARCH 1
	#define ARM_ARCH 0
#elif defined(__aarch64__) || defined(_M_ARM64)
	#define ARCH_ARM64
	#define NON_ARM_ARCH 0
	#define ARM_ARCH 1
#elif defined(__arm__) || defined(_M_ARM)
	#define ARCH_ARM32
	#define NON_ARM_ARCH 0
	#define ARM_ARCH 1
#endif

//===================================================================================================================================
//												      LANGUAGE & ENVIRONMENT DETECTION
//===================================================================================================================================

#if defined(_MSC_VER)
	#define __WINDOWS__ 1
#elif defined(__linux__)
	#define __LINUX__ 1
#endif

#if defined(__WINDOWS__)
	#if defined(_MSVC_LANG)
		#if _MSVC_LANG < 202002L
		#error "This program requires C++20 or higher. Please enable C++20 in your compiler settings."
		#endif
	#endif
#elif defined(__LINUX__) || defined(__APPLE__)
	#if defined(__cplusplus)
		#if __cplusplus < 202002L
		#error "This program requires C++20 or higher. Please enable C++20 in your compiler settings."
		#endif
	#endif
#endif

//===================================================================================================================================
//												         API & EXPORT DEFINITIONS
//===================================================================================================================================

#if defined(__WINDOWS__)
	#if defined(UTK_BUILD_EXPORT)
		#define UTK_API __declspec(dllexport)
	#elif defined(UTK_BUILD_IMPORT)
		#define UTK_API __declspec(dllimport)
	#else
		#define UTK_API
	#endif
	#define UTK_API_CALL __stdcall
#else
	#define UTK_API
	#define UTK_API_CALL
#endif

//===================================================================================================================================
//														  UTK TOOL DEFINITIONS
//===================================================================================================================================

#ifdef UTK_LOGGER
#define UTK_LOGGER_ENABLED 1
#endif

#endif