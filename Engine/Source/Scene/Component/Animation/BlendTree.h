#pragma once

#include "Core.h"
#include "AnimatorStateBase.h"

class Animation;

struct BlendNode
{
	Ref<Animation> Animation;
	float AnimationTime = 0.0f;
	float AnimationSpeed = 1.0f;
	float BlendLimit = 0.0f;
};

class BlendTree : public AnimatorStateBase
{
public:
	BlendTree();
	BlendTree(Animator* owner);

	static Ref<BlendTree> Create(Animator* owner);

	virtual void UpdateState(float deltaTime) override;

	void AddNode(const BlendNode& node);

	std::vector<BlendNode> GetNodes() const { return m_Nodes; }
	void SetBlendParemeterName(const std::string& name) { m_BlendParameterName = name; }

private:
	std::vector<BlendNode> m_Nodes;
	std::string m_BlendParameterName;
};