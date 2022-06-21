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
	virtual bool HasAnimationEnd() override;
	virtual void StartWaiting() override;

	inline Ref<Animation> GetAnimation() const { return m_Animation; }
	inline float GetAnimationSpeed() const { return m_AnimationSpeed; }
	inline float GetAnimationTime() const { return m_AnimationTime; }

	inline void SetAnimation(Ref<Animation> anim) { m_Animation = anim; }
	inline void SetAnimationSpeed(float speed) { m_AnimationSpeed = speed; }
	inline void SetAnimationTime(float time) { m_AnimationTime = time; }

	inline void SetAnimationID(int id) { m_AnimationID = id; }

private:
	Ref<Animation> m_Animation;
	float m_AnimationSpeed;
	float m_AnimationTime;

	float m_WaitTime;
	bool m_AnimationHasEnded;

	int m_AnimationID;

	friend class ActorDetailsPanel;
};