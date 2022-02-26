#pragma once

#include "typedefs.h"
#include "Renderer/Renderer.h"
#include "SceneHierarchyPanel.h"
#include "EntityDetailsPanel.h"
#include "ContentBrowserPanel.h"
#include "MaterialEditorPanel.h"
#include "RendererSettingsPanel.h"
#include "WorldSettingsPanel.h"
#include "DebugPanel.h"
#include "Viewport.h"

#include <ImGuizmo.h>
#include <mutex>

class Editor : public std::enable_shared_from_this<Editor>
{
private:
	static Ref<Editor> s_Instance;
	static std::mutex s_Mutex;

	Ref<Scene> m_Scene;

	Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	Ref<EntityDetailsPanel> m_EntityDetailsPanel;
	Ref<ContentBrowserPanel> m_ContentBrowserPanel;
	Ref<MaterialEditorPanel> m_MaterialEditorPanel;
	Ref<RendererSettingsPanel> m_RendererSettingsPanel;
	Ref<WorldSettingsPanel> m_WorldSettingsPanel;
	Ref<DebugPanel> m_DebugPanel;
	Ref<Viewport> m_Viewport;

	bool m_PlayMode;

	bool m_DetailsPanel;
	bool m_MaterialEditor;
	ImGuizmo::OPERATION m_Operation = ImGuizmo::OPERATION::TRANSLATE;

public:
	Editor();
	~Editor();

	Editor(Editor& other) = delete;
	void operator=(const Editor&) = delete;

	static Ref<Editor> GetInstance();

	void Initialize(Ref<Scene> scene);
	void Render();

	void ShowDetails(Ref<Entity> entity);
	void HideDetails();

	void ShowMaterialEditor(Ref<Material> material);
	void HideMaterialEditor();

	void ManagePlayMode();

	inline ImGuizmo::OPERATION GetGizmoOperation() const { return m_Operation; }
	inline void SetGizmoOperation(ImGuizmo::OPERATION operation) { m_Operation = operation; }

	inline Ref<Scene> GetScene() const { return m_Scene; }
	inline Ref<SceneHierarchyPanel> GetSceneHierarchyPanel() const { return m_SceneHierarchyPanel; }
	inline Ref<Viewport> GetViewport() const { return m_Viewport; }
	inline bool IsPlayMode() const { return m_PlayMode; }

private:
	Ref<Editor> GetReference();
};