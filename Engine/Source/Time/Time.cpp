#include "Time.h"

Time::Time()
{
	m_ElapsedTime = 0.0f;
	m_DeltaTime = 0.0f;
	m_MaximumDeltaTime = 0.033f;

	m_FixedElapsedTime = 0.0f;
	m_FixedDeltaTime = 0.01f;

	m_LastFrameTime = 0.0f;
	m_CurrentFrameTime = 0.0f;
}

Time::~Time()
{
}

void Time::Tick(Ref<Scene> scene)
{
	m_DeltaTime = m_CurrentFrameTime - m_LastFrameTime;
	m_LastFrameTime = m_CurrentFrameTime;

	if (m_DeltaTime > m_MaximumDeltaTime)
		m_DeltaTime = m_MaximumDeltaTime;

	// even first should update animation
	scene->UpdateAnimation(m_DeltaTime);

	// physics & collisions should be calculated first
	scene->Update(m_DeltaTime);
	m_ElapsedTime += m_DeltaTime;

	while (m_FixedElapsedTime + m_FixedDeltaTime <= m_ElapsedTime)
	{
		scene->FixedUpdate();
		m_FixedElapsedTime += m_FixedDeltaTime;
	}

}
