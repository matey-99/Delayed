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
	m_ShouldWaitUntilAnimationEnd = false;

	m_IsWaiting = false;
}

void AnimatorStateBase::UpdateState(float deltaTime)
{
	for (auto transition : m_ExitTransitions)
	{
		if (transition->CheckConditions())
		{
			if (m_ShouldWaitUntilAnimationEnd)
			{
				if (!m_IsWaiting)
				{
					StartWaiting();
				}

				if (HasAnimationEnd())
				{
					ExitState(transition);
					return;
				}
			}
			else
			{
				ExitState(transition);
				return;
			}
		}
	}
}

void AnimatorStateBase::StartWaiting()
{
	m_IsWaiting = true;
}

void AnimatorStateBase::ExitState(Ref<AnimatorTransition> transition)
{
	m_Owner->SetCurrentState(nullptr);
	m_Owner->SetCurrentTransition(transition);
}

void AnimatorStateBase::AddExitTransition(Ref<AnimatorTransition> transition)
{
	m_ExitTransitions.push_back(transition);
}
