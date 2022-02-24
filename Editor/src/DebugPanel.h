#pragma once

#include "Scene/Scene.h"
#include "imgui.h"

class Editor;

class DebugPanel
{
private:
	Ref<Editor> m_Editor;

public:
	DebugPanel(Ref<Editor> editor);
	void Render();
};