#pragma once

#include <mutex>

#include "typedefs.h"

template <typename T>
class Singleton
{
public:
	Singleton() {};
	~Singleton() {};

	Singleton(Singleton& other) = delete;
	void operator=(const Singleton&) = delete;

	static Ref<T> GetInstance()
	{
		std::lock_guard<std::mutex> lock(s_Mutex);
		if (!s_Instance)
			s_Instance = CreateRef<T>();

		return s_Instance;
	}

private:
	static Ref<T> s_Instance;
	static std::mutex s_Mutex;
};

template <typename T>
Ref<T> Singleton<T>::s_Instance{};

template <typename T>
std::mutex Singleton<T>::s_Mutex;