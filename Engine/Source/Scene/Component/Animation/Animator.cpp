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
	m_CurrentState = nullptr;
}

void Animator::Start()
{
	m_SkeletalMeshComponent = m_Owner->GetComponent<SkeletalMeshComponent>();
	if (!m_SkeletalMeshComponent)
		m_SkeletalMeshComponent = m_Owner->AddComponent<SkeletalMeshComponent>();

	m_FloatParameters.insert({ "Speed", 0.0f });
	m_BoolParameters.insert({ "IsGrounded", true });
	m_BoolParameters.insert({ "IsLanding", true });
	m_BoolParameters.insert({ "IsSprinting", false });
	m_BoolParameters.insert({ "IsJumping", false });
	m_BoolParameters.insert({ "IsDoubleJumping", false });
	m_BoolParameters.insert({ "IsDashing", false });

	/* Movement Blend Tree */
	Ref<BlendTree> movementBlendTree = BlendTree::Create(this);
	movementBlendTree->SetName("Movement");

	BlendNode IdleNode;
	IdleNode.AnimationSpeed = 1.0f;
	IdleNode.BlendLimit = 0.0f;
	IdleNode.Animation = m_SkeletalMeshComponent->GetAnimation(3);
	IdleNode.AnimationID = 3;

	BlendNode WalkNode;
	WalkNode.AnimationSpeed = 1.0f;
	WalkNode.BlendLimit = 0.5f;
	WalkNode.Animation = m_SkeletalMeshComponent->GetAnimation(4);
	WalkNode.AnimationID = 4;

	BlendNode RunNode;
	RunNode.AnimationSpeed = 1.0f;
	RunNode.BlendLimit = 1.0f;
	RunNode.Animation = m_SkeletalMeshComponent->GetAnimation(5);
	RunNode.AnimationID = 5;

	movementBlendTree->SetBlendParemeterName("Speed");
	movementBlendTree->AddNode(IdleNode);
	movementBlendTree->AddNode(WalkNode);
	movementBlendTree->AddNode(RunNode);

	m_States.push_back(movementBlendTree);
	m_CurrentState = movementBlendTree;

	/* Jump States */
	Ref<AnimatorState> jumpState = AnimatorState::Create(this);
	jumpState->SetName("Jump");
	jumpState->SetAnimation(m_SkeletalMeshComponent->GetAnimation(8));
	jumpState->SetAnimationSpeed(1.0f);
	jumpState->SetAnimationID(8);
	m_States.push_back(jumpState);

	Ref<AnimatorState> fallState = AnimatorState::Create(this);
	fallState->SetName("Fall");
	fallState->SetAnimation(m_SkeletalMeshComponent->GetAnimation(11));
	fallState->SetAnimationSpeed(1.0f);
	fallState->SetAnimationID(11);
	m_States.push_back(fallState);

	Ref<AnimatorState> landState = AnimatorState::Create(this);
	landState->SetName("Land");
	landState->SetAnimation(m_SkeletalMeshComponent->GetAnimation(9));
	landState->SetAnimationSpeed(1.0f);
	landState->SetAnimationID(9);
	m_States.push_back(landState);

	/* Skills States */
	Ref<AnimatorState> sprintState = AnimatorState::Create(this);
	sprintState->SetName("Sprint");
	sprintState->SetAnimation(m_SkeletalMeshComponent->GetAnimation(6));
	sprintState->SetAnimationSpeed(1.0f);
	sprintState->SetAnimationID(6);
	m_States.push_back(sprintState);

	Ref<AnimatorState> doubleJumpState = AnimatorState::Create(this);
	doubleJumpState->SetName("DoubleJump");
	doubleJumpState->SetAnimation(m_SkeletalMeshComponent->GetAnimation(8));
	doubleJumpState->SetAnimationSpeed(1.0f);
	doubleJumpState->SetAnimationID(8);
	m_States.push_back(doubleJumpState);

	Ref<AnimatorState> dashState = AnimatorState::Create(this);
	dashState->SetName("Dash");
	dashState->SetAnimation(m_SkeletalMeshComponent->GetAnimation(0));
	dashState->SetAnimationSpeed(1.0f);
	dashState->SetAnimationID(0);
	m_States.push_back(dashState);

	/* Movement Blend Tree Transitions */
	Ref<AnimatorTransition> movementToJump = AnimatorTransition::Create(this, movementBlendTree, jumpState);
	movementToJump->AddCondition("IsJumping", true);

	Ref<AnimatorTransition> movementToFall = AnimatorTransition::Create(this, movementBlendTree, fallState);
	movementToFall->AddCondition("IsGrounded", false);

	Ref<AnimatorTransition> movementToDash = AnimatorTransition::Create(this, movementBlendTree, dashState);
	movementToDash->AddCondition("IsDashing", true);

	Ref<AnimatorTransition> movementToSprint = AnimatorTransition::Create(this, movementBlendTree, sprintState);
	movementToSprint->AddCondition("IsSprinting", true);
	movementToSprint->AddCondition("Speed", NumericalConditionType::Greater, 0.5f);

	movementBlendTree->AddExitTransition(movementToDash);
	movementBlendTree->AddExitTransition(movementToJump);
	movementBlendTree->AddExitTransition(movementToFall);
	movementBlendTree->AddExitTransition(movementToSprint);

	m_Transitions.push_back(movementToJump);
	m_Transitions.push_back(movementToFall);
	m_Transitions.push_back(movementToDash);
	m_Transitions.push_back(movementToSprint);

	/* Jump State Transitions */
	Ref<AnimatorTransition> jumpToFall = AnimatorTransition::Create(this, jumpState, fallState);
	jumpToFall->AddCondition("IsGrounded", false);
	jumpToFall->SetWaitUntilAnimationEnd(true);

	Ref<AnimatorTransition> jumpToLand = AnimatorTransition::Create(this, jumpState, landState);
	jumpToLand->AddCondition("IsLanding", true);

	Ref<AnimatorTransition> jumpToMovement = AnimatorTransition::Create(this, jumpState, movementBlendTree);
	jumpToMovement->SetWaitUntilAnimationEnd(true);

	Ref<AnimatorTransition> jumpToDoubleJump = AnimatorTransition::Create(this, jumpState, doubleJumpState);
	jumpToDoubleJump->AddCondition("IsDoubleJumping", true);

	Ref<AnimatorTransition> jumpToDash = AnimatorTransition::Create(this, jumpState, dashState);
	jumpToDash->AddCondition("IsDashing", true);
	jumpToDash->SetTransitionTime(0.05f);

	//jumpState->AddExitTransition(jumpToLand);
	//jumpState->AddExitTransition(jumpToFall);
	jumpState->AddExitTransition(jumpToDash);
	jumpState->AddExitTransition(jumpToMovement);
	jumpState->AddExitTransition(jumpToDoubleJump);

	m_Transitions.push_back(jumpToDash);
	m_Transitions.push_back(jumpToLand);
	m_Transitions.push_back(jumpToFall);
	m_Transitions.push_back(jumpToMovement);
	m_Transitions.push_back(jumpToDoubleJump);
	

	/* Fall State Transitions */
	Ref<AnimatorTransition> fallToMovement = AnimatorTransition::Create(this, fallState, movementBlendTree);
	fallToMovement->AddCondition("IsLanding", true);

	Ref<AnimatorTransition> fallToLand = AnimatorTransition::Create(this, fallState, landState);
	fallToLand->AddCondition("IsLanding", true);

	Ref<AnimatorTransition> fallToDoubleJump = AnimatorTransition::Create(this, fallState, doubleJumpState);
	fallToDoubleJump->AddCondition("IsDoubleJumping", true);

	Ref<AnimatorTransition> fallToDash = AnimatorTransition::Create(this, fallState, dashState);
	fallToDash->AddCondition("IsDashing", true);
	fallToDash->SetTransitionTime(0.05f);

	fallState->AddExitTransition(fallToDash);
	fallState->AddExitTransition(fallToMovement);
	fallState->AddExitTransition(fallToDoubleJump);

	m_Transitions.push_back(fallToDash);
	m_Transitions.push_back(fallToLand);
	m_Transitions.push_back(fallToDoubleJump);

	/* Land State Transitions */
	Ref<AnimatorTransition> landToMovement = AnimatorTransition::Create(this, landState, movementBlendTree);
	landToMovement->AddCondition("IsGrounded", true);
	//landToMovement->SetWaitUntilAnimationEnd(true);

	Ref<AnimatorTransition> landToDoubleJump = AnimatorTransition::Create(this, landState, doubleJumpState);
	landToDoubleJump->AddCondition("IsDoubleJumping", true);

	Ref<AnimatorTransition> landToDash = AnimatorTransition::Create(this, landState, dashState);
	landToDash->AddCondition("IsDashing", true);

	landState->AddExitTransition(landToMovement);
	landState->AddExitTransition(landToDoubleJump);
	landState->AddExitTransition(landToDash);

	m_Transitions.push_back(landToMovement);
	m_Transitions.push_back(landToDoubleJump);
	m_Transitions.push_back(landToDash);

	/* Sprint State Transitions */
	Ref<AnimatorTransition> sprintToMovement = AnimatorTransition::Create(this, sprintState, movementBlendTree);
	sprintToMovement->AddCondition("IsSprinting", false);

	Ref<AnimatorTransition> sprintToJump = AnimatorTransition::Create(this, sprintState, jumpState);
	sprintToJump->AddCondition("IsJumping", true);

	Ref<AnimatorTransition> sprintToDash = AnimatorTransition::Create(this, sprintState, dashState);
	sprintToDash->AddCondition("IsDashing", true);

	sprintState->AddExitTransition(sprintToDash);
	sprintState->AddExitTransition(sprintToJump);
	sprintState->AddExitTransition(sprintToMovement);

	m_Transitions.push_back(sprintToMovement);
	m_Transitions.push_back(sprintToJump);
	m_Transitions.push_back(sprintToDash);

	/* Double Jump State Transitions */
	Ref<AnimatorTransition> doubleJumpToFall = AnimatorTransition::Create(this, doubleJumpState, fallState);

	Ref<AnimatorTransition> doubleJumpToDash = AnimatorTransition::Create(this, doubleJumpState, dashState);
	doubleJumpToDash->AddCondition("IsDashing", true);

	doubleJumpState->AddExitTransition(doubleJumpToFall);
	doubleJumpState->AddExitTransition(doubleJumpToDash);

	m_Transitions.push_back(doubleJumpToFall);
	m_Transitions.push_back(doubleJumpToDash);

	/* Dash State Transitions */
	Ref<AnimatorTransition> dashToFall = AnimatorTransition::Create(this, dashState, fallState);
	dashToFall->AddCondition("IsGrounded", false);
	dashToFall->AddCondition("IsDashing", false);

	Ref<AnimatorTransition> dashToSprint = AnimatorTransition::Create(this, dashState, sprintState);
	dashToSprint->AddCondition("IsSprinting", true);
	dashToSprint->AddCondition("Speed", NumericalConditionType::Greater, 0.5f);

	Ref<AnimatorTransition> dashToMovement = AnimatorTransition::Create(this, dashState, movementBlendTree);
	dashToMovement->AddCondition("IsDashing", false);

	dashState->AddExitTransition(dashToFall);
	dashState->AddExitTransition(dashToSprint);
	dashState->AddExitTransition(dashToMovement);

	m_Transitions.push_back(dashToFall);
	m_Transitions.push_back(dashToMovement);
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

void Animator::SetBoolParameter(std::string parameterName, bool value)
{
	auto param = m_BoolParameters.find(parameterName);
	if (param != m_BoolParameters.end())
		param->second = value;
	else
		std::cout << parameterName << " not found!" << std::endl;
}

void Animator::SetFloatParameter(std::string parameterName, float value)
{
	auto param = m_FloatParameters.find(parameterName);
	if (param != m_FloatParameters.end())
		param->second = value;
	else
		std::cout << parameterName << " not found!" << std::endl;
}

