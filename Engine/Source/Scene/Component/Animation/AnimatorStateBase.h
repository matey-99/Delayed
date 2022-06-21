#pragma once

#include "Core.h"

class Animator;
class AnimatorTransition;

class AnimatorStateBase
{
public:
	AnimatorStateBase();
	AnimatorStateBase(Animator* owner);

	virtual void UpdateState(float deltaTime);
	virtual bool HasAnimationEnd() = 0;
	virtual void StartWaiting();

	void ExitState(Ref<AnimatorTransition> transition);

	void AddExitTransition(Ref<AnimatorTransition> transition);

	inline std::string GetName() const { return m_Name; }

	inline void SetName(const std::string& name) { m_Name = name; }

protected:
	Animator* m_Owner;
	std::vector<Ref<AnimatorTransition>> m_ExitTransitions;

	bool m_ShouldPlayAnimation;
	bool m_IsWaiting;

private:
	std::string m_Name;

	float m_StateTime;
};