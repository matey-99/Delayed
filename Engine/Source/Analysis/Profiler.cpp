#include "Profiler.h"

#include "Time/Time.h"

Profiler::Profiler()
{
	m_Enabled = false;
	m_Profiling = false;
	m_ElapsedTimeToProfile = 5.0f;
	m_ProfilerTimer = 0.0f;
	m_FrameCounter = 0;
}

Profiler::~Profiler()
{
	Serialize();
}

void Profiler::Update()
{
	m_FrameCounter++;

	m_Profiling = false;

	m_ProfilerTimer += Time::GetInstance()->GetDeltaTime();
	if (m_ProfilerTimer >= m_ElapsedTimeToProfile)
	{
		m_Profiling = true;
		m_ProfilerTimer = 0.0f;
	}
}

void Profiler::StartProfiling(std::string name)
{
	m_CurrentProfilingName = name;
	m_StartProfilingTime = std::chrono::system_clock::now();
}

void Profiler::StopProfiling()
{
	m_StopProfilingTime = std::chrono::system_clock::now();
	std::chrono::duration<float> executionTime = m_StopProfilingTime - m_StartProfilingTime;

	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	m_Output += "Profile Name: " + m_CurrentProfilingName + '\n';
	m_Output += "System Time: " + std::string(std::ctime(&currentTime));
	m_Output += "Frame: " + std::to_string(m_FrameCounter) + '\n';
	m_Output += "Execution Time: " + std::to_string(executionTime.count()) + '\n';
}

void Profiler::Serialize()
{
	std::ofstream file("../../../Analysis/ProfilerOutput.txt");
	file << m_Output.c_str();
	file.close();
}

bool Profiler::IsProfiling()
{
	return m_Enabled && m_Profiling;
}

