#include "Editor.h"

#include <glm/gtc/type_ptr.hpp>

#include "Camera/CameraManager.h"
#include "Scene/Component/Light/Light.h"

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
}

void Editor::Update(float deltaTime)
{
	m_Camera->Update();

	if (m_IsCameraComponentViewport)
	{
		if (m_SelectedCameraComponent)
			m_SelectedCameraComponent->Update(deltaTime);
	}
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

	if (Renderer::GetInstance()->IsPostProcessing())
		m_Viewport->Render(Renderer::GetInstance()->GetPostProcessingFramebuffer());
	else
		m_Viewport->Render(Renderer::GetInstance()->GetMainSceneFramebuffer());

	if (m_IsCameraComponentViewport)
	{
		if (Renderer::GetInstance()->IsPostProcessing())
			m_CameraComponentViewport->Render(Renderer::GetInstance()->GetPostProcessingFramebuffer(), m_SelectedCameraComponent);
		else
			m_CameraComponentViewport->Render(Renderer::GetInstance()->GetMainSceneFramebuffer(), m_SelectedCameraComponent);
	}
}

void Editor::RenderScene()
{
	m_Scene->PreRender();

	auto renderer = Renderer::GetInstance();
	renderer->GetMainSceneFramebuffer()->Bind();

	glClearColor(m_Scene->GetBackgroundColor()->x, m_Scene->GetBackgroundColor()->y, m_Scene->GetBackgroundColor()->z, m_Scene->GetBackgroundColor()->w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto camera = CameraManager::GetInstance()->GetMainCamera();

	renderer->GetCameraVertexUniformBuffer()->Bind();
	renderer->GetCameraVertexUniformBuffer()->SetUniform(0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjectionMatrix()));
	renderer->GetCameraVertexUniformBuffer()->SetUniform(GLSL_MAT4_SIZE, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
	renderer->GetCameraVertexUniformBuffer()->SetUniform(GLSL_MAT4_SIZE * 2, sizeof(glm::mat4), glm::value_ptr(camera->GetProjectionMatrix()));
	renderer->GetCameraVertexUniformBuffer()->Unbind();

	renderer->GetCameraFragmentUniformBuffer()->Bind();
	renderer->GetCameraFragmentUniformBuffer()->SetUniform(0, sizeof(glm::vec3), glm::value_ptr(camera->GetWorldPosition()));
	renderer->GetCameraFragmentUniformBuffer()->Unbind();

	m_Scene->Render();

	renderer->GetMainSceneFramebuffer()->Unbind();
}

void Editor::ShowDetails(Ref<Actor> actor)
{
	m_ActorDetailsPanel->SetActor(actor);
	m_DetailsPanel = true;

	if (m_SelectedCameraComponent = actor->GetComponent<CameraComponent>())
		m_IsCameraComponentViewport = true;
	else
		m_IsCameraComponentViewport = false;
}

void Editor::HideDetails()
{
	m_DetailsPanel = false;
	m_CameraComponentViewport = false;
	m_SelectedCameraComponent = nullptr;
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
