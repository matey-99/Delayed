#include "TimerManager.h"

TimerManager::TimerManager()
{
}

TimerManager::~TimerManager()
{
}

void TimerManager::Tick(float deltaTime)
{
	for (auto& timer : m_Timers)
	{
		timer.Time += deltaTime;

		if (timer.IsDelay)
		{
			if (timer.Time < timer.Delay)
				continue;
			else
				timer.IsDelay = false;
		}

		if (timer.Time >= timer.Rate)
		{
			timer.Event.Broadcast();

			if (timer.Loop)
				timer.Time = 0.0f;
		}
	}
}

const TimerHandle& TimerManager::SetTimer(Event e, float rate, bool loop, float delay)
{
	Timer t = Timer(e, rate, loop, delay);
	m_Timers.push_back(t);

	return t.Handle;
}

void TimerManager::ClearTimer(TimerHandle& handle)
{
	m_Timers.erase(std::remove_if(m_Timers.begin(), m_Timers.end(), [handle](const Timer& timer) {
		return handle == timer.Handle;
	}), m_Timers.end());

	handle = 0;
}
