#pragma once

#include <vector>
#include <functional>

#include "Core.h"

class Event
{
public:
	Event() {};

	template <class T>
	void Add(void (T::*func)(), T* object)
	{
		m_Functions.push_back(std::bind(func, object));
	}

	template <class T>
	void Remove(void (T::* func)(), T* object)
	{
		auto bind = std::bind(func, object);
		m_Functions.erase(std::remove(m_Functions.begin(), m_Functions.end(), bind), m_Functions.end());
	}

	void Clear()
	{
		m_Functions.clear();
	}

	void Broadcast()
	{
		for (auto func : m_Functions)
			func();
	}

private:
	std::vector<std::function<void()>> m_Functions;
};