#pragma once

#include "Core.h"
#include "Animation.h"
#include "Math/AssimpGLMHelper.h"
#include "Scene/Component/Component.h"
#include "SkeletalMeshComponent.h"
#include "Scene/Actor.h"
#include "AnimatorState.h"
#include "BlendTree.h"

class Animator : public Component
{
public:
	Animator(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	void CalculateBoneTransform(Ref<Animation> anim, const AssimpNodeData* node, float animTime, const glm::mat4& parentTransform);

	void CalculateBlendedBoneTransform(Ref<Animation> previousAnim, const AssimpNodeData* pNode, 
		Ref<Animation> nextAnim, const AssimpNodeData* nNode, 
		float previousAnimTime, float nextAnimTime, 
		const glm::mat4& parentTransform, float blendValue);

	bool GetBoolParameter(std::string parameterName);
	float GetFloatParameter(std::string parameterName);

	void SetBoolParameter(std::string parameterName, bool value);
	void SetFloatParameter(std::string parameterName, float value);

	inline void SetCurrentState(Ref<AnimatorStateBase> state) { m_CurrentState = state; }
	inline void SetCurrentTransition(Ref<AnimatorTransition> transition) { m_CurrentTransition = transition; }

private:
	std::vector<Ref<AnimatorStateBase>> m_States;
	std::vector<Ref<AnimatorTransition>> m_Transitions;
	std::unordered_map<std::string, bool> m_BoolParameters;
	std::unordered_map<std::string, float> m_FloatParameters;

	Ref<AnimatorStateBase> m_CurrentState;
	Ref<AnimatorTransition> m_CurrentTransition;
	
	Ref<SkeletalMeshComponent> m_SkeletalMeshComponent;
	std::vector<glm::mat4> m_FinalBoneMatrices;
	float m_DeltaTime;
	int m_CurrentAnimationNumber = 0;
	

	// To serialize animations
	std::vector<int> m_AnimationsIDs;
	
	friend class ActorDetailsPanel;
};
