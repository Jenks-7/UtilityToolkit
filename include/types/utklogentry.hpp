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
#include <optional>
#include "types/utkstates.hpp"

using FormatStrings = std::vector<std::string>;

namespace UTK::Types::LogEntry {

	struct logEntry {
		States::Logger lg;
		States::Operations op;
		FormatStrings formatArgs;
		FormatStrings formatValues;
		std::optional<std::string> fileName = std::nullopt;
		std::optional<int> fileLine = std::nullopt;
		std::optional<std::string> funcName = std::nullopt;
	};

	inline namespace LogEntryHelpers {

		inline logEntry makeLogEntry(
			States::Logger lg,
			States::Operations op,
			FormatStrings args,
			FormatStrings values,
			std::string fileName = "",
			int fileLine = -1,
			std::string funcName = "")
		{
			return { lg, op, std::move(args), std::move(values), fileName, fileLine, funcName };
		}

		inline logEntry makeTerminalEntry(
			States::Operations op = States::Operations::LG_NOP,
			FormatStrings args = {},
			FormatStrings values = {},
			std::string fileName = "",
			int fileLine = -1,
			std::string funcName = "")
		{
			return { States::Logger::TERMINAL, op, std::move(args), std::move(values), fileName, fileLine, funcName };
		}
	}
}