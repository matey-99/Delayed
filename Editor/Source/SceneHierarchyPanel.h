#pragma once

#include "Scene/Scene.h"
#include "imgui.h"

class Editor;

class SceneHierarchyPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;
	Ref<Actor> m_SelectedActor;

public:
	SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene);
	void Render();

	void DuplicateSelectedActor();
	void DeleteSelectedActor();

	void SelectActor(Ref<Actor> actor);
	void UnselectActor();

	inline Ref<Actor> GetSelectedActor() const { return m_SelectedActor; }

private:
	void TreeChildren(Ref<Actor> actor);
};