#pragma once

#include "Core.h"
#include "AnimatorStateBase.h"

class Animation;

class AnimatorState : public AnimatorStateBase
{
public:
	AnimatorState();
	AnimatorState(Animator* owner);

	static Ref<AnimatorState> Create(Animator* owner);

	virtual void UpdateState(float deltaTime) override;

	inline Ref<Animation> GetAnimation() const { return m_Animation; }
	inline float GetAnimationSpeed() const { return m_AnimationSpeed; }

	inline void SetAnimation(Ref<Animation> anim) { m_Animation = anim; }
	inline void SetAnimationSpeed(float speed) { m_AnimationSpeed = speed; }

private:
	Ref<Animation> m_Animation;
	float m_AnimationSpeed;
	float m_AnimationTime;
};