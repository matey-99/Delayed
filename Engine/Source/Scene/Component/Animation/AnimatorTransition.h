#pragma once

#include "Core.h"

class Animator;
class AnimatorStateBase;
class AnimatorState;
class BlendTree;
struct BlendNode;

enum class NumericalConditionType
{
	Equal, Greater, Less
};

struct FloatCondition
{
	std::string Name;
	NumericalConditionType Type;
	float Value;
};

struct BoolCondition
{
	std::string Name;
	bool Value;
};

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
	void AddCondition(std::string paramName, NumericalConditionType type, float value);

	inline Ref<AnimatorStateBase> GetPreviousState() const { return m_PreviousState; }
	inline Ref<AnimatorStateBase> GetNextState() const { return m_NextState; }
	inline bool IsWaitingUntilAnimationEnd() const { return m_WaitUntilAnimationEnd; }

	inline void SetWaitUntilAnimationEnd(bool wait) { m_WaitUntilAnimationEnd = wait; }
	inline void SetTransitionTime(float time) { m_TransitionTime = time; }

private:
	BlendNode GetNearestNode(Ref<BlendTree> tree);

private:
	Animator* m_Owner;
	Ref<AnimatorStateBase> m_PreviousState;
	Ref<AnimatorStateBase> m_NextState;

	std::vector<BoolCondition> m_Conditions;
	std::vector<FloatCondition> m_FloatConditions;

	bool m_WaitUntilAnimationEnd;

	float m_TransitionTime;
	float m_TransitionTimer;

	float m_PreviousAnimationTime;
	float m_NextAnimationTime;

	friend class ActorDetailsPanel;
};