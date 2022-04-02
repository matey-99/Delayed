#include "Logger.h"

void Logger::Log(std::string message)
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);

	std::cout << std::ctime(&time) << "LOG MESSAGE: " << message << std::endl;
}
