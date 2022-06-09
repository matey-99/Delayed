#pragma once
#include "Animation.h"
#include "typedefs.h"
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <vector>
#include "Math/AssimpGLMHelper.h"
#include "../Component.h"
#include "SkeletalMeshComponent.h"
#include "../../Actor.h"

class Animator : public Component
{
	Ref<SkeletalMeshComponent> m_SkeletalMeshComponent;
	Ref<Animation> m_CurrentAnimation;
	std::vector<glm::mat4> m_FinalBoneMatrices;  // To feed shader with
	float m_CurrentTime;
	float m_DeltaTime;
	int m_CurrentAnimationNumber = 0;

	Ref<Animation> m_Animations[3];  // Animations to blend between. Uses m_BlendFactor. Use this instead of m_CurrentAnimation.


public:
	Animator(Actor* owner) : Component(owner)
	{
		FindSkeletalMeshComponent();

		m_CurrentTime = 0.0;
		m_DeltaTime = 0.0f;

		m_FinalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

	}

	virtual void Start() override { };

	virtual void Update(float deltaTime) override;

	virtual void FixedUpdate() override { }

	virtual void Destroy() override { }

	void PlayAnimation(Ref<Animation> animationToPlay)
	{
		//m_CurrentAnimation = animationToPlay;
		//m_CurrentTime = 0.0f;
	}

	size_t HowManyAnimationsAreThere() { return m_SkeletalMeshComponent->HowManyAnimations(); }

	float GetCurrentAnimationTime() { return m_CurrentTime; }

	//std::string GetCurrentAnimationName() { return m_Animations->GetAnimationName(); }

	bool HasSkeletalMeshComponent()
	{
		if (m_SkeletalMeshComponent)
			return true;
		else if (m_SkeletalMeshComponent == nullptr)
			return false;
	}

	void DebugDisplayAnimationNames()
	{
		for (int index = 0; index < m_SkeletalMeshComponent->HowManyAnimations(); index++)
		{
			m_SkeletalMeshComponent->GetAnimation(index)->DebugDisplayAnimationName();
		}
	}
	void DebugSwitchAnimation()
	{
		m_CurrentAnimationNumber++;
		m_CurrentAnimation = m_SkeletalMeshComponent->GetAnimation(m_CurrentAnimationNumber % m_SkeletalMeshComponent->HowManyAnimations());
	}

	void FindSkeletalMeshComponent()
	{
		m_SkeletalMeshComponent = m_Owner->GetComponent<SkeletalMeshComponent>();

		if (m_SkeletalMeshComponent->HowManyAnimations() > 0)
			m_CurrentAnimation = m_SkeletalMeshComponent->GetAnimation(0);  // winowajca

		if (m_SkeletalMeshComponent->HowManyAnimations() > 3)
		{
			SetAnimation1(m_SkeletalMeshComponent->GetAnimation(3));
			SetAnimation2(m_SkeletalMeshComponent->GetAnimation(1));
			SetAnimation3(m_SkeletalMeshComponent->GetAnimation(2));
		}
	}

	std::vector<Ref<Animation>> GetAnimations() { return m_SkeletalMeshComponent->GetAnimations(); }
	Ref<Animation> GetAnimation(int index) { return m_Animations[index]; }
	void SetAnimation1(Ref<Animation> anim) { if (anim) m_Animations[0] = anim; }
	void SetAnimation2(Ref<Animation> anim) { if (anim) m_Animations[1] = anim; }
	void SetAnimation3(Ref<Animation> anim) { if (anim) m_Animations[2] = anim; }
	void SetBlendFactor(float factor);

	void ComputeBoneTransforms(AssimpNodeData* node, glm::mat4 parentTransform);
	void BlendAnimations(Ref<Animation> pAnim, Ref<Animation> lAnim, Ref<Animation> aAnim);
	void ComputeBlendedBoneTransforms(
		Ref<Animation> pAnim, const AssimpNodeData* pNode,
		Ref<Animation> lAnim, const AssimpNodeData* lNode,
		const float pCurrentTime, const float lCurrentTime,
		glm::mat4 parentTransform );

	float m_BlendFactor = 0.f;
	float m_BlendFactor2 = 0.f;

	float m_PAnimSpeed = 1.0f;
	float m_LAnimSpeed = 1.0f;
};