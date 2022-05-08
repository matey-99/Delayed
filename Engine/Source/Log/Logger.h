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
	static void Log(std::string message, std::string location = LINE_OF_CODE);
	static void Log(Actor* owner, std::string message, std::string location = LINE_OF_CODE);
};


#define DEBUG_LOG(x) Logger::Log(x, LINE_OF_CODE);
#define ACTOR_DEBUG_LOG(owner, x) Logger::Log(owner, x, LINE_OF_CODE);