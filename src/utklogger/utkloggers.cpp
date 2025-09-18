//===================================================================================================================================
// @file	utkloggers.cpp
// @author	Jac Jenkins
// @date	16/09/2024
// 
// @brief   Source file containing the implementation of logging utilities 
//			provided by Utility Toolkit.
//===================================================================================================================================

#include "logger/utkloggers.hpp"
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <numeric>
#include <format>
#include <memory>
#include <chrono>
#include <ctime>

using namespace std;
using namespace chrono;
using namespace UTK::Loggers;
using namespace UTK::Types::States;
using namespace UTK::Types::Metadata;
using namespace UTK::Types::LogEntry;

using StringVector = vector<string>;
using OperationsMap = unordered_map<Operations, string_view>;

//===================================================================================================================================
//											        HELPER FUNCTIONS & UTILITIES
//===================================================================================================================================

static const string_view getOpsToSuffix(const Operations& opKey) {

	/// Local map that only gets instantiated once between calls
	static const OperationsMap opsToSuffix {
		{ Operations::LG_RD,  "[READ]"sv },
		{ Operations::LG_WR,  "[WRITE]"sv },
		{ Operations::LG_IN,  "[LOGIN]"sv },
		{ Operations::LG_ERR, "[ERROR]"sv },
		{ Operations::LG_OUT, "[LOGOUT]"sv },
		{ Operations::LG_IDL, "[IDLE]"sv },
		{ Operations::LG_MSG, "[MESSAGE]"sv },
		{ Operations::LG_NOP, ""sv }
	};
	
	/// Check if operation is present in map
	auto it = opsToSuffix.find(opKey);
	if (it != opsToSuffix.end()) {
		return it->second;
	}
	
	return "[UNKNOWN]"sv;
}

//===================================================================================================================================
//													  STANDARD LOGGER INTEFACE 
//===================================================================================================================================

class IKeyValueLogger {

public:
	virtual void createLog(logEntry& entry) = 0;
	virtual ~IKeyValueLogger() = default;

protected:
	virtual string getTimeStamp() const {
		return "";
	}
};

//===================================================================================================================================
//											    INTERFACE AND LOGGER IMPLEMENTATIONS
//===================================================================================================================================

class terminalLogger : public IKeyValueLogger {

private:
	string _prefix;
	string _suffix;
	const int _fixedPrefixWidth = 60;

	string getTimeStamp() const override {

		/// Acquire current time point
		time_t now_t{ system_clock::to_time_t(system_clock::now()) };

		/// Convert to local time(Windows version, because it forces you to use _s equivalent...)
		tm tt;
		localtime_s(&tt, &now_t);

		/// Convert time to string
		stringstream ss;
		ss << put_time(&tt, "%d/%m/%Y %H:%M:%S");

		return ss.str();
	};
	string joinFormatData(const StringVector& fmt, const StringVector& data) {
		
		/// Reserve memory to prevent constant reinitializations in lambda
		string infoString;
		infoString.reserve(300);

		/// Lambda to format the info string 
		auto append_fn = [&infoString](string_view f, string_view d) {
			if (!f.empty()) infoString.append(f).append(" ");
			if (!d.empty()) infoString.append(d).append(" ");
			};

		/// Combine format and data args together & account for differing lengths
		size_t max_size = max(fmt.size(), data.size());

		for (size_t i = 0; i < max_size; i++) {
			string_view _format = (i < fmt.size()) ? string_view(fmt[i]) : string_view{};
			string_view _data = (i < data.size()) ? string_view(data[i]) : string_view{};

			append_fn(_format, _data);
		}

		/// Cleanup final character and optimize memory from prior reservation
		if (!infoString.empty()) infoString.pop_back();
		//string(infoString).swap(infoString);		// UNECESSARY: Modern compilers will use NVRO/move schemantics

		return infoString;
	}
	void generatePrefix(string_view fileName, int fileLine, string_view funcName) {

		_prefix = format("{} {}:{}:{}", getTimeStamp(), fileName, fileLine, funcName);
	}
	void generateSuffix(Operations op, const StringVector& fmt, const StringVector& data) {

		_suffix = format("{} {}", getOpsToSuffix(op), joinFormatData(fmt, data));
	}

public:

	void createLog(logEntry& entry) noexcept override {
		try {
			auto file = entry.fileName.value_or("<unknown_file>");
			auto func = entry.funcName.value_or("<unknown_func>");
			auto line = entry.fileLine.value_or(-1);

			// Shorten file path to just be file name
			file = filesystem::path(file).filename().string();

			// These methods create the string for the prefix and suffix members
			generatePrefix(file, line, func);
			generateSuffix(entry.op, entry.formatArgs, entry.formatValues);

			/// Print only the suffix if no prefix, or print the aligned prefix and suffix
			if (_prefix.empty()) {
				cout << _suffix << "\n";
			}
			else {
				cout << format("{:<{}} {}", _prefix, _fixedPrefixWidth, _suffix) << "\n";
			}
		}
		catch (const std::exception& e) {
			cerr << "[Terminal Logger Error] " << e.what() << "\n";
		}
		catch (...) {
			cerr << "[Terminal Logger Error] Unknown exception\n";
		}
	}

	terminalLogger() = default;
	terminalLogger(terminalLogger& lg) = delete;
	terminalLogger(terminalLogger&& lg) = delete;
	terminalLogger& operator=(const terminalLogger&) = delete;
};

class csvLogger : public  IKeyValueLogger {

private:
	ofstream _file;

	void ensureOpen(const string& fileName = {}) {
		if (!_file.is_open()) {
			throw runtime_error(fileName.empty()
				? "Invalid file stream"
				: "Failed to open csv file: " + fileName
			);
		}
	}
	string escapeCsvField(string_view field) {

		// Check for presence of CSV special characters(comma, quote, or newline). Is so, quoting is required.
		bool needsQuotes = field.find_first_of(",\"\n") != string::npos;

		string result;

		if (needsQuotes) {
			result.reserve(field.size() + 2);	// Add memory for the start and end quotes
			result.push_back('"');				// Add in starting quote to the result

			// If char is a quote, escape by doubling them.
			for (char c : field) {
				if (c == '"') result += "\"\"";
				else result.push_back(c);
			}
			result.push_back('"');				// Add in closing quote
		}
		else {
			// No escaping needed, copy field directly
			result.assign(field);
		}

		return result;
	}
	string makeCsvRow(const vector<string>& fields) {

		// UPDATE FORMATTING LOGIC AND MERGE CSV AND TERMINAL METHODS
		// TO A BETTER WAY TO ACCOUNT FOR LSIST VALUES ETC IN CHATGPT CONVO.

		// Creatively utilizes accumulate() to format and join CSV fields.
		string str = accumulate(
			next(fields.begin()), fields.end(), escapeCsvField(fields[0]),
			[this](const string& a, const string& b) {
				return a + ',' + escapeCsvField(b);
			}
		);

		return str;
	}

public:
	explicit csvLogger(const string& fileName, ios::openmode mode) : _file(fileName, mode) {
		ensureOpen(fileName);
	};
	csvLogger(ofstream&& file) : _file(move(file)) {
		ensureOpen();
	};

	void createLog(logEntry& entry) noexcept override {
		
		// Decide on a full row of all keys/values or tabulating the Key 
		// Values at the end of each log. All that will change is the way
		// each row is created here and the makeCsvRow method will alter
		// to just work for each key value

		return;
	}

	csvLogger() = default;
	csvLogger(ofstream& file) = delete;
	csvLogger& operator=(const csvLogger&) = delete;
};

//===================================================================================================================================
//													 LOGGER FACTORY DEFINITION
//===================================================================================================================================

class lgFactory {

private:
	lgFactory() = delete;

public:
	static unique_ptr<IKeyValueLogger> getLogger(Logger lg) {

		switch (lg) {
			case Logger::JSON:
				//break;
			case Logger::CSV:
				return make_unique<csvLogger>();
			case Logger::TERMINAL:
			default:
				return make_unique<terminalLogger>();
				break;
		}
	}
};

//===================================================================================================================================
//													 LOG CONTROLLER DEFINITION
//===================================================================================================================================

class logController {
private:
	using LoggerCache = unordered_map<Logger, unique_ptr<IKeyValueLogger>>;

	LoggerCache cache;
	IKeyValueLogger& getLogger(Logger lgType) {
		
		auto lg = cache.find(lgType);
		if (lg != cache.end()) {
			return *(lg->second);
		}

		// Lazy init to create a logger instance and store in the cache for future lookup
		auto logger = lgFactory::getLogger(lgType);
		IKeyValueLogger& ref = *logger;
		cache.emplace(lgType, move(logger));

		return ref;
	}

public:
	void logEntry(logEntry&& entry) {
		IKeyValueLogger& lg = getLogger(entry.lg);
		lg.createLog(entry);
	}
};

//===================================================================================================================================
//												  DISPATCHER METHOD IMPLEMENTATIONS
//===================================================================================================================================

void logDispatcher::pushEntry(logEntry entry) {

	lock_guard<mutex> lock(_mutex);
	_logQueue.push(entry);
}

void logDispatcher::dispatchLogs() {

	loggerEntryQueue localQueue;
	unique_ptr<logController> dispatchController = make_unique<logController>();

	// Lock the thread for the copy to improve performance in single and multithreaded use
	{
		lock_guard<mutex> lock(_mutex);
		swap(localQueue, _logQueue);	// Effectively copies the queue to the local queue and clears the _logQueue member
	}

	while (!localQueue.empty()) {

		dispatchController->logEntry(move(localQueue.front()));
		localQueue.pop();
	}
}