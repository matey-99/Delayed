#pragma once

#include "Renderer/Renderer.h"
#include "imgui.h"

class Editor;

class RendererSettingsPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Renderer> m_Renderer;

public:
	RendererSettingsPanel(Ref<Editor> editor, Ref<Renderer> renderer);
	void Render();
};