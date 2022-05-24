#include "Animator.h"
#include <Renderer/Bone.h>

void Animator::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;

	// one-animation-mode
	/*if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * deltaTime;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		ComputeBoneTransforms(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}*/

	// Invoke BlendAnimations somewhere there
	if (m_Animations[0] && m_Animations[1])
	{
		BlendAnimations(m_Animations[0], m_Animations[1]);
	}

	// Send computed transforms to each SkeletalMesh
	m_SkeletalMeshComponent->PropagateBoneTransforms(m_FinalBoneMatrices);
}

// This should be somewhere else
void Animator::ComputeBoneTransforms(AssimpNodeData* node, glm::mat4 parentTransform)
{
	// New, with BoneMap:
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;
	glm::mat4 globalTransformation;

	Ref<Bone> bone = m_CurrentAnimation->FindBone(nodeName);

	if (bone != nullptr)
	{
		bone->Update(m_CurrentTime);

		nodeTransform = bone->GetLocalTransform();
		globalTransformation = parentTransform * nodeTransform;
		m_FinalBoneMatrices[bone->GetID()] = globalTransformation * bone->GetOffset(); // * m_GlobalInverseTransform (1:35:00 ogl)
	}
	else
	{
		globalTransformation = parentTransform * nodeTransform;
	}

	for (int i = 0; i < node->childrenCount; i++)
	{
		ComputeBoneTransforms(&node->children[i], globalTransformation);
	}
}

// Blends between two animations
void Animator::BlendAnimations(Ref<Animation> pAnim, Ref<Animation> lAnim)
{
	// Blend Factor: 0 means pAnim is played at 100%

	// Speed multipliers
	float pAnimMul = 1.0f;
	float lAnimMul = pAnim->GetDuration() / lAnim->GetDuration();
	const float speedMultiplierUp = (1.0f - m_BlendFactor) * pAnimMul + lAnimMul * m_BlendFactor;  // Lerp

	pAnimMul = lAnim->GetDuration() / pAnim->GetDuration();
	lAnimMul = 1.0f;
	const float speedMultiplierDown = (1.0f - m_BlendFactor) * pAnimMul + lAnimMul * m_BlendFactor;  // Also lerp

	static float pCurrentTime = 0.0f;
	pCurrentTime += pAnim->GetTicksPerSecond() * m_DeltaTime * speedMultiplierUp;  // Not lerp
	pCurrentTime = fmod(pCurrentTime, pAnim->GetDuration());

	static float lCurrentTime = 0.0;
	lCurrentTime += lAnim->GetTicksPerSecond() * m_DeltaTime * speedMultiplierDown;  // Still not lerp
	lCurrentTime = fmod(lCurrentTime, lAnim->GetDuration());

	// Blend two animations (into m_FinalBoneMatrices)
	ComputeBlendedBoneTransforms(pAnim, &pAnim->GetRootNode(), lAnim, &lAnim->GetRootNode(), pCurrentTime, lCurrentTime, glm::mat4(1.0f));
}

void Animator::ComputeBlendedBoneTransforms(Ref<Animation> pAnim, const AssimpNodeData* pNode, Ref<Animation> lAnim, const AssimpNodeData* lNode, const float pCurrentTime, const float lCurrentTime, glm::mat4 parentTransform)
{
	const std::string& nodeName = pNode->name;
	glm::mat4 pNodeTransform = pNode->transformation;
	glm::mat4 lNodeTransform = lNode->transformation;

	Ref<Bone> pBone = pAnim->FindBone(nodeName);
	if (pBone)
	{
		pBone->Update(pCurrentTime);
		pNodeTransform = pBone->GetLocalTransform();
	}

	Ref<Bone> lBone = lAnim->FindBone(nodeName);
	if (lBone)
	{
		lBone->Update(lCurrentTime);
		lNodeTransform = lBone->GetLocalTransform();
	}

	// Blend two matrices
	glm::quat blendedRot = glm::slerp  // Slerp
	(
		glm::quat_cast(pNodeTransform),
		glm::quat_cast(lNodeTransform),
		m_BlendFactor
	);
	glm::mat4 blendedMat = glm::mat4_cast(blendedRot);
	blendedMat[3] = (1.0f - m_BlendFactor) * pNodeTransform[3] + lNodeTransform[3] * m_BlendFactor;  // Yes, it is lerp

	glm::mat4 globalTransform = parentTransform * blendedMat;

	Ref<BoneMap> mappedBone = m_SkeletalMeshComponent->FindBoneInRig(nodeName);
	if (mappedBone)
	{
		m_FinalBoneMatrices[mappedBone->ID]
			= globalTransform * mappedBone->OffsetMatrix;
	}

	for (size_t i = 0; i < pNode->children.size(); ++i)
	{
		ComputeBlendedBoneTransforms(pAnim, &pNode->children[i], lAnim, &lNode->children[i], pCurrentTime, lCurrentTime, globalTransform);
	}


	// stackoverflow
	/*const glm::quat rot0 = glm::quat_cast(pNodeTransform);
	const glm::quat rot1 = glm::quat_cast(layeredNodeTransform);
	const glm::quat finalRot = glm::slerp(rot0, rot1, blendFactor);
	glm::mat4 blendedMat = glm::mat4_cast(finalRot);
	blendedMat[3] = (1.0f - blendFactor) * nodeTransform[3] + layeredNodeTransform[3] * blendFactor;

	glm::mat4 globalTransformation = parentTransform * blendedMat;

	const auto& boneInfoMap = pAnimationBase->GetBoneInfoMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		const int index = boneInfoMap.at(nodeName).id;
		const glm::mat4& offset = boneInfoMap.at(nodeName).offset;

		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (size_t i = 0; i < node->children.size(); ++i)
		CalculateBlendedBoneTransform(pAnimationBase, &node->children[i], pAnimationLayer, &nodeLayered->children[i], currentTimeBase, currentTimeLayered, globalTransformation, blendFactor);*/

}

