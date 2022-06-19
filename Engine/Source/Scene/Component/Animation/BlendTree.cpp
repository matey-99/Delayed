#include "BlendTree.h"

#include "Animator.h"

BlendTree::BlendTree()
	: AnimatorStateBase() 
{
}

BlendTree::BlendTree(Animator* owner)
	: AnimatorStateBase(owner) 
{
}

Ref<BlendTree> BlendTree::Create(Animator* owner)
{
	return CreateRef<BlendTree>(owner);
}

void BlendTree::UpdateState(float deltaTime)
{
	AnimatorStateBase::UpdateState(deltaTime);

	float blendValue = m_Owner->GetFloatParameter(m_BlendParameterName);
	BlendNode* previousNode;
	BlendNode* nextNode;

	if (m_Nodes.empty())
	{
		return;
	}
	else if (m_Nodes.size() == 1)
	{
		previousNode = &m_Nodes[0];
		nextNode = previousNode;
	}
	else
	{
		for (int i = 0; i < m_Nodes.size(); i++)
		{
			int next = i + 1;
			if (next < m_Nodes.size())
			{
				if (blendValue >= m_Nodes[i].BlendLimit && blendValue < m_Nodes[next].BlendLimit)
				{
					previousNode = &m_Nodes[i];
					nextNode = &m_Nodes[next];
					break;
				}
			}
			else
			{
				previousNode = &m_Nodes[i - 1];
				nextNode = &m_Nodes[i];
			}
		}
	}

	float blendRange = nextNode->BlendLimit - previousNode->BlendLimit;
	Ref<Animation> previousAnim = previousNode->Animation;
	Ref<Animation> nextAnim = nextNode->Animation;

	float pAnimMul = previousNode->AnimationSpeed;
	float nAnimMul = previousAnim->GetDuration() / nextAnim->GetDuration();
	float speedMultiplierUp = (1.0f - blendValue) * pAnimMul + nAnimMul * blendValue;

	pAnimMul = nextAnim->GetDuration() / previousAnim->GetDuration();
	nAnimMul = nextNode->AnimationSpeed;
	float speedMultiplierDown = (1.0f - blendValue) * pAnimMul + nAnimMul * blendValue;

	previousNode->AnimationTime += previousAnim->GetTicksPerSecond() * deltaTime * speedMultiplierUp;
	previousNode->AnimationTime = fmod(previousNode->AnimationTime, previousAnim->GetDuration());

	nextNode->AnimationTime += nextAnim->GetTicksPerSecond() * deltaTime * speedMultiplierDown;
	nextNode->AnimationTime = fmod(nextNode->AnimationTime, nextAnim->GetDuration());

	m_Owner->CalculateBlendedBoneTransform(
		previousAnim, &previousAnim->GetRootNode(),
		nextAnim, &nextAnim->GetRootNode(),
		previousNode->AnimationTime, nextNode->AnimationTime,
		glm::mat4(1.0f), blendValue);
}

void BlendTree::AddNode(const BlendNode& node)
{
	m_Nodes.push_back(node);
}
