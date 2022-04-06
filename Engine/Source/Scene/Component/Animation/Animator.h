#pragma once
#include "Animation.h"
#include "typedefs.h"
#include <glm/glm.hpp>
#include <assimp/anim.h>  // ?
#include <vector>

class Animator
{
	Ref<Animation> m_CurrentAnimation;
	std::vector<glm::mat4> m_FinalBoneMatrices;
	float m_CurrentTime;
	float m_DeltaTime;

public:
	Animator(Ref<Animation> animationToPlay)
	{
		m_CurrentAnimation = animationToPlay;
		// [...]
	}

	void PlayAnimation();
	void Update(float deltaTime);
	void ComputeBoneTransforms(aiNodeAnim* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices();

};