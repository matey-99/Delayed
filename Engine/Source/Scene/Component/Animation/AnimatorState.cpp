#include "AnimatorState.h"

#include "Animator.h"

AnimatorState::AnimatorState()
	: AnimatorStateBase() 
{
}

AnimatorState::AnimatorState(Animator* owner)
	: AnimatorStateBase(owner) 
{
	m_AnimationTime = 0.0f;
}

Ref<AnimatorState> AnimatorState::Create(Animator* owner)
{
	return CreateRef<AnimatorState>(owner);
}

void AnimatorState::UpdateState(float deltaTime)
{
	AnimatorStateBase::UpdateState(deltaTime);

	m_AnimationTime += m_Animation->GetTicksPerSecond() * deltaTime;
	m_AnimationTime = fmod(m_AnimationTime, m_Animation->GetDuration());

	m_Owner->CalculateBoneTransform(m_Animation, &m_Animation->GetRootNode(), m_AnimationTime, glm::mat4(1.0f));
}
