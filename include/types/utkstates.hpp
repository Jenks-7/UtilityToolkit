//===================================================================================================================================
// @file	utkstates.hpp
// @author	Jac Jenkins
// @date	19/09/2024
// 
// @brief   This file contains the definitions of different operations and states 
//			found in UTK.
//===================================================================================================================================

#pragma once

namespace UTK::Types::States {

	/**
	 *	@brief Enum for logger types
	 *
	 *	Enumeration to of different types of loggers
	*/
	enum class Logger {
		JSON,
		FILE,
		TERMINAL
	};

	/**
	 *	@brief Enum for operation types
	 *
	 *	Enumeration used to represent the different logging operations.
	*/
	enum class Operations {
		LG_WR,
		LG_RD,
		LG_IN,
		LG_OUT,
		LG_IDL,
		LG_ERR,
		LG_MSG,
		LG_NOP
	};
}