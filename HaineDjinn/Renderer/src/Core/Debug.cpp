
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <cassert>
#include <chrono>

#include "Core/Debug.h"
#include "Core/Constants.h"

// define this to use std::localtime()
#pragma warning(disable : 4996)


namespace Debug
{
	namespace {
		bool isDebugging;

		std::ofstream file;
		std::stringstream logBuffer;
	}

	void InitDebugging()
	{
		isDebugging = true;

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);

#if HD_IS_LOGGING
		std::tm* timeinfo;
		char buffer[80];

		std::time(&time);
		timeinfo = std::localtime(&time);

		std::strftime(buffer, 80, "%Y_%m_%d_%H_%M_%S", timeinfo);
		std::puts(buffer);

		std::string logName(buffer);
		logName = "../Logs/" + logName + ".txt";

		file.open(logName);

		HD_ASSERT(file.is_open(), "Can't create log file");
#endif
	}

	void EndDebugging()
	{
		isDebugging = false;
		HD_LOG("Ending debug", LogSeverity::LOG_MESSAGE);

#if HD_IS_LOGGING
		if (!file.is_open())
			return;

		file << logBuffer.rdbuf();
		file.close();
#endif
	}

	void WriteTempLog()
	{
#if HD_IS_LOGGING
		if (!isDebugging || !file.is_open())
			InitDebugging();

		file << logBuffer.rdbuf();
		logBuffer.str(std::string());
#endif
	}

	void Log(const std::string& logContent, const LogSeverity severity, const int line, const std::string time, const std::string filePath)
	{
		std::string sevString;

		switch (severity)
		{
		case (LogSeverity::LOG_MESSAGE):
			sevString = "[Message] ";
			break;
		case (LogSeverity::LOG_WARNING):
			sevString = "[Warning] ";
			break;
		case (LogSeverity::LOG_ERROR):
			sevString = "[Error] ";
			break;
		case (LogSeverity::LOG_ASSERT):
			sevString = "[Assert] ";
			break;
		default:
			break;
		}

#if HD_IS_LOGGING
		logBuffer.seekg(0, std::ios::end);
		std::streampos buffSize = logBuffer.tellg();
		logBuffer.seekg(0, std::ios::beg);

		if (buffSize >= MAX_LOG_STRING_SIZE)
			WriteTempLog();

		logBuffer << sevString + time + " | " + std::filesystem::path(filePath).filename().string() + "(" + std::to_string(line) + ") : " + logContent << std::endl;
#endif
		std::cout << sevString << std::filesystem::path(filePath).filename().string() + "(" + std::to_string(line) + ") : " + logContent << std::endl;
	}

	void Assert(bool condition, const std::string& message, const int line, const std::string time, const std::string filePath)
	{
#if HD_IS_LOGGING
		if (logBuffer.gcount() >= MAX_LOG_STRING_SIZE)
			WriteTempLog();

		logBuffer << "[Assert] " + time + " | HD_ASSERT called from " + std::filesystem::path(filePath).filename().string() + "(" + std::to_string(line) + ")" << std::endl;
#endif

		if (condition)
			return;
		
		HD_LOG(message + " | Assert base info: " + std::filesystem::path(filePath).filename().string() + "(" + std::to_string(line) + ")", LogSeverity::LOG_ASSERT);
		EndDebugging();
		assert(condition);
	}
}
