#pragma once

#include "Scene/Component/TransformBaseComponent.h"

#include <glm/glm.hpp>

enum class AnchorType
{
	Center, Top, Bottom, Left, Right
};

class RectTransformComponent : public TransformBaseComponent
{
public:
	RectTransformComponent(Actor* owner);

	inline AnchorType GetAnchorType() const { return m_AnchorType; }
	inline glm::vec2 GetAnchor() const { return m_Anchor; }

	void SetAnchor(AnchorType type);

	virtual void CalculateLocalModelMatrix() override;

private:
	AnchorType m_AnchorType;
	glm::vec2 m_Anchor;

	friend class SceneSerializer;
};