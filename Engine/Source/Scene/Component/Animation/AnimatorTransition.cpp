#include "AnimatorTransition.h"

#include "Animator.h"
#include "AnimatorStateBase.h"
#include "Math/Math.h"

AnimatorTransition::AnimatorTransition()
{
}

AnimatorTransition::AnimatorTransition(Animator* owner, Ref<AnimatorStateBase> previous, Ref<AnimatorStateBase> next)
	: m_Owner(owner), m_PreviousState(previous), m_NextState(next)
{
	m_TransitionTime = 0.25f;
	m_TransitionTimer = 0.0f;

	m_PreviousAnimationTime = 0.0f;
	m_NextAnimationTime = 0.0f;
}

Ref<AnimatorTransition> AnimatorTransition::Create(Animator* owner, Ref<AnimatorStateBase> previous, Ref<AnimatorStateBase> next)
{
	return CreateRef<AnimatorTransition>(owner, previous, next);
}

bool AnimatorTransition::CheckConditions()
{
	bool shouldMakeTransition = true;
	for (auto& condition : m_Conditions)
	{
		auto param = m_Owner->GetBoolParameter(condition.first);
		if (param != condition.second)
		{
			shouldMakeTransition = false;
			break;
		}
	}

	return shouldMakeTransition;
}

void AnimatorTransition::AddCondition(std::string paramName, bool value)
{
	m_Conditions.insert({ paramName, value });
}

BlendNode AnimatorTransition::GetNearestNode(Ref<BlendTree> tree)
{
	float blendValue = m_Owner->GetFloatParameter(tree->GetBlendParameterName());

	int nodeIndex = 0;
	float minDelta = Math::Infinity;
	for (int i = 0; i < tree->GetNodes().size(); i++)
	{
		float delta = glm::abs(tree->GetNodes()[i].BlendLimit - blendValue);
		if (delta < minDelta)
		{
			minDelta = delta;
			nodeIndex = i;
		}
	}

	return tree->GetNodes()[nodeIndex];
}

void AnimatorTransition::UpdateTransition(float deltaTime)
{
	m_TransitionTimer += 1.0f / m_TransitionTime * deltaTime;
	if (m_TransitionTimer >= 1.0f)
	{
		ResetTransition();
		m_Owner->SetCurrentState(m_NextState);
		return;
	}

	Ref<Animation> previousAnim, nextAnim;
	float previousAnimSpeed, nextAnimSpeed;

	if (auto blendTree = Cast<BlendTree>(m_PreviousState))
	{
		BlendNode node = GetNearestNode(blendTree);

		previousAnim = node.Animation;
		previousAnimSpeed = node.AnimationSpeed;
	}
	else if (auto state = Cast<AnimatorState>(m_PreviousState))
	{
		previousAnim = state->GetAnimation();
		previousAnimSpeed = state->GetAnimationSpeed();
	}

	if (auto blendTree = Cast<BlendTree>(m_NextState))
	{
		BlendNode node = GetNearestNode(blendTree);

		nextAnim = node.Animation;
		nextAnimSpeed = node.AnimationSpeed;
	}
	else if (auto state = Cast<AnimatorState>(m_NextState))
	{
		nextAnim = state->GetAnimation();
		nextAnimSpeed = state->GetAnimationSpeed();
	}

	float blendValue = m_TransitionTimer;

	float pAnimMul = previousAnimSpeed;
	float nAnimMul = previousAnim->GetDuration() / nextAnim->GetDuration();
	float speedMultiplierUp = (1.0f - blendValue) * pAnimMul + nAnimMul * blendValue;

	pAnimMul = nextAnim->GetDuration() / previousAnim->GetDuration();
	nAnimMul = nextAnimSpeed;
	float speedMultiplierDown = (1.0f - blendValue) * pAnimMul + nAnimMul * blendValue;

	m_PreviousAnimationTime += previousAnim->GetTicksPerSecond() * deltaTime * speedMultiplierUp;
	m_PreviousAnimationTime = fmod(m_PreviousAnimationTime, previousAnim->GetDuration());

	m_NextAnimationTime += nextAnim->GetTicksPerSecond() * deltaTime * speedMultiplierDown;
	m_NextAnimationTime = fmod(m_NextAnimationTime, nextAnim->GetDuration());

	m_Owner->CalculateBlendedBoneTransform(
		previousAnim, &previousAnim->GetRootNode(),
		nextAnim, &nextAnim->GetRootNode(),
		m_PreviousAnimationTime, m_NextAnimationTime,
		glm::mat4(1.0f), blendValue);
}

void AnimatorTransition::ResetTransition()
{
	m_TransitionTimer = 0.0f;
	m_PreviousAnimationTime = 0.0f;
	m_NextAnimationTime = 0.0f;
}
