#pragma once

#include "Scene/Scene.h"

class Editor;

class ContentBrowserPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;
	std::vector<std::string> m_SupportedFileFormats;
	std::string m_DisplayedDirectory;
	bool m_IsCreateMaterialWindowDisplayed;
	std::string m_NewMaterialName;

public:
	ContentBrowserPanel(Ref<Editor> editor, Ref<Scene> scene);
	void Render();

private:
	//void CreateFile(std::string filename);

	void MakeAction(std::string path, std::string extension);
	void CorrectPath(std::string& path);
};