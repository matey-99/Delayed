#pragma once
#include "Animation.h"
#include "typedefs.h"
#include "Importer/SkeletalMeshImporter.h"
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
	Ref<Animation> m_CurrentAnimation;
	std::vector<glm::mat4> m_FinalBoneMatrices;
	float m_CurrentTime;
	float m_DeltaTime;

	Ref<SkeletalMeshComponent> m_SkeletalMeshComponent;
	std::vector<Ref<Animation>> m_Animations;

public:
	Animator(Actor* owner) : Component(owner)
	{
		FindSkeletalMeshComponent();

		m_CurrentTime = 0.0;

		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}

	virtual void Start() override
	{
		// Load default animation file here
	}

	virtual void Update(float deltaTime) override
	{
		m_DeltaTime = deltaTime;

		if (m_CurrentAnimation)
		{
			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * deltaTime;
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
			ComputeBoneTransforms(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	virtual void FixedUpdate() override { }

	virtual void Destroy() override { }

	void PlayAnimation(Ref<Animation> animationToPlay)
	{
		m_CurrentAnimation = animationToPlay;
		m_CurrentTime = 0.0f;
	}

	uint32_t HowManyAnimationsAreThere()
	{
		return m_Animations.size();
	}

	float GetCurrentAnimationTime()
	{
		return m_CurrentTime;
	}

	bool HasSkeletalMeshComponent()
	{
		if (m_SkeletalMeshComponent)
			return true;
		else if (m_SkeletalMeshComponent == nullptr)
			return false;
	}

	void FindSkeletalMeshComponent()
	{
		m_SkeletalMeshComponent = m_Owner->GetComponent<SkeletalMeshComponent>();
	}

	void ComputeBoneTransforms(AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }

};
