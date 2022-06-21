#include "AnimatorState.h"

#include "Animator.h"

AnimatorState::AnimatorState()
	: AnimatorStateBase() 
{
}

AnimatorState::AnimatorState(Animator* owner)
	: AnimatorStateBase(owner) 
{
	m_AnimationSpeed = 1.0f;
	m_AnimationTime = 0.0f;

	m_WaitTime = 0.0f;
	m_AnimationHasEnded = false;
}

Ref<AnimatorState> AnimatorState::Create(Animator* owner)
{
	return CreateRef<AnimatorState>(owner);
}

void AnimatorState::UpdateState(float deltaTime)
{
	AnimatorStateBase::UpdateState(deltaTime);

	if (!m_ShouldPlayAnimation)
		return;

	if (m_IsWaiting)
	{
		float animTime = m_AnimationTime + m_Animation->GetTicksPerSecond() * deltaTime * m_AnimationSpeed;
		if (animTime >= m_Animation->GetDuration())
		{
			m_AnimationHasEnded = true;
			return;
		}
	}

	m_AnimationTime += m_Animation->GetTicksPerSecond() * deltaTime * m_AnimationSpeed;
	m_AnimationTime = fmod(m_AnimationTime, m_Animation->GetDuration());

	m_Owner->CalculateBoneTransform(m_Animation, &m_Animation->GetRootNode(), m_AnimationTime, glm::mat4(1.0f));
}

bool AnimatorState::HasAnimationEnd()
{
	if (m_AnimationHasEnded)
	{
		m_IsWaiting = false;
		m_AnimationHasEnded = false;

		return true;
	}

	return false;
}

void AnimatorState::StartWaiting()
{
	AnimatorStateBase::StartWaiting();

	m_WaitTime = m_AnimationTime;
}
