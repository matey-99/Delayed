#pragma once

#include "Scene/Scene.h"
#include "imgui.h"

class Editor;

class WorldSettingsPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;

public:
	WorldSettingsPanel(Ref<Editor> editor, Ref<Scene> scene);
	void Render();
};