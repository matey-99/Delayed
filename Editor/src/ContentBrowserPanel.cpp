#include "ContentBrowserPanel.h"

#include "imgui.h"
#include "Editor.h"
#include "Scene/Scene.h"
#include "Material/Material.h"
#include "Material/MaterialSerializer.h"
#include "Importer/MaterialImporter.h"

ContentBrowserPanel::ContentBrowserPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SupportedFileFormats.push_back("obj");
	m_SupportedFileFormats.push_back("3ds");
	m_SupportedFileFormats.push_back("fbx");
	m_SupportedFileFormats.push_back("mat");

	m_ResourcesDirectory = "../../../Assets";
	m_DisplayedDirectory = m_ResourcesDirectory;
}

void ContentBrowserPanel::Render()
{
	ImGui::Begin("Content Browser");

	if (m_DisplayedDirectory != m_ResourcesDirectory)
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
		int count = 0;
		for (auto& p : std::filesystem::directory_iterator(m_DisplayedDirectory))
		{
			if (p.is_directory())
				continue;

			std::stringstream ss;
			ss << p.path();
			std::string path = ss.str();
			CorrectPath(path);
			std::string filename = path.substr(path.find_last_of("/") + 1);

			if (filename.substr(0, 12) == "New material")
				count++;
		}
		Material::Create("New material " + std::to_string(count), "Standard");
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
		std::string entityName = path.substr(path.find_last_of('/') + 1, path.find_last_of('.') - (path.find_last_of('/') + 1));

		unsigned int countSameName = 0;
		for (auto entity : m_Scene->GetEntities())
		{
			if (entity->GetName().substr(0, entity->GetName().find_last_of(" ")) == entityName)
				countSameName++;
		}
		if (countSameName > 0)
		{
			entityName += " (" + std::to_string(countSameName) + ")";
		}

		m_Scene->AddEntity(path, entityName, m_Scene->FindEntity(0));
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
	}
}
