//===================================================================================================================================
// @file	utkloggers.cpp
// @author	Jac Jenkins
// @date	16/09/2024
// 
// @brief   Source file containing the implementation of logging utilities 
//			provided by Utility Toolkit.
//===================================================================================================================================

#include "logger/utkloggers.hpp"
#include <filesystem>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <format>
#include <memory>
#include <chrono>
#include <ctime>

using namespace std;
using namespace chrono;
using namespace UTK::Loggers;
using namespace UTK::Types::States;
using namespace UTK::Types::Metadata;

using StringVector = vector<string>;
using OperationsMap = unordered_map<Operations, string>;

//===================================================================================================================================
//											    HELPER FUNCTIONS & UTILITIES
//===================================================================================================================================
static const string getOpsToSuffix(const Operations& opKey) {

	/// Local map that only gets instantiated once between calls
	static const OperationsMap opsToSuffix{
		{ Operations::LG_RD,  "[READ]" },
		{ Operations::LG_WR,  "[WRITE]" },
		{ Operations::LG_IN,  "[LOGIN]" },
		{ Operations::LG_ERR, "[ERROR]" },
		{ Operations::LG_OUT, "[LOGOUT]" },
		{ Operations::LG_IDL, "[IDLE]" },
		{ Operations::LG_MSG, "[MESSAGE]" },
		{ Operations::LG_NOP, "" }
	};
	
	/// Check if operation is present in map
	auto it = opsToSuffix.find(opKey);
	if (it != opsToSuffix.end()) {
		return it->second;
	}
	
	return "[UNKNOWN]"s;
}

//===================================================================================================================================
//													STANDARD LOGGER INTEFACE 
//===================================================================================================================================

class ILogger {

public:
	virtual void generatePrefix(string_view fileName, string_view fileLine, string_view funcName) = 0;
	virtual void generateSuffix(const Operations& ops, const StringVector& data, const StringVector& format) = 0;
	virtual void printLog() const = 0;
	virtual ~ILogger() = default;

protected:
	virtual string getTimeStamp() = 0;
};

//===================================================================================================================================
//											    INTERFACE AND LOGGER IMPLEMENTATIONS
//===================================================================================================================================

class terminalLogger : public ILogger {

private:
	string prefix;
	string suffix;

protected:
	string getTimeStamp() {

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

public:
	void generatePrefix(string_view fileName, string_view fileLine, string_view funcName) override {

		prefix = format("{} {}:{}:{}", getTimeStamp(), fileName, fileLine, funcName);
	}

	void generateSuffix(const Operations& ops, const StringVector& format, const StringVector& data) override {

		string infoString;
		size_t max_size = max(format.size(), data.size());

		/// Reserve memory to prevent constant reinitializations in lambda
		infoString.reserve(300);

		/// Lambda to format the info string 
		auto append_fn = [&infoString](string_view f, string_view d) {
			if (!f.empty()) {
				infoString.append(f).append(" ");
			}
			if (!d.empty()) {
				infoString.append(d).append(" ");
			}
			};

		/// Combine format and data args together
		for (size_t i = 0; i < max_size; i++) {
			string_view _format = (i < format.size()) ? string_view(format[i]) : string_view{};
			string_view _data = (i < data.size()) ? string_view(data[i]) : string_view{};

			append_fn(_format, _data);
		}

		/// Cleanup final character and optimize memory from prior reservation
		infoString.pop_back();					
		string(infoString).swap(infoString);	

		/// Generate the suffix
		auto& operation = getOpsToSuffix(ops);
		suffix.append(operation).append(" ").append(infoString);
	}

	void printLog() const override {

		if (prefix.empty()) {
			cout << suffix << "\n";
		}
		else {
			constexpr int fixedPrefixWidth = 70;
			cout << format("{:<{}} {}", prefix, fixedPrefixWidth, suffix) << "\n";
		}
	}
};

//===================================================================================================================================
//													 LOGGER FACTORY DEFINITION
//===================================================================================================================================

class lgFactory {

private:
	lgFactory() = delete;

public:
	static unique_ptr<ILogger> getLogger(Logger lg) {
		switch (lg)
		{
		case Logger::JSON:
			//break;
		case Logger::FILE:
			//break;
		case Logger::TERMINAL:
		default:
			return make_unique<terminalLogger>();
			break;
		}
	}
};

//===================================================================================================================================
//												LOGGER HANDLER METHOD IMPLEMENTATIONS
//===================================================================================================================================

loggerHandler::loggerHandler(string_view file, const int& line, string_view func)
	: fileName(filesystem::path(file).filename().string()), fileLine(line), funcName(func)
{
	;
}

string loggerHandler::getFileName() const {

	return filesystem::path(__FILE__).filename().string();
}

void loggerHandler::setFileName(string_view fileName) {

	this->fileName = filesystem::path(fileName).filename().string();
}

void loggerHandler::setFileLine(const int& fileLine) {

	this->fileLine = fileLine;
}

void loggerHandler::setFuncName(string_view funcName) {

	this->funcName = funcName;
}

void loggerHandler::logOperation(Operations op, Logger lg, const FormatStrings& format, const ReflectedValues& metadata) {

	unique_ptr<ILogger> logger = lgFactory::getLogger(lg);

	/// If specified parameters are empty, set them as the following.
	if (fileName.empty())    fileName = "<unknown file>";
	if (funcName.empty())    funcName = "<unknown function>";
	
	/// Needed to allow for clearer output when invalid params are given
    string lineStr = fileLine < 0 ? "<unknown_line>" : to_string(fileLine);

	/// Generate log
	logger->generatePrefix(fileName, lineStr, funcName);
	logger->generateSuffix(op, format, metadata);
	logger->printLog();
}