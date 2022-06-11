#pragma once

#include "Scene/Scene.h"
#include "imgui.h"

class Editor;

class DebugPanel
{
private:
	Ref<Editor> m_Editor;
	uint32_t m_ShadowMaps[4];
	bool m_DisplayShadowMaps;

public:
	DebugPanel(Ref<Editor> editor);
	void Render();
};