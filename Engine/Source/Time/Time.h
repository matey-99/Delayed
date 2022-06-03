#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"
#include "Scene/Scene.h"

#define FIXED_TIME_FPS 60.0f
#define FIXED_TIME_MS_PER_SECOND 1 / FIXED_TIME_FPS

class Time : public Singleton<Time>
{
public:
	Time();
	~Time();

	void Tick(Ref<Scene> scene);

	/* Getters */
	inline float GetElapsedTime() const { return m_ElapsedTime; }
	inline float GetDeltaTime() const { return m_DeltaTime; }
	inline float GetMaximumDeltaTime() const { return m_MaximumDeltaTime; }

	inline float GetFixedElapsedTime() const { return m_FixedElapsedTime; }
	inline float GetFixedDeltaTime() const { return m_FixedDeltaTime; }

	inline float GetLastFrameTime() const { return m_LastFrameTime; }
	inline float GetCurrentFrameTime() const { return m_CurrentFrameTime; }

	/* Setters */
	inline void SetLastFrameTime(float lastFrameTime) { m_LastFrameTime = lastFrameTime; }
	inline void SetCurrentFrameTime(float currentFrameTime) { m_CurrentFrameTime = currentFrameTime; }
	inline void SetTimeScale(float timeScale) { m_TimeScale = timeScale; }

private:
	float m_ElapsedTime;
	float m_DeltaTime;
	float m_MaximumDeltaTime;

	float m_FixedElapsedTime;
	float m_FixedDeltaTime;

	float m_LastFrameTime;
	float m_CurrentFrameTime;

	float m_TimeScale;
};