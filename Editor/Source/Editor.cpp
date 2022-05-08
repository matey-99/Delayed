#include "Editor.h"

#include <glm/gtc/type_ptr.hpp>

#include "Camera/CameraManager.h"
#include "Scene/Component/Light/Light.h"
#include "Renderer/RenderPass/GBufferPass.h"
#include "Renderer/RenderPass/LightingPass.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Animation/SkeletalMeshComponent.h"
#include "Scene/Component/Animation/Animator.h"
#include "Scene/Component/LODGroupComponent.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"
#include "Scene/Component/UI/UIComponent.h"
#include "Input/Input.h"

#define UPDATE_UI 1

Ref<Editor> Editor::s_Instance{};
std::mutex Editor::s_Mutex;

Editor::Editor()
{
	m_Scene = Ref<Scene>();
	m_Camera = Ref<EditorCamera>();

	m_SceneHierarchyPanel = Ref<SceneHierarchyPanel>();
	m_ActorDetailsPanel = Ref<ActorDetailsPanel>();
	m_ContentBrowserPanel = Ref<ContentBrowserPanel>();
	m_MaterialEditorPanel = Ref<MaterialEditorPanel>();
	m_WorldSettingsPanel = Ref<WorldSettingsPanel>();
	m_DebugPanel = Ref<DebugPanel>();
	m_Viewport = Ref<Viewport>();
	m_CameraComponentViewport = Ref<CameraComponentViewport>();

	m_DetailsPanel = false;
	m_IsCameraComponentViewport = false;
	m_MaterialEditor = false;

	m_PlayMode = false;
}


Editor::~Editor()
{
}

Ref<Editor> Editor::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<Editor>();

	return s_Instance;
}

void Editor::Initialize(Ref<Scene> scene)
{
	m_Scene = scene;
	m_Camera = CreateRef<EditorCamera>(m_Scene.get());

	m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(GetReference(), scene);
	m_ActorDetailsPanel = CreateRef<ActorDetailsPanel>(GetReference());
	m_ContentBrowserPanel = CreateRef<ContentBrowserPanel>(GetReference(), scene);
	m_MaterialEditorPanel = CreateRef<MaterialEditorPanel>(GetReference());
	m_RendererSettingsPanel = CreateRef<RendererSettingsPanel>(GetReference(), Renderer::GetInstance());
	m_WorldSettingsPanel = CreateRef<WorldSettingsPanel>(GetReference(), scene);
	m_DebugPanel = CreateRef<DebugPanel>(GetReference());
	m_Viewport = CreateRef<Viewport>(GetReference(), scene);
	m_CameraComponentViewport = CreateRef<CameraComponentViewport>(GetReference(), scene);

	m_Camera->Position = glm::vec3(-19.5f, 8.5f, -0.5f);
	m_Camera->Pitch = -26.0f;
	m_Camera->Yaw = 0.25f;
}

void Editor::Start()
{
	auto meshes = m_Scene->GetComponents<StaticMeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->Start();
	}

	auto skelMeshes = m_Scene->GetComponents<SkeletalMeshComponent>();
	for (auto skelMesh : skelMeshes)
	{
		skelMesh->Start();
	}
	
	auto lods = m_Scene->GetComponents<LODGroupComponent>();
	for (auto lod : lods)
	{
		lod->Start();
	}

	auto colliders = m_Scene->GetComponents<ColliderComponent>();
	for (auto collider : colliders)
	{
		collider->Start();
	}

	Input::GetInstance()->SetInputMode(InputMode::UI);
}

void Editor::Update(float deltaTime)
{
	m_Camera->Update();

	auto meshes = m_Scene->GetComponents<StaticMeshComponent>();
	for (auto mesh : meshes)
	{
		mesh->Update(deltaTime);
	}

	auto animators = m_Scene->GetComponents<Animator>();
	for (auto animator : animators)
	{
		animator->Update(deltaTime);
	}

	auto lods = m_Scene->GetComponents<LODGroupComponent>();
	for (auto lod : lods)
	{
		lod->Update(deltaTime);
	}

	auto colliders = m_Scene->GetComponents<ColliderComponent>();
	for (auto collider : colliders)
	{
		collider->Update(deltaTime);
	}

	auto particleSystems = m_Scene->GetComponents<ParticleSystemComponent>();
	for (auto particleSystem : particleSystems)
	{
		particleSystem->Update(deltaTime);
	}

	if (m_IsCameraComponentViewport)
	{
		if (m_SelectedCameraComponent)
			m_SelectedCameraComponent->Update(deltaTime);
	}

#if UPDATE_UI

	auto uiComponents = m_Scene->GetComponents<UIComponent>();
	for (auto comp : uiComponents)
		comp->Update(deltaTime);


#endif

}

void Editor::Render()
{
	m_SceneHierarchyPanel->Render();
	m_RendererSettingsPanel->Render();
	m_WorldSettingsPanel->Render();
	m_DebugPanel->Render();

	if (m_DetailsPanel)
	{
		m_ActorDetailsPanel->Render();
	}
	if (m_MaterialEditor)
	{
		m_MaterialEditorPanel->Render();
	}

	m_ContentBrowserPanel->Render();

	m_Viewport->Render();

	if (m_IsCameraComponentViewport)
		m_CameraComponentViewport->Render(m_SelectedCameraComponent);

}

void Editor::RenderScene()
{
	auto camera = CameraManager::GetInstance()->GetMainCamera();
	Renderer::GetInstance()->Render(m_Scene, camera);
}

void Editor::ShowDetails(Ref<Actor> actor)
{
	m_ActorDetailsPanel->SetActor(actor);
	m_DetailsPanel = true;

	if (m_SelectedCameraComponent = actor->GetComponent<CameraComponent>())
		m_IsCameraComponentViewport = true;
	else
	{
		if (!m_CameraComponentViewport->IsLocked())
			m_IsCameraComponentViewport = false;
	}


}

void Editor::HideDetails()
{
	m_DetailsPanel = false;
	m_IsCameraComponentViewport = false;
	m_SelectedCameraComponent = Ref<CameraComponent>();
}

void Editor::ShowMaterialEditor(Ref<Material> material)
{
	m_MaterialEditorPanel->SetMaterial(material);
	m_MaterialEditor = true;
}

void Editor::HideMaterialEditor()
{
	m_MaterialEditor = false;
}

void Editor::ManagePlayMode()
{
	if (m_PlayMode)
		m_PlayMode = false;
	else
		m_PlayMode = true;
}

Ref<Editor> Editor::GetReference()
{
	return shared_from_this();
}
