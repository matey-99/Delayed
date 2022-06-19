#include "Animator.h"
#include <Renderer/Bone.h>
#include "Assets/SkeletalModel.h"
#include "Assets/AssetManager.h"
#include "AnimatorTransition.h"

Animator::Animator(Actor* owner)
	: Component(owner)
{
	m_FinalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++)
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

	m_CurrentTransition = nullptr;

	Ref<BlendTree> blendTree = BlendTree::Create(this);
	m_States.push_back(blendTree);
	m_CurrentState = blendTree;

	Ref<AnimatorState> jumpState = AnimatorState::Create(this);
	m_States.push_back(jumpState);
}

void Animator::Start()
{
	m_SkeletalMeshComponent = m_Owner->GetComponent<SkeletalMeshComponent>();
	if (!m_SkeletalMeshComponent)
		m_SkeletalMeshComponent = m_Owner->AddComponent<SkeletalMeshComponent>();

	m_FloatParameters.insert({ "Speed", 0.0f });
	m_BoolParameters.insert({ "IsJumping", false });

	// TEMPORARY
	BlendNode IdleNode;
	IdleNode.AnimationSpeed = 1.0f;
	IdleNode.BlendLimit = 0.0f;
	IdleNode.Animation = m_SkeletalMeshComponent->GetAnimation(3);

	BlendNode RunNode;
	RunNode.AnimationSpeed = 1.0f;
	RunNode.BlendLimit = 1.0f;
	RunNode.Animation = m_SkeletalMeshComponent->GetAnimation(1);

	Ref<BlendTree> blendTree = Cast<BlendTree>(m_CurrentState);
	blendTree->SetBlendParemeterName("Speed");
	blendTree->AddNode(IdleNode);
	blendTree->AddNode(RunNode);

	Ref<AnimatorState> jumpState = Cast<AnimatorState>(m_States[1]);
	jumpState->SetAnimation(m_SkeletalMeshComponent->GetAnimation(2));
	jumpState->SetAnimationSpeed(1.0f);

	Ref<AnimatorTransition> t1 = AnimatorTransition::Create(this, blendTree, jumpState);
	t1->AddCondition("IsJumping", true);

	Ref<AnimatorTransition> t2 = AnimatorTransition::Create(this, jumpState, blendTree);
	t2->AddCondition("IsJumping", false);

	blendTree->AddExitTransition(t1);
	jumpState->AddExitTransition(t2);
}

void Animator::Update(float deltaTime)
{
	if (m_CurrentState)
		m_CurrentState->UpdateState(deltaTime);
	else if (m_CurrentTransition)
		m_CurrentTransition->UpdateTransition(deltaTime);

	// Send to SkeletalModel
	m_SkeletalMeshComponent->PropagateBoneTransforms(m_FinalBoneMatrices);
}

void Animator::Destroy()
{
}

void Animator::CalculateBoneTransform(Ref<Animation> anim, const AssimpNodeData* node, float animTime, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	glm::mat4 globalTransform;

	Ref<Bone> bone = anim->FindBone(nodeName);
	if (bone)
	{
		bone->Update(animTime);
		nodeTransform = bone->GetLocalTransform();

		globalTransform = parentTransform * nodeTransform;
		
		Ref<BoneMap> mappedBone = m_SkeletalMeshComponent->FindBoneInRig(nodeName);
		if (mappedBone)
			m_FinalBoneMatrices[mappedBone->ID] = globalTransform * mappedBone->OffsetMatrix;
	}
	else
	{
		globalTransform = parentTransform * nodeTransform;
	}

	for (size_t i = 0; i < node->children.size(); ++i)
	{
		CalculateBoneTransform(anim, &node->children[i], animTime, globalTransform);
	}
}

void Animator::CalculateBlendedBoneTransform(Ref<Animation> previousAnim, const AssimpNodeData* pNode, 
	Ref<Animation> nextAnim, const AssimpNodeData* nNode, 
	float previousAnimTime, float nextAnimTime, 
	const glm::mat4& parentTransform, float blendValue)
{
	const std::string& nodeName = pNode->name;

	glm::mat4 pNodeTransform = pNode->transformation;
	glm::mat4 nNodeTransform = nNode->transformation;
	
	Ref<Bone> pBone = previousAnim->FindBone(nodeName);
	if (pBone)
	{
		pBone->Update(previousAnimTime);
		pNodeTransform = pBone->GetLocalTransform();
	}

	Ref<Bone> nBone = nextAnim->FindBone(nodeName);
	if (nBone)
	{
		nBone->Update(nextAnimTime);
		nNodeTransform = nBone->GetLocalTransform();
	}

	glm::quat rot0 = glm::quat_cast(pNodeTransform);
	glm::quat rot1 = glm::quat_cast(nNodeTransform);
	glm::quat finalRot = glm::slerp(rot0, rot1, blendValue);
	glm::mat4 blendedMat = glm::mat4_cast(finalRot);
	blendedMat[3] = (1.0f - blendValue) * pNodeTransform[3] + nNodeTransform[3] * blendValue;

	glm::mat4 globalTransform = parentTransform * blendedMat;

	Ref<BoneMap> mappedBone = m_SkeletalMeshComponent->FindBoneInRig(nodeName);
	if (mappedBone)
	{
		m_FinalBoneMatrices[mappedBone->ID] = globalTransform * mappedBone->OffsetMatrix;
	}

	for (size_t i = 0; i < pNode->children.size(); ++i)
	{
		CalculateBlendedBoneTransform(previousAnim, &pNode->children[i], nextAnim, &nNode->children[i], previousAnimTime, nextAnimTime, globalTransform, blendValue);
	}
}

bool Animator::GetBoolParameter(std::string parameterName)
{
	return m_BoolParameters.find(parameterName)->second;
}

float Animator::GetFloatParameter(std::string parameterName)
{
	return m_FloatParameters.find(parameterName)->second;
}

void Animator::SetFloatParameter(std::string parameterName, float value)
{
	auto param = m_FloatParameters.find(parameterName);
	if (param != m_FloatParameters.end())
		param->second = value;
	else
		std::cout << parameterName << " not found!" << std::endl;
}

