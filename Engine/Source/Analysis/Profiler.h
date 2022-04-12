#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#define PROFILER_START(name) if(Profiler::GetInstance()->IsProfiling()) Profiler::GetInstance()->StartProfiling(name);
#define PROFILER_STOP() if(Profiler::GetInstance()->IsProfiling()) Profiler::GetInstance()->StopProfiling();

class Profiler : public Singleton<Profiler>
{
public:
	Profiler();
	~Profiler();

	void Update();

	void StartProfiling(std::string name);
	void StopProfiling();

	void Serialize();

	bool IsProfiling();

	inline bool IsEnabled() const { return m_Enabled; }
	inline void SetEnabled(bool enabled) { m_Enabled = enabled; }

private:
	bool m_Enabled;
	bool m_Profiling;
	float m_ElapsedTimeToProfile;
	float m_ProfilerTimer;

	std::string m_CurrentProfilingName;
	std::chrono::system_clock::time_point m_StartProfilingTime;
	std::chrono::system_clock::time_point m_StopProfilingTime;

	uint64_t m_FrameCounter;
	std::string m_Output;
};