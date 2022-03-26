#pragma once

#include "UIComponent.h"
#include "Renderer/Texture.h"

class ImageComponent : public UIComponent
{
public:
	ImageComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Destroy() override;

private:
	Ref<Texture> m_Image;
	glm::vec4 m_Color;

	friend class ActorDetailsPanel;
};