#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

class Logger
{
public:
	static void Log(std::string message);
};

#define DEBUG_LOG(x) Logger::Log(x);