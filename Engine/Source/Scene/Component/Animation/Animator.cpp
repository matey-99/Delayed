#include "Animator.h"
#include <Renderer/Bone.h>

void Animator::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;

	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * deltaTime;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		ComputeBoneTransforms(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}

	// Send computed transforms to each SkeletalMesh
	m_SkeletalMeshComponent->PropagateBoneTransforms(m_FinalBoneMatrices);

	// [DEBUG ONLY]
	/*for (auto& bone : m_FinalBoneMatrices)
	{
		for (int i = 0; i < 16; i++)
			std::cout << bone[i / 4][i % 4] << " ";
		std::cout << "\n";
	}*/

}

// This should be somewhere else
void Animator::ComputeBoneTransforms(AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;
	Ref<Bone> bone = m_SkeletalMeshComponent->FindBoneInRig(nodeName);

	glm::mat4 globalTransformation;

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

	// Old stuff:
	//std::string nodeName = node->name;
	//glm::mat4 nodeTransform = node->transformation;
	//Bone* Bone = m_CurrentAnimation->FindBone(nodeName);  // Search for bone with specific name
	//if (Bone)
	//{
	//	Bone->Update(m_CurrentTime);
	//	nodeTransform = Bone->GetLocalTransform();
	//}
	//glm::mat4 globalTransformation = parentTransform * nodeTransform;

	//auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	//if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	//{
	//	int index = boneInfoMap[nodeName].ID;
	//	glm::mat4 offset = boneInfoMap[nodeName].Offset;
	//	m_FinalBoneMatrices[index] = globalTransformation * offset;
	//}
	//for (int i = 0; i < node->childrenCount; i++)
	//	ComputeBoneTransforms(&node->children[i], globalTransformation);
}