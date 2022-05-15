#include "Logger.h"

#include "Scene/Actor.h"

void Logger::Warn(std::string message, std::string location)
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);

	std::string log = std::ctime(&time);
	log += "LOG MESSAGE: " + message + " | ";
	log += "LINE OF CODE: " + location;

	std::cout << log << std::endl;
}
