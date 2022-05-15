#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#define LINE_OF_CODE "File: " + std::string(__FILE__) + ", Line: " + std::to_string(__LINE__)

class Actor;

class Logger
{
public:
	static void Warn(std::string message, std::string location = LINE_OF_CODE);
};


#define DEBUG_LOG(x, ...) printf(x, ...);
#define WARN(x) Logger::Warn(x, LINE_OF_CODE);