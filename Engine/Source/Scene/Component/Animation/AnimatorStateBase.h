#pragma once

#include "Core.h"

class Animator;
class AnimatorTransition;

class AnimatorStateBase
{
public:
	AnimatorStateBase();
	AnimatorStateBase(Animator* owner);

	static Ref<AnimatorStateBase> Create(Animator* owner);

	virtual void UpdateState(float deltaTime);

	void ExitState(Ref<AnimatorTransition> transition, float deltaTime);

	void AddExitTransition(Ref<AnimatorTransition> transition);

protected:
	Animator* m_Owner;
	std::vector<Ref<AnimatorTransition>> m_ExitTransitions;

private:
	std::string m_Name;
	bool m_IsExiting;
};