//===================================================================================================================================
// @file	utkloggers.h
// @author	Jac Jenkins
// @date	25/09/2024
// 
// @brief	Header file containing top level API definitions for the key:value
//			loggers found in UTK's logging capabilities
//===================================================================================================================================

#pragma once

#include "core/utkexports.hpp"
#include "types/utkstates.hpp"
#include "types/utkmetadata.hpp"
#include "types/utklogentry.hpp"
#include <string_view>
#include <string>
#include <queue>
#include <mutex>

namespace UTK::Loggers {

	/**
	 * @brief Template class for handling logging operations for different logger types.
	 *
	 * This template class specializes logging behavior depending on the Logger type parameter
	 * Each specialization implements the logging logic for a specific logger (e.g., terminal, CSV).
	 *
	 * @tparam L Logger type enum value that selects the specialization.
	*/
	template<Types::States::Logger L>
	class loggerHandler {
	public:
		loggerHandler() = default;
		using FormatStrings = std::vector<std::string>;

		void logOperation(
			Types::States::Operations op,
			const FormatStrings& format = {},
			const Types::Metadata::ReflectedValues& metadata = {}
		) {
			static_assert(L != L, "loggerHandler specialization for this Logger type is not implemented.");
		}
	};

	/**
	 * @brief Logs an operation with optional formatting and metadata.
	 *
	 * This method performs the logging action specific to the logger type specialization
	 * It formats the log message based on the operation, format strings, and metadata provided.
	 *
	 * @param op The operation type (e.g., READ, WRITE) to be logged.
	 * @param format Optional vector of strings for formatting the log metadata.
	 * @param metadata Optional reflected values providing additional log data.
	*/
	template<>
	class loggerHandler<Types::States::Logger::TERMINAL> {
	private:
		std::string fileName;
		std::string funcName;
		int fileLine = -1;

		using FormatStrings = std::vector<std::string>;

	public:
		loggerHandler() = default;
		~loggerHandler() = default;
		loggerHandler(const loggerHandler& object) = delete;
		loggerHandler& operator=(const loggerHandler& object) = delete;

		/**
		 * @brief Paramaterized constructor
		 *
		 * This constructor accepts the required file information for the prefix. The file param
		 * can either be the exact name of the file you want to specify, or the macro __FILE__ which
		 * will automatically resolve the to full file path, but this will be shortened to just the
		 * file name because of the constructors internal logic.
		 *
		 * @param file String reference for the file name where log is made.
		 * @param line: Integer for the file line where log is made.
		 * @param func: String reference for function name where log is made.
		*/
		loggerHandler(std::string_view file, const int line, std::string_view func);

		/**
		 * @brief Set fileName Attribute
		 *
		 * @param fileName: Reference string to file name to include in log.
		*/
		void setFileName(std::string_view fileName);

		/**
		 * @brief Set fileLine Attribute
		 *
		 * @param fileLine: Integer to file line to include in log.
		*/
		void setFileLine(const int fileLine);

		/**
		 * @brief Set funcName Attribute
		 *
		 * @param fileName: Reference string to function name to include in log.
		*/
		void setFuncName(std::string_view funcName);

		/**
		 * @brief Logs the operation conducted
		 *
		 * This method handles the generation of logs based on the selected operation
		 * and logger (e.g. terminal or file). It also allows the user to provide
		 * additional metadata to be included in the log message, formatted according to
		 * an optional format vector.
		 *
		 * @param op: Operation type used to determine suffix.
		 * @param metadata: Optional metadata to include.
		 * @param format: Optional format strings to structure metadata values.
		 *
		 * @note Pass format vector in anyone of the following ways:
		 *
		 *  Using brace initialization with string literals:
		 *	 @code
		 *   foo({ "Header", "Body", "Footer" });
		 *   @endcode
		 *
		 *  Using an explicitly constructed std::vector<std::string>:
		 *   @code
		 *   std::vector<std::string> labels = { "Step 1", "Step 2" };
		 *   processFormatStrings(labels);
		 *   @endcode
		 *
		 *  Using std::string literals to ensure std::string type for an initilizer_list:
		 *   @code
		 *   using namespace std::string_literals;
		 *   auto valid = { "One"s, "Two"s };
		 *   processFormatStrings(std::vector<std::string>(valid));
		 *   @endcode
		*/
		void logOperation(
			Types::States::Operations op,
			const FormatStrings& format = {},
			const Types::Metadata::ReflectedValues& metadata = {}
		);
	};

	/// TESTING GROUND FOR NEW HANDLER STRUCTURE ///

	/**
	 * @brief Class used to store data within to be dispatched to the UTK logger system
	 */
	class logDispatcher {
	private:
		using loggerEntryQueue = std::queue<UTK::Types::LogEntry::logEntry>;

		loggerEntryQueue logQueue;
		std::mutex _mutex;
	public:
		/**
		 * @brief Adds entries to the dispatcher internal queue
		 * 
		 * @param entry: A LogEntry struct to be actioned by the logging system
		 */
		void pushEntry(UTK::Types::LogEntry::logEntry entry);

		/**
		 * @brief Evaluates each item in the queue and dispatches each to the logging system
		 */
		void dispatchLogs();
	};

	/// TESTING GROUND FOR NEW HANDLER STRUCTURE ///
}