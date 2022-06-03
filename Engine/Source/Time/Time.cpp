#include "Time.h"
#include "Analysis/Profiler.h"
#include "TimerManager.h"

Time::Time()
{
	m_ElapsedTime = 0.0f;
	m_DeltaTime = 0.0f;
	m_MaximumDeltaTime = 0.033f;

	m_FixedElapsedTime = 0.0f;
	m_FixedDeltaTime = 0.02f;

	m_LastFrameTime = 0.0f;
	m_CurrentFrameTime = 0.0f;

	m_TimeScale = 1.0f;
}

Time::~Time()
{
}

void Time::Tick(Ref<Scene> scene)
{
	m_DeltaTime = m_CurrentFrameTime - m_LastFrameTime;
	m_LastFrameTime = m_CurrentFrameTime;
	m_ElapsedTime += m_DeltaTime;

	if (m_DeltaTime > m_MaximumDeltaTime)
		m_DeltaTime = m_MaximumDeltaTime;

	PROFILER_START("Update Time");
	scene->Update(m_DeltaTime);
	PROFILER_STOP();

	PROFILER_START("Fixed Update Time");
	scene->FixedUpdate();
	PROFILER_STOP();

	TimerManager::GetInstance()->Tick(m_DeltaTime);
}
