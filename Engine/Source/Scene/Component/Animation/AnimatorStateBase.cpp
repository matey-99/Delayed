#include "AnimatorState.h"

#include "Animator.h"
#include "AnimatorTransition.h"
#include "Time/TimerManager.h"

AnimatorStateBase::AnimatorStateBase()
{

}

AnimatorStateBase::AnimatorStateBase(Animator* owner)
	: m_Owner(owner)
{
	m_IsExiting = false;
}

Ref<AnimatorStateBase> AnimatorStateBase::Create(Animator* owner)
{
	return CreateRef<AnimatorStateBase>(owner);
}

void AnimatorStateBase::UpdateState(float deltaTime)
{
	for (auto transition : m_ExitTransitions)
	{
		if (transition->CheckConditions())
		{
			m_Owner->SetCurrentState(nullptr);
			m_Owner->SetCurrentTransition(transition);
			return;
		}
	}
}

void AnimatorStateBase::AddExitTransition(Ref<AnimatorTransition> transition)
{
	m_ExitTransitions.push_back(transition);
}
