#include "PostProcessingVolume.h"

#include "Scene/Actor.h"
#include "Player.h"

PostProcessingVolume::PostProcessingVolume(Actor* owner)
	: GameComponent(owner)
{
	m_Settings = PostProcessingPass::PostProcessingSettings();
}

void PostProcessingVolume::Start()
{
	if (auto collider = m_Owner->GetComponent<ColliderComponent>())
	{
		collider->OnTriggerEnterDelegate.Add(&PostProcessingVolume::OnTriggerEnter, this);
		collider->OnTriggerExitDelegate.Add(&PostProcessingVolume::OnTriggerExit, this);
	}
}

void PostProcessingVolume::OnTriggerEnter(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>())
		ReplacePostProcessingSettings();
}

void PostProcessingVolume::OnTriggerExit(ColliderComponent* other)
{
	if (other->GetOwner()->GetComponent<Player>())
		ReplacePostProcessingSettings();
}

void PostProcessingVolume::ReplacePostProcessingSettings()
{
	auto& postProcessingPass = Renderer::GetInstance()->m_PostProcessingPass;
	auto temp = postProcessingPass->GetSettings();

	postProcessingPass->SetSettings(m_Settings);
	m_Settings = temp;
}
