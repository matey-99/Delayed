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

	inline void ShouldWaitUntilAnimationEnd(bool should) { m_ShouldWaitUntilAnimationEnd = should; }

protected:
	Animator* m_Owner;
	std::vector<Ref<AnimatorTransition>> m_ExitTransitions;

	bool m_IsWaiting;

private:
	std::string m_Name;
	bool m_ShouldWaitUntilAnimationEnd;

	float m_StateTime;
};