#pragma once

#include "Scene/Component/TransformBaseComponent.h"

#include <glm/glm.hpp>

enum class AnchorType
{
	Center, Top, Bottom, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight
};

class RectTransformComponent : public TransformBaseComponent
{
public:
	RectTransformComponent(Actor* owner);

	inline AnchorType GetAnchorType() const { return m_AnchorType; }
	inline glm::vec2 GetAnchor() const { return m_Anchor; }

	inline glm::vec3 GetLocalPositionUI() const { return m_LocalPositionUI; }
	inline glm::vec3 GetLocalScaleUI() const { return m_LocalScaleUI; }

	void SetAnchor(AnchorType type);

	virtual void CalculateLocalModelMatrix() override;

private:
	AnchorType m_AnchorType;
	glm::vec2 m_Anchor;

	glm::vec3 m_LocalPositionUI;
	glm::vec3 m_LocalScaleUI;

	friend class SceneSerializer;
};