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
}

Ref<AnimatorState> AnimatorState::Create(Animator* owner)
{
	return CreateRef<AnimatorState>(owner);
}

void AnimatorState::UpdateState(float deltaTime)
{
	AnimatorStateBase::UpdateState(deltaTime);

	m_AnimationTime += m_Animation->GetTicksPerSecond() * deltaTime * m_AnimationSpeed;
	m_AnimationTime = fmod(m_AnimationTime, m_Animation->GetDuration());

	m_Owner->CalculateBoneTransform(m_Animation, &m_Animation->GetRootNode(), m_AnimationTime, glm::mat4(1.0f));
}

bool AnimatorState::HasAnimationEnd()
{
	float delta = glm::abs(m_AnimationTime - m_WaitTime);
	m_WaitTime += delta;

	if (m_WaitTime >= m_Animation->GetDuration())
	{
		m_WaitTime = 0.0f;
		m_IsWaiting = false;

		return true;
	}

	return false;
}

void AnimatorState::StartWaiting()
{
	AnimatorStateBase::StartWaiting();

	m_WaitTime = m_AnimationTime;
}
