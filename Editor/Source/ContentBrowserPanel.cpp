#include "ContentBrowserPanel.h"

#include "imgui.h"
#include "Editor.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Material/Material.h"
#include "Material/MaterialSerializer.h"
#include "Importer/MaterialImporter.h"
#include "Assets/AssetManager.h"

ContentBrowserPanel::ContentBrowserPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SupportedFileFormats.push_back("obj");
	m_SupportedFileFormats.push_back("3ds");
	m_SupportedFileFormats.push_back("fbx");
	m_SupportedFileFormats.push_back("mat");

	m_DisplayedDirectory = AssetManager::ContentDirectory;

	m_IsCreateMaterialWindowDisplayed = false;
	m_NewMaterialName = "New Material";
}

void ContentBrowserPanel::Render()
{
	ImGui::Begin("Content Browser");

	if (m_DisplayedDirectory != AssetManager::ContentDirectory)
	{
		bool selected = false;
		ImGui::Selectable("..", &selected);
		if (selected)
		{
			m_DisplayedDirectory = m_DisplayedDirectory.substr(0, m_DisplayedDirectory.find_last_of("/"));
		}
	}

	for (auto& p : std::filesystem::directory_iterator(m_DisplayedDirectory))
	{
		std::stringstream ss;
		ss << p.path();
		std::string path = ss.str();
		CorrectPath(path);
		std::string name = path.substr(path.find_last_of("/") + 1);

		bool selected = false;
		ImGui::Selectable(name.c_str(), &selected);
		if (selected)
		{
			if (p.is_directory())
			{
				m_DisplayedDirectory += "/" + name;
			}
			else
			{
				std::string extension = name.substr(name.find_first_of('.') + 1);
				MakeAction(path, extension);
			}
		}
	}

	bool createMaterial = false;
	if (ImGui::BeginPopupContextWindow())
	{
		ImGui::MenuItem("New Material", "", &createMaterial);

		ImGui::EndPopup();
	}

	if (createMaterial)
	{
		m_IsCreateMaterialWindowDisplayed = true;
	}

	if (m_IsCreateMaterialWindowDisplayed)
	{
		ImGui::Begin("Create Material");

		size_t maxSize = 128;
		char* name = (char*)m_NewMaterialName.c_str();
		ImGui::InputText("##Name", name, maxSize);
		m_NewMaterialName = name;
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if (ImGui::Button("Save"))
		{
			m_IsCreateMaterialWindowDisplayed = false;
			Material::Create(m_NewMaterialName, "Standard");
		}
		if (ImGui::Button("Exit"))
		{
			m_IsCreateMaterialWindowDisplayed = false;
		}
		ImGui::End();
	}

	ImGui::End();
}

void ContentBrowserPanel::MakeAction(std::string path, std::string extension)
{
	if (extension == "mat")
	{
		Ref<Material> material = MaterialImporter::GetInstance()->ImportMaterial(path);
		m_Editor->ShowMaterialEditor(material);
	}
	else if (extension == "obj" || extension == "fbx" || extension == "3ds" || extension == "dae")
	{
		std::string actorName = path.substr(path.find_last_of('/') + 1, path.find_last_of('.') - (path.find_last_of('/') + 1));

		unsigned int countSameName = 0;
		for (auto actor : m_Scene->GetActors())
		{
			if (actor->GetName().substr(0, actor->GetName().find_last_of(" ")) == actorName)
				countSameName++;
		}
		if (countSameName > 0)
		{
			actorName += " (" + std::to_string(countSameName) + ")";
		}

		m_Scene->AddActor(path, actorName, m_Scene->FindActor(0));
	}
	else if (extension == "scene")
	{
		SceneManager::GetInstance()->LoadScene(path);
		m_Editor->Initialize(SceneManager::GetInstance()->GetCurrentScene());
	}
}

void ContentBrowserPanel::CorrectPath(std::string& path)
{
	path = path.substr(path.find_first_of('"') + 1, path.length() - 2);

	std::size_t index = 0;
	while (true)
	{
		index = path.find("\\\\", index);
		if (index == std::string::npos)
			break;

		path.replace(index, 2, "/");
		path = path.substr(1);
	}

	path = path.substr(AssetManager::ContentDirectory.size());
}
