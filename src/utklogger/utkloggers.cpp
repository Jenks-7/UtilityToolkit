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
//											        HELPER FUNCTIONS & UTILITIES
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
//													  STANDARD LOGGER INTEFACE 
//===================================================================================================================================

class ILogger {

public:
	virtual void generatePrefix(string_view, string_view, string_view) {};					// Parameter names can be left out
	virtual void generateSuffix(Operations, const StringVector&, const StringVector&) {};	// Parameter names can be left out
	virtual void createLog() const = 0;
	virtual ~ILogger() = default;

protected:
	virtual string getTimeStamp() const {
		return "";
	}
};

//===================================================================================================================================
//											    INTERFACE AND LOGGER IMPLEMENTATIONS
//===================================================================================================================================

class terminalLogger : public ILogger {

private:
	string prefix;
	string suffix;

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

		/// Combine format and data args together
		size_t max_size = max(fmt.size(), data.size());

		for (size_t i = 0; i < max_size; i++) {
			string_view _format = (i < fmt.size()) ? string_view(fmt[i]) : string_view{};
			string_view _data = (i < data.size()) ? string_view(data[i]) : string_view{};

			append_fn(_format, _data);
		}

		/// Cleanup final character and optimize memory from prior reservation
		if (!infoString.empty()) infoString.pop_back();
		string(infoString).swap(infoString);

		return infoString;
	}

public:
	void generatePrefix(string_view fileName, string_view fileLine, string_view funcName) override {

		prefix = format("{} {}:{}:{}", getTimeStamp(), fileName, fileLine, funcName);
	}

	void generateSuffix(Operations ops, const StringVector& fmt, const StringVector& data) override {

		suffix = format("{} {}", getOpsToSuffix(ops), joinFormatData(fmt, data));
	}

	void createLog() const override {

		/// Print only the suffix if no prefix, or print the aligned prefix and suffix
		if (prefix.empty()) {
			cout << suffix << "\n";
		}
		else {
			constexpr int fixedPrefixWidth = 70;
			cout << format("{:<{}} {}", prefix, fixedPrefixWidth, suffix) << "\n";
		}
	}
};

/** This needs to be thoguht through properly as it doesn't fit current design hierarchy **/

//class csvLogger : public  ILogger {
//private:
//
//	struct fileData;
//	string fileName;
//	ofstream file;
//
//public:
//	csvLogger(string fileName) : fileName(fileName) {};
//	csvLogger(ofstream&& file) : file(move(file)) {};
//};

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
		case Logger::CSV:
			//return make_unique<csvLogger>();
			//break;
		case Logger::TERMINAL:
		default:
			return make_unique<terminalLogger>();
			break;
		}
	}
};

//===================================================================================================================================
//												TERMINAL LOGGER METHOD IMPLEMENTATIONS
//===================================================================================================================================

loggerHandler<Logger::TERMINAL>::loggerHandler(string_view file, const int line, string_view func)
	: fileName(filesystem::path(file).filename().string()), fileLine(line), funcName(func)
{
	;
}

void loggerHandler<Logger::TERMINAL>::setFileName(string_view fileName) {

	this->fileName = filesystem::path(fileName).filename().string();
}

void loggerHandler<Logger::TERMINAL>::setFileLine(const int fileLine) {

	this->fileLine = fileLine;
}

void loggerHandler<Logger::TERMINAL>::setFuncName(string_view funcName) {

	this->funcName = funcName;
}

void loggerHandler<Logger::TERMINAL>::logOperation(Operations op, const FormatStrings& format, const ReflectedValues& metadata) {

	unique_ptr<ILogger> logger = lgFactory::getLogger(Logger::TERMINAL);

	/// If specified parameters are empty, set them as the following.
	if (fileName.empty())    fileName = "<unknown file>";
	if (funcName.empty())    funcName = "<unknown function>";
	
	/// Needed to allow for clearer output when invalid params are given
    string lineStr = fileLine < 0 ? "<unknown_line>" : to_string(fileLine);

	/// Generate log
	logger->generatePrefix(fileName, lineStr, funcName);
	logger->generateSuffix(op, format, metadata);
	logger->createLog();
}

//===================================================================================================================================
//												   CSV LOGGER METHOD IMPLEMENTATIONS
//===================================================================================================================================

