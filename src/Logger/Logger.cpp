#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include "Logger.h"


std::vector<LogEntry> Logger::messages;

std::string GetCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    char buf[sizeof "YYYY-MM-DD HH:MM:SS"];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::string timestamp(buf);
    return timestamp;
}

std::string GetCurrentDateTimeMessage(const std::string& message) {
    return "[" + GetCurrentDateTime() + "] " + message;
}


void Logger::Success(const std::string& message) {
    std::string finalMessage = GetCurrentDateTimeMessage(message);
    InsertLogMessage(LogType::LOG_SUCCESS, finalMessage);
    std::cout << "\033[1;32m" << finalMessage << "\033[0m" << std::endl;
}

void Logger::Log(const std::string& message) {
    std::string finalMessage = GetCurrentDateTimeMessage(message);
    InsertLogMessage(LogType::LOG_INFO, finalMessage);
    std::cout << "\033[1;33m" << finalMessage << "\033[0m" << std::endl;
}

void Logger::Err(const std::string& message) {
    std::string finalMessage = GetCurrentDateTimeMessage(message);
    InsertLogMessage(LogType::LOG_ERROR, finalMessage);
    std::cout << "\033[1;31m" << finalMessage << "\033[0m" << std::endl;
}




void Logger::InsertLogMessage(LogType type, std::string message) {
    LogEntry logEntry;
    logEntry.type = type;
    logEntry.message = message;
    messages.push_back(logEntry);
}
