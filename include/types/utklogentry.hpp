//===================================================================================================================================
// @file	utklogentry.hpp
// @author	Jac Jenkins
// @date	09/09/2025
// 
// @brief   This file contains the definition for the logEntry type used by
//			logDispatcher found in utkloggers.
//===================================================================================================================================

#pragma once

#include <vector>
#include <string>
#include "types/utkstates.hpp"

using FormatStrings = std::vector<std::string>;

namespace UTK::Types::LogEntry {

	struct logEntry {
		UTK::Types::States::Logger lg;
		UTK::Types::States::Operations op;
		FormatStrings formatArgs;
		FormatStrings formatValues;
	};
}