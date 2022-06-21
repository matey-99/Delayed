#pragma once

#include <vector>
#include <functional>

#include "Core.h"

template <typename ParamType>
class DelegateOneParam
{
public:
	DelegateOneParam() {};

	template <class T>
	void Add(void (T::*func)(ParamType), T* object)
	{
		m_Functions.push_back(std::bind(func, object, std::placeholders::_1));
	}

	// TO DO: objects of type std::bind can't compare between each other with using '==' - need to find another way of removing binded functions
	template <class T>
	void Remove(void (T::* func)(ParamType), T* object)
	{
		auto bind = std::bind(func, object, std::placeholders::_1);
		m_Functions.erase(std::remove(m_Functions.begin(), m_Functions.end(), bind), m_Functions.end());
	}

	void Clear()
	{
		m_Functions.clear();
	}

	void Broadcast(ParamType param)
	{
		for (auto func : m_Functions)
			func(param);
	}

private:
	std::vector<std::function<void(ParamType)>> m_Functions;
};

template <typename ParamType1, typename ParamType2>
class DelegateTwoParams
{
public:
	DelegateTwoParams() {};

	template <class T>
	void Add(void (T::* func)(ParamType1, ParamType2), T* object)
	{
		m_Functions.push_back(std::bind(func, object, std::placeholders::_1, std::placeholders::_2));
	}

	// TO DO: objects of type std::bind can't compare between each other with using '==' - need to find another way of removing binded functions
	template <class T>
	void Remove(void (T::* func)(ParamType1, ParamType2), T* object)
	{
		auto bind = std::bind(func, object, std::placeholders::_1, std::placeholders::_2);
		m_Functions.erase(std::remove(m_Functions.begin(), m_Functions.end(), bind), m_Functions.end());
	}

	void Clear()
	{
		m_Functions.clear();
	}

	void Broadcast(ParamType1 param1, ParamType2 param2)
	{
		for (auto func : m_Functions)
			func(param1, param2);
	}

private:
	std::vector<std::function<void(ParamType1, ParamType2)>> m_Functions;
};

template <typename ParamType1, typename ParamType2, typename ParamType3, typename ParamType4>
class DelegateFourParams
{
public:
	DelegateFourParams() {};

	template <class T>
	void Add(void (T::* func)(ParamType1, ParamType2, ParamType3, ParamType4), T* object)
	{
		m_Functions.push_back(std::bind(func, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}

	// TO DO: objects of type std::bind can't compare between each other with using '==' - need to find another way of removing binded functions
	template <class T>
	void Remove(void (T::* func)(ParamType1, ParamType2), T* object)
	{
		auto bind = std::bind(func, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		m_Functions.erase(std::remove(m_Functions.begin(), m_Functions.end(), bind), m_Functions.end());
	}

	void Clear()
	{
		m_Functions.clear();
	}

	void Broadcast(ParamType1 param1, ParamType2 param2, ParamType3 param3, ParamType4 param4)
	{
		for (auto func : m_Functions)
			func(param1, param2, param3, param4);
	}

private:
	std::vector<std::function<void(ParamType1, ParamType2, ParamType3, ParamType4)>> m_Functions;
};