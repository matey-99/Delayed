#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"
#include "Patterns/Event.h"
#include "Patterns/Delegate.h"

class TimerManager : public Singleton<TimerManager>
{
private:
	struct Timer
	{
		Timer(Event e, float rate, bool loop, float delay = 0.0f)
			: Event(e), Rate(rate), Loop(loop), Delay(delay)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution distribution(1, 999999999);

			Handle = distribution(gen);

			Time = 0.0f;
			IsDelay = Delay > 0.0f;
		}

		TimerHandle Handle;
		Event Event;
		float Rate;
		bool Loop;
		float Delay;

		float Time;
		bool IsDelay;
	};

public:
	TimerManager();
	~TimerManager();

	void Tick(float deltaTime);

	const TimerHandle& SetTimer(Event e, float rate, bool loop, float delay = 0.0f);

	void ClearTimer(const TimerHandle& handle);

private:
	std::vector<Timer> m_Timers;
};