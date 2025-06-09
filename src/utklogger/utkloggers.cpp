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
#include <iostream>
#include <iomanip>
#include <sstream>
#include <format>
#include <memory>
#include <chrono>
#include <ctime>
#include <map>

using namespace std;
using namespace chrono;
using namespace UTK::Loggers;
using namespace UTK::Types::States;
using namespace UTK::Types::Metadata;

using StringVector = std::vector<std::string>;

//===================================================================================================================================
//											    INTERFACE AND LOGGER IMPLEMENTATIONS
//===================================================================================================================================

class ILogger {

public:
	virtual void generatePrefix(string_view fileName, const int& fileLine, string_view funcName) = 0;
	virtual void generateSuffix(Operations ops, const vector<string>& data, const vector<string>& format) = 0;
	virtual void printLog() const = 0;
	virtual ~ILogger() = default;

protected:
	virtual string getTimeStamp() = 0;
};

class terminalLogger : public ILogger {

private:
	string prefix;
	string suffix;
	const int spacing = 40;

	map<Operations, string> opsToSuffix{
		{Operations::LG_RD,  "[READ] "},
		{Operations::LG_WR,  "[WRITE] "},
		{Operations::LG_IN,  "[LOGIN] "},
		{Operations::LG_ERR, "[ERROR] "},
		{Operations::LG_OUT, "[LOGOUT] "},
		{Operations::LG_IDL, "[IDLE] "},
		{Operations::LG_NOP, ""}
	};

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
	void generatePrefix(string_view fileName, const int& fileLine, string_view funcName) override {

		prefix = format("{} {}:{}:{}", getTimeStamp(), fileName, fileLine, funcName);
	}

	void generateSuffix(Operations ops, const StringVector& format, const StringVector& data) override {

		string infoString;
		size_t max_size = max(format.size(), data.size());

		infoString.reserve(300);

		auto append_fn = [&infoString](string_view f, string_view d) {
			if (!f.empty()) {
				infoString.append(f).append(" ");
			}
			if (!d.empty()) {
				infoString.append(d).append(" ");
			}
			};

		for (size_t i = 0; i < max_size; i++) {
			string_view _format = (i < format.size()) ? string_view(format[i]) : string_view{};
			string_view _data = (i < data.size()) ? string_view(data[i]) : string_view{};

			append_fn(_format, _data);
		}

		infoString.pop_back();				  // Trims the final space character
		string(infoString).swap(infoString);  // Optimize memory from prior reservation

		/// MAIN LOGIC HERE
		suffix = opsToSuffix.at(ops);
		suffix.append(infoString);
	}

	void printLog() const {
		
		prefix.append("Hello");

		cout << setw(spacing)
			<< left
			<< prefix
			<< suffix
			<< endl;
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

	/// It's assumed you want the class method name, source file & line if initialized without members.
	if (fileName.empty() || funcName.empty()) {

		fileName = getFileName();
		funcName = __func__;
	}

	/// Generate log
	logger->generatePrefix(fileName, fileLine, funcName);
	logger->generateSuffix(op, format, metadata);
	logger->printLog();
}