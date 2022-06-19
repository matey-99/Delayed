#pragma once

#include "Core.h"

class Animator;
class AnimatorStateBase;

class AnimatorTransition
{
public:
	AnimatorTransition();
	AnimatorTransition(Animator* owner, Ref<AnimatorStateBase> previous, Ref<AnimatorStateBase> next);

	static Ref<AnimatorTransition> Create(Animator* owner, Ref<AnimatorStateBase> previous, Ref<AnimatorStateBase> next);

	bool CheckConditions();
	void UpdateTransition(float deltaTime);
	void ResetTransition();

	void AddCondition(std::string paramName, bool value);

	inline Ref<AnimatorStateBase> GetPreviousState() const { return m_PreviousState; }
	inline Ref<AnimatorStateBase> GetNextState() const { return m_NextState; }

private:

private:
	Animator* m_Owner;
	Ref<AnimatorStateBase> m_PreviousState;
	Ref<AnimatorStateBase> m_NextState;

	std::unordered_map<std::string, bool> m_Conditions;

	float m_TransitionTime;
	float m_TransitionTimer;

	float m_PreviousAnimationTime;
	float m_NextAnimationTime;
};