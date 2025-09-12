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

	/// TESTING GROUND FOR NEW HANDLER STRUCTURE ///

	/**
	 * @brief Class used to store data within to be dispatched to the UTK logger system
	 */
	class logDispatcher {
	private:
		using loggerEntryQueue = std::queue<UTK::Types::LogEntry::logEntry>;
		
		struct Impl;
		std::mutex _mutex;
		loggerEntryQueue _logQueue;
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