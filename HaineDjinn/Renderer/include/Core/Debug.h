#pragma once

#include <string>

#define HD_LOG(logContent, severity) Debug::Log(logContent, severity, __LINE__, __TIME__, __FILE__)
#define HD_ASSERT(condition, message) Debug::Assert(condition, message, __LINE__, __TIME__, __FILE__)

#define HD_IS_LOGGING 0 // set to 0 to disable logging in file

enum class LogSeverity
{
	LOG_MESSAGE,
	LOG_WARNING,
	LOG_ERROR,
	LOG_ASSERT,

};

namespace Debug
{
	void InitDebugging();
	void EndDebugging();

	void Log(const std::string& str, const LogSeverity severity, const int line = 0, const std::string time = "", const std::string filePath = "");
	void Assert(bool condition, const std::string& message, const int line = 0, const std::string time = "", const std::string filePath = "");
}