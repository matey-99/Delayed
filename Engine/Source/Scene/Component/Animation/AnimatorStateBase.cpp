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
	m_ShouldPlayAnimation = true;

	m_IsWaiting = false;
}

void AnimatorStateBase::UpdateState(float deltaTime)
{
	for (auto transition : m_ExitTransitions)
	{
		if (transition->CheckConditions())
		{
			if (transition->IsWaitingUntilAnimationEnd())
			{
				if (!m_IsWaiting)
				{
					m_IsWaiting = true;
				}

				if (HasAnimationEnd())
				{
					m_ShouldPlayAnimation = false;
					ExitState(transition);
					return;
				}
			}
			else
			{
				m_ShouldPlayAnimation = false;
				ExitState(transition);
				return;
			}
		}
	}

	m_ShouldPlayAnimation = true;
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
