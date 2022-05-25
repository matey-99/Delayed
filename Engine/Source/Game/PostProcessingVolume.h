#pragma once

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Renderer/RenderPass/PostProcessingPass.h"

class PostProcessingVolume : public GameComponent 
{
public:
    PostProcessingVolume(Actor *owner);

	virtual void Start() override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;
	virtual void OnTriggerExit(ColliderComponent* other) override;

private:
	void ReplacePostProcessingSettings();

private:
	PostProcessingPass::PostProcessingSettings m_Settings;

    friend class SceneSerializer;
    friend class ActorDetailsPanel;
};

