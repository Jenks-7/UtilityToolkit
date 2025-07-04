//===================================================================================================================================
// @file	utkloggers.h
// @author	Jac Jenkins
// @date	25/09/2024
// 
// @brief	Header file containing top level API definitions for logging capabilities 
//			in Utility Toolkit.
//===================================================================================================================================

#pragma once

#include "core/utkexports.hpp"
#include "types/utkstates.hpp"
#include "types/utkmetadata.hpp"
#include <string_view>
#include <string>

namespace UTK::Loggers {

	class loggerHandler {

	private:
		std::string fileName;
		std::string funcName;
		int fileLine = -1;

		using FormatStrings = std::vector<std::string>;

		/**
		 * @brief Internal function used to acquire a filename for the log when it is not defined for the object
		*/
		std::string getFileName() const;

	public:
		UTK_API loggerHandler() = default;

		UTK_API ~loggerHandler() = default;

		/**
		 * @brief Disable copying
		*/
		UTK_API loggerHandler(const loggerHandler& object) = delete;

		/**
		 * @brief Disable assignment operations
		*/
		UTK_API loggerHandler& operator=(const loggerHandler& object) = delete;

		/**
		 * @brief Paramaterized constructor 
		 *	
		 * This constructor accepts the required file information for the prefix. The file param
		 * can either be the exact name of the file you want to specify, or the macro __FILE__ which
		 * will automatically ressolve the to full file path, but this will be shortened to just the
		 * file name because of the constructors internal logic.
		 *
		 * @param file String reference for the file name where log is made.
		 * @param line: Integer for the file line where log is made.
		 * @param func: String reference for function name where log is made.
		*/
		UTK_API loggerHandler(std::string_view file, const int line, std::string_view func);

		/**
		 * @brief Set fileName Attribute
		 *
		 * @param fileName: Reference string to file name to include in log.
		*/
		UTK_API void setFileName(std::string_view fileName);

		/**
		 * @brief Set fileLine Attribute
		 *
		 * @param fileLine: Integer to file line to include in log.
		*/
		UTK_API void setFileLine(const int fileLine);

		/**
		 * @brief Set funcName Attribute
		 *
		 * @param fileName: Reference string to function name to include in log.
		*/
		UTK_API void setFuncName(std::string_view funcName);

		/**
		 * @brief Logs the operation conducted
		 * 
		 * This method handles the generation of logs based on the selected operation
		 * and logger (e.g. terminal or file). It also allows the user to provide
		 * additional metadata to be included in the log message, formatted according to 
		 * an optional format vector.
		 * 
		 * @param op: Operation type used to determine suffix.
		 * @param lg: Enum to control the logger used.
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
		UTK_API void logOperation(
			Types::States::Operations op, 
			Types::States::Logger lg, 
			const FormatStrings& format = {},
			const Types::Metadata::ReflectedValues& metadata = {}
		);
	};
}