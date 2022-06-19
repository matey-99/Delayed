#include "MaterialEditorPanel.h"

#include "Assets/AssetManager.h"
#include "imgui.h"
#include "Editor.h"
#include "Material/MaterialSerializer.h"

MaterialEditorPanel::MaterialEditorPanel(Ref<Editor> editor) : m_Editor(editor)
{
	m_Material = Ref<Material>();
}

void MaterialEditorPanel::Render()
{
	ImGui::Begin("Material Editor");

    //size_t maxSize = 128;
    //char* name = (char*)m_Material->m_Name.c_str();
    //ImGui::InputText("##Name", name, maxSize);
    //m_Material->m_Name = name;
    ImGui::Text(m_Material->m_Name.c_str());
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    const char* blendModeName = "";
    switch (m_Material->GetBlendMode())
    {
    case Material::BlendMode::Opaque:
        blendModeName = "Opaque";
        break;
    case Material::BlendMode::Transparent:
        blendModeName = "Transparent";
        break;
    }

    if (ImGui::BeginCombo("Blend Mode", blendModeName))
    {
        if (ImGui::Selectable("Opaque"))
            m_Material->m_BlendMode = Material::BlendMode::Opaque;
        if (ImGui::Selectable("Transparent"))
            m_Material->m_BlendMode = Material::BlendMode::Transparent;

        ImGui::EndCombo();
    }

    std::string shaderName = m_Material->m_Shader->GetName();
    if (ImGui::BeginCombo("Shader", shaderName.c_str()))
    {
        std::vector<std::string> extensions = std::vector<std::string>();
        extensions.push_back("frag");

        auto sl = ShaderLibrary::GetInstance();
        for (auto sh : sl->GetMaterialShaders())
        {
            if (ImGui::Selectable(sh.first.c_str()))
            {
                m_Material->m_Shader = sh.second;
                m_Material->LoadParameters();

            }
        }
        ImGui::EndCombo();
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    for (auto& param : m_Material->m_BoolParameters)
    {
        std::string name = param.first.substr(param.first.find_first_of('.') + 1);

        ImGui::Checkbox(name.c_str(), &param.second);
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    int index = 0;
    for (auto& param : m_Material->m_Texture2DParameters)
    {
        std::string name = param.first.substr(param.first.find_first_of('.') + 1);
        std::string filename = "NULL" + std::to_string(index);
        index++;
        if (param.second)
        {
            filename = param.second->GetPath().substr(param.second->GetPath().find_last_of('/'));
        }
        ImGui::PushID(index - 1);
        if (ImGui::BeginCombo(name.c_str(), filename.c_str()))
        {
            DisplayTextures(param.first);

            ImGui::EndMenu();
        }
        ImGui::PopID();
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    for (auto& param : m_Material->m_Vec3Parameters)
    {
        std::string name = param.first.substr(param.first.find_first_of('.') + 1);

        ImGui::ColorEdit3(name.c_str(), (float*)&param.second, ImGuiColorEditFlags_Float);
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    for (auto& param : m_Material->m_Vec4Parameters)
    {
        std::string name = param.first.substr(param.first.find_first_of('.') + 1);

        if (name.find("Color") != std::string::npos)
        {
            ImGui::ColorEdit4(name.c_str(), (float*)&param.second, ImGuiColorEditFlags_Float);
        }
        else
        {
            ImGui::DragFloat4(name.c_str(), (float*)&param.second, 0.1f);
        }
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    for (auto& param : m_Material->m_FloatParameters)
    {
        std::string name = param.first.substr(param.first.find_first_of('.') + 1);

        ImGui::DragFloat(name.c_str(), &param.second, 0.01f);
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (ImGui::Button("Recompile Shader"))
    {
        m_Material->GetShader()->Recompile();
        m_Material->LoadParameters();
    }

    if (ImGui::Button("Save"))
        AssetManager::SaveMaterial(m_Material);

    if (ImGui::Button("Close"))
        m_Editor->HideMaterialEditor();

	ImGui::End();
}

void MaterialEditorPanel::DisplayTextures(std::string name)
{
    for (auto& p : std::filesystem::recursive_directory_iterator(AssetManager::ContentDirectory))
    {
        std::stringstream ss;
        ss << p.path();
        std::string path = ss.str();
        CorrectPath(path);
        path = path.substr(AssetManager::ContentDirectory.size());

        std::string filename = path.substr(path.find_last_of("/") + 1);
        std::string shaderName = filename.substr(0, filename.find_last_of("."));
        std::string ext = filename.substr(filename.find_first_of('.') + 1);

        std::vector<std::string> extensions = { "jpg", "png", "exr" };

        for (auto extension : extensions)
        {
            if (extension == ext)
            {
                const bool isSelected = false;
                if (ImGui::Selectable(shaderName.c_str(), &isSelected))
                {
                    Texture::Type type = Texture::Type::BaseColor;
                    if (name == "u_Material.normalMap")
                        type = Texture::Type::Normal;
                    else if (name == "u_Material.roughnessMap")
                        type = Texture::Type::Roughness;
                    else if (name == "u_Material.metallicMap")
                        type = Texture::Type::Metallic;
                    else if (name == "u_Material.aoMap")
                        type = Texture::Type::AO;
                    else if (name == "u_Material.emissiveMap")
                        type = Texture::Type::Emissive;
                    else if (name == "u_Material.opacityMap")
                        type = Texture::Type::Opacity;

                    auto texture = AssetManager::LoadTexture(path, type);
                    m_Material->m_Texture2DParameters.find(name)->second = texture;
                }
            }
        }
    }
}

void MaterialEditorPanel::CorrectPath(std::string& path)
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
