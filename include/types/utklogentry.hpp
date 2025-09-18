//===================================================================================================================================
// @file	utklogentry.hpp
// @author	Jac Jenkins
// @date	09/09/2025
// 
// @brief   This file contains the definition for the logEntry type used by
//			logDispatcher found in utkloggers.
//===================================================================================================================================

#pragma once

#include "types/utkstates.hpp"
#include <optional>
#include <vector>
#include <string>

using FormatStrings = std::vector<std::string>;

namespace UTK::Types::LogEntry {

	/**
	 * @brief Data container holding message data for UTK loggers.
	 */
	struct logEntry {
		States::Logger lg;
		States::Operations op;
		FormatStrings formatArgs;
		FormatStrings formatValues;
		std::optional<std::string> fileName = std::nullopt;
		std::optional<int> fileLine = std::nullopt;
		std::optional<std::string> funcName = std::nullopt;
	};

	inline namespace LogHelpers {

		/**
		 * @brief Helper function to create and configure logEntry objects
		 * 
		 * @param lg		Logger type to use for output.
		 * @param op		Operation performed.
		 * @param args		Format string arguments.
		 * @param values	Format string values.
		 * @param fileName	Name of file where log was created(may be unused by some loggers).
		 * @param fileLine	Line of file where log was created(may be unused by some loggers).
		 * @param funcName	Function where the log was created(may be unused by some loggers).
		 * 
		 * @return Configured logEntry object.
		 */
		inline logEntry makeLogEntry(
			Types::States::Logger lg,
			Types::States::Operations op = States::Operations::LG_NOP,
			FormatStrings args = {},
			FormatStrings values = {},
			std::string fileName = "",
			int fileLine = -1,
			std::string funcName = "")
		{
			return { lg, op, std::move(args), std::move(values), fileName, fileLine, funcName };
		}

		/**
		 * @brief Helper function to create logEntry objects for terminal logging.
		 *
		 * @param op		Operation performed(default: LG_NOP, no operation specified).
		 * @param args		Format string arguments.
		 * @param values	Format string values.
		 * @param fileName	Name of file where log was created(may be unused by some loggers).
		 * @param fileLine	Line of file where log was created(may be unused by some loggers).
		 * @param funcName	Function where the log was created(may be unused by some loggers).
		 *
		 * @return Configured logEntry object.
		 */
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

		/**
		 * @brief Helper function to create logEntry objects for csv logging
		 * 
		 * @param op		Operation performed(default: LG_NOP, no operation specified).
		 * @param args		Format string arguments.
		 * @param values	Format string values.
		 * 
		 * @return Configured logEntry object.
		 */
		inline logEntry makeCsvEntry(
			States::Operations op,
			FormatStrings args,
			FormatStrings values,
			std::string fileName)
		{
			return { States::Logger::CSV, op, std::move(args), std::move(values), fileName };
		}
	}
}