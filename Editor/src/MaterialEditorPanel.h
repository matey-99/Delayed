#pragma once

#include "Material/Material.h"

class Editor;

class MaterialEditorPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Material> m_Material;

public:
	MaterialEditorPanel(Ref<Editor> editor);
	void Render();

	inline void SetMaterial(Ref<Material> material) { m_Material = material; }

private:
	void DisplayTextures(std::string name);
	void CorrectPath(std::string& path);
};