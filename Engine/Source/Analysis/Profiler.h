#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#define PROFILER_START(name) if(Profiler::GetInstance()->IsProfiling()) Profiler::GetInstance()->StartProfiling(name);
#define PROFILER_STOP() if(Profiler::GetInstance()->IsProfiling()) Profiler::GetInstance()->StopProfiling();

struct ProfileData
{
	std::string Name;
	std::chrono::system_clock::time_point StartTime;

	ProfileData(std::string name, std::chrono::system_clock::time_point startTime)
		: Name(name), StartTime(startTime) 
	{}
};

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

	std::stack<ProfileData> m_ProfilesData;

	uint64_t m_FrameCounter;
	std::string m_Output;
};