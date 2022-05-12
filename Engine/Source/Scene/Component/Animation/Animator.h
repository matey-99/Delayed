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

public:
	Animator(Actor* owner) : Component(owner)
	{
		FindSkeletalMeshComponent();

		m_CurrentTime = 0.0;
		m_DeltaTime = 0.0f;

		m_FinalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

		if (m_SkeletalMeshComponent->HowManyAnimations() > 0)
			m_CurrentAnimation = m_SkeletalMeshComponent->GetAnimation(1);

	}

	virtual void Start() override
	{
		// Load default animation file here
	}

	virtual void Update(float deltaTime) override;

	virtual void FixedUpdate() override { }

	virtual void Destroy() override { }

	void PlayAnimation(Ref<Animation> animationToPlay)
	{
		//m_CurrentAnimation = animationToPlay;
		//m_CurrentTime = 0.0f;
	}

	size_t HowManyAnimationsAreThere()
	{
		return m_SkeletalMeshComponent->HowManyAnimations();
	}

	float GetCurrentAnimationTime()
	{
		return m_CurrentTime;
	}

	std::string GetCurrentAnimationName() { return m_CurrentAnimation->GetAnimationName(); }

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

	void FindSkeletalMeshComponent()
	{
		m_SkeletalMeshComponent = m_Owner->GetComponent<SkeletalMeshComponent>();

		if (m_SkeletalMeshComponent->HowManyAnimations() > 0)
			m_CurrentAnimation = m_SkeletalMeshComponent->GetAnimation(1);  // winowajca
	}

	void ComputeBoneTransforms(AssimpNodeData* node, glm::mat4 parentTransform);

	//std::vector<glm::mat4> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }


};
