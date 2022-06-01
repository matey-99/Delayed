#include "ActorDetailsPanel.h"

#include "imgui.h"
#include "Editor.h"
#include "Assets/AssetManager.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/FoliageComponent.h"
#include "Scene/Component/Animation/SkeletalMeshComponent.h"
#include "Scene/Component/LODGroupComponent.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Scene/Component/Light/SkyLight.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"
#include "Scene/Component/Particle/ParticleEmitterBox.h"
#include "Scene/Component/Particle/ParticleEmitterSphere.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Scene/Component/UI/TextComponent.h"
#include "Scene/Component/UI/RectTransformComponent.h"
#include "Scene/Component/Collider/BoxColliderComponent.h"
#include "Scene/Component/AudioSourceComponent.h"
#include "Scene/Component/AudioListenerComponent.h"

#include "Game/Player.h"
#include "Game/CharacterController.h"
#include "Game/CameraController.h"
#include "Game/Platform.h"
#include "Game/Ghost.h"
#include "Game/Button.h"
#include "Game/Checkpoint.h"
#include "Game/DeathArea.h"
#include "Game/Obelisks/Obelisk.h"
#include "Game/PostProcessingVolume.h"

#include <glm/gtc/type_ptr.hpp>
#include <Scene/Component/Collider/SphereColliderComponent.h>
#include <Scene/Component/RigidBodyComponent.h>


bool Equals(float arr[3], glm::vec3 vec)
{
    return arr[0] == vec.x &&
           arr[1] == vec.y &&
           arr[2] == vec.z;
}

bool Equals(int arr[3], glm::vec3 vec)
{
    return arr[0] == (int)vec.x &&
        arr[1] == (int)vec.y &&
        arr[2] == (int)vec.z;
}

ActorDetailsPanel::ActorDetailsPanel(Ref<Editor> editor) : m_Editor(editor)
{
	m_Actor = Ref<Actor>();
}

void ActorDetailsPanel::Render()
{
	ImGui::Begin("Details");

    int componentIndex = 0;

    size_t maxSize = 128;
    char* name = (char*)m_Actor->m_Name.c_str();
    ImGui::InputText("##Name", name, maxSize);
    m_Actor->m_Name = name;

    std::string idStr = std::to_string(m_Actor->m_ID);
    char* id = (char*)idStr.c_str();
    ImGui::InputText("ID", id, maxSize, ImGuiInputTextFlags_ReadOnly);

    ImGui::Checkbox("Dynamic", &m_Actor->m_Dynamic);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (auto transform = m_Actor->GetComponent<TransformComponent>())
    {
        ImGui::Text("Transform");
        float pos[3];
        pos[0] = transform->GetLocalPosition().x;
        pos[1] = transform->GetLocalPosition().y;
        pos[2] = transform->GetLocalPosition().z;

        float rot[3];
        rot[0] = transform->GetLocalRotation().x;
        rot[1] = transform->GetLocalRotation().y;
        rot[2] = transform->GetLocalRotation().z;

        float scl[3];
        scl[0] = transform->GetLocalScale().x;
        scl[1] = transform->GetLocalScale().y;
        scl[2] = transform->GetLocalScale().z;

        ImGui::DragFloat3("Position", pos, 0.5f);
        ImGui::DragFloat3("Rotation", rot, 1.0f);
        ImGui::DragFloat3("Scale", scl, 0.1f);

        if (!Equals(pos, transform->GetLocalPosition()))
            transform->SetLocalPosition({ pos[0], pos[1], pos[2] });

        if (!Equals(rot, transform->GetLocalRotation()))
            transform->SetLocalRotation({ rot[0], rot[1], rot[2] });    

        if (!Equals(scl, transform->GetLocalScale()))
            transform->SetLocalScale({ scl[0], scl[1], scl[2] });

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto transform = m_Actor->GetComponent<RectTransformComponent>())
    {
        ImGui::Text("Rect Transform");

        const char* anchorName = "";
        switch (transform->GetAnchorType())
        {
        case AnchorType::Center:
            anchorName = "Center";
            break;
        case AnchorType::Top:
            anchorName = "Top";
            break;
        case AnchorType::Bottom:
            anchorName = "Bottom";
            break;
        case AnchorType::Left:
            anchorName = "Left";
            break;
        case AnchorType::Right:
            anchorName = "Right";
            break;
        case AnchorType::TopLeft:
            anchorName = "Top Left";
            break;
        case AnchorType::TopRight:
            anchorName = "Top Right";
            break;
        case AnchorType::BottomLeft:
            anchorName = "Bottom Left";
            break;
        case AnchorType::BottomRight:
            anchorName = "Bottom Right";
            break;
        }

        if (ImGui::BeginCombo("Anchor", anchorName))
        {
            if (ImGui::Selectable("Center"))
                transform->SetAnchor(AnchorType::Center);
            if (ImGui::Selectable("Top"))
                transform->SetAnchor(AnchorType::Top);
            if (ImGui::Selectable("Bottom"))
                transform->SetAnchor(AnchorType::Bottom);
            if (ImGui::Selectable("Left"))
                transform->SetAnchor(AnchorType::Left);
            if (ImGui::Selectable("Right"))
                transform->SetAnchor(AnchorType::Right);
            if (ImGui::Selectable("TopLeft"))
                transform->SetAnchor(AnchorType::TopLeft);
            if (ImGui::Selectable("TopRight"))
                transform->SetAnchor(AnchorType::TopRight);
            if (ImGui::Selectable("BottomLeft"))
                transform->SetAnchor(AnchorType::BottomLeft);
            if (ImGui::Selectable("BottomRight"))
                transform->SetAnchor(AnchorType::BottomRight);

            ImGui::EndCombo();
        }

        int pos[3];
        pos[0] = transform->GetLocalPosition().x;
        pos[1] = transform->GetLocalPosition().y;
        pos[2] = transform->GetLocalPosition().z;

        float rot[3];
        rot[0] = transform->GetLocalRotation().x;
        rot[1] = transform->GetLocalRotation().y;
        rot[2] = transform->GetLocalRotation().z;

        float scl[3];
        scl[0] = transform->GetLocalScale().x;
        scl[1] = transform->GetLocalScale().y;
        scl[2] = transform->GetLocalScale().z;

        ImGui::DragInt3("Position", pos, 0.5f);
        ImGui::DragFloat3("Rotation", rot, 1.0f);
        ImGui::DragFloat3("Scale", scl, 0.1f);

        if (!Equals(pos, transform->GetLocalPosition()))
            transform->SetLocalPosition({ pos[0], pos[1], pos[2] });

        if (!Equals(rot, transform->GetLocalRotation()))
            transform->SetLocalRotation({ rot[0], rot[1], rot[2] });

        if (!Equals(scl, transform->GetLocalScale()))
            transform->SetLocalScale({ scl[0], scl[1], scl[2] });

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto camera = m_Actor->GetComponent<CameraComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Camera");

        ImGui::DragFloat2("Aspect Ratio", (float*)&camera->m_AspectRatio, 1.0f, 1.0f, 30.0f);
        ImGui::DragFloat("Field Of View", &camera->m_FieldOfView, 1.0f, 1.0f, 180.0f);
        ImGui::DragFloat("Near Clip Plane", &camera->m_NearClipPlane, 0.1f, 0.1f, 1000.0f);
        ImGui::DragFloat("Far Clip Plane", &camera->m_FarClipPlane, 0.1f, 0.1f, 1000.0f);

        ImGui::PopID();
        componentIndex++;
    }

    if (auto mesh = m_Actor->GetComponent<StaticMeshComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Static Mesh");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<StaticMeshComponent>();

        std::string path = mesh->GetModel()->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Static Mesh", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("obj");
            extensions.push_back("fbx");
            extensions.push_back("3ds");
            extensions.push_back("dae");
            DisplayResources(mesh, extensions);

            ImGui::EndCombo();
        }

        ImGui::Text("Materials");
        for (int i = 0; i < mesh->GetMaterials().size(); i++)
        {
            path = mesh->GetMaterialsPaths().at(i);
            name = path.substr(path.find_last_of("/") + 1);

            ImGui::PushID(i);
            if (ImGui::BeginCombo(("[" + std::to_string(i) + "]").c_str(), name.c_str()))
            {
                std::vector<std::string> extensions = std::vector<std::string>();
                extensions.push_back("mat");
                DisplayResources(mesh, extensions, i);

                ImGui::EndCombo();
            }
            ImGui::PopID();
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto mesh = m_Actor->GetComponent<SkeletalMeshComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Skeletal Mesh");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<SkeletalMeshComponent>();

        std::string path = mesh->GetModel()->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Skeletal Mesh", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("obj");
            extensions.push_back("fbx");
            extensions.push_back("3ds");
            extensions.push_back("dae");
            DisplayResources(mesh, extensions);

            ImGui::EndCombo();
        }

        ImGui::Text("Materials");
        for (int i = 0; i < mesh->GetMaterials().size(); i++)
        {
            path = mesh->GetMaterialsPaths().at(i);
            name = path.substr(path.find_last_of("/") + 1);

            ImGui::PushID(i);
            if (ImGui::BeginCombo(("[" + std::to_string(i) + "]").c_str(), name.c_str()))
            {
                std::vector<std::string> extensions = std::vector<std::string>();
                extensions.push_back("mat");
                DisplayResources(mesh, extensions, i);

                ImGui::EndCombo();
            }
            ImGui::PopID();
        }

        // Skeleton informations
        ImGui::Text("Skeleton informations");
        ImGui::Text("Bones: %i", mesh->GetBoneCount());

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto foliage = m_Actor->GetComponent<FoliageComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Foliage");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<FoliageComponent>();

        std::string path = foliage->GetModel()->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Model", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("obj");
            extensions.push_back("fbx");
            extensions.push_back("3ds");
            extensions.push_back("dae");
            DisplayResources(foliage, extensions);

            ImGui::EndCombo();
        }

        path = foliage->GetMaterialPath();
        name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Material", name.c_str()))
        {
            std::vector<std::string> extensions;
            extensions.push_back("mat");
            DisplayResources(foliage, extensions);

            ImGui::EndCombo();
        }

        int instancesCount = foliage->m_InstancesCount;
        ImGui::DragInt("Instances Count", &instancesCount, 1, 0, 1000);
        if (instancesCount != foliage->m_InstancesCount)
            foliage->SetInstancesCount(instancesCount);

        ImGui::DragFloat("Radius", &foliage->m_Radius, 0.1f, 0.0f, 1000.0f);
        ImGui::DragFloat("Min Instance Scale", &foliage->m_MinInstanceScale, 0.1f, 0.0f, 100.0f);
        ImGui::DragFloat("Max Instance Scale", &foliage->m_MaxInstanceScale, 0.1f, 0.0f, 100.0f);

        if (ImGui::Button("Generate"))
            foliage->Generate();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto lodGroup = m_Actor->GetComponent<LODGroupComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("LOD Group");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<LODGroupComponent>();

        ImGui::Text(("Current LOD: " + std::to_string(lodGroup->GetCurrentLOD().Level)).c_str());

        int i = 0;
        for (auto& lod : lodGroup->m_LODs)
        {
            ImGui::PushID(i);
            ImGui::Text(("LOD" + std::to_string(lod.Level)).c_str());
            std::string path = lod.Model->GetPath();
            std::string name = path.substr(path.find_last_of("/") + 1);
            if (ImGui::BeginCombo("Model", name.c_str()))
            {
                std::vector<std::string> extensions = std::vector<std::string>();
                extensions.push_back("obj");
                extensions.push_back("fbx");
                extensions.push_back("3ds");
                extensions.push_back("dae");
                DisplayResources(lodGroup, extensions, -1, i);

                ImGui::EndCombo();
            }

            ImGui::Text("Materials");
            for (int j = 0; j < lod.Materials.size(); j++)
            {
                path = lod.Materials[j]->GetPath();
                name = path.substr(path.find_last_of("/") + 1);

                ImGui::PushID(j);
                if (ImGui::BeginCombo(("[" + std::to_string(j) + "]").c_str(), name.c_str()))
                {
                    std::vector<std::string> extensions = std::vector<std::string>();
                    extensions.push_back("mat");
                    DisplayResources(lodGroup, extensions, j, i);

                    ImGui::EndCombo();
                }
                ImGui::PopID();
            }

            ImGui::DragFloat("MaxDistance", &lod.MaxDistance, 1.0f, 0.0f, 1000.0f);

            ImGui::PopID();
            i++;
        }

        if (ImGui::Button("Add New LOD"))
        {
            LOD newLOD = lodGroup->m_LODs[lodGroup->m_LODs.size() - 1];
            newLOD.Level += 1;
            lodGroup->m_LODs.push_back(newLOD);

        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto light = m_Actor->GetComponent<Light>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Light");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<Light>();

        const char* type = "";
        if (Cast<DirectionalLight>(light))
            type = "Directional";
        else if (Cast<PointLight>(light))
            type = "Point";
        else if (Cast<SpotLight>(light))
            type = "Spot";

        if (ImGui::BeginCombo("Type", type))
        {
            if (ImGui::Selectable("Directional"))
            {
                if (!Cast<DirectionalLight>(light))
                {
                    m_Actor->RemoveComponent<Light>();
                    m_Actor->AddComponent<DirectionalLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
                }
            }
            if (ImGui::Selectable("Point"))
            {
                if (!Cast<PointLight>(light))
                {
                    m_Actor->RemoveComponent<Light>();
                    m_Actor->AddComponent<PointLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
                }
            }
            if (ImGui::Selectable("Spot"))
            {
                if (!Cast<SpotLight>(light))
                {
                    m_Actor->RemoveComponent<Light>();
                    m_Actor->AddComponent<SpotLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
                }
            }

            ImGui::EndCombo();
        }

        ImGui::ColorPicker3("Color", glm::value_ptr(light->m_Color));
        ImGui::DragFloat("Intensity", &light->m_Intensity, 0.5f, 0.0f, 10000.0f);

        if (auto pointLight = m_Actor->GetComponent<PointLight>())
        {
            ImGui::DragFloat("Radius", &pointLight->m_Radius, 0.5f, 0.0f, 10000.0f);
            ImGui::DragFloat("FalloffExponent", &pointLight->m_FalloffExponent, 0.1f, 1.0f, 16.0f);
            ImGui::Checkbox("UseInverseSquaredFalloff", &pointLight->m_UseInverseSquaredFalloff);
        }

        if (auto spotLight = m_Actor->GetComponent<SpotLight>())
        {
            ImGui::DragFloat("Radius", &spotLight->m_Radius, 0.5f, 0.0f, 10000.0f);
            ImGui::DragFloat("FalloffExponent", &spotLight->m_FalloffExponent, 0.1f, 1.0f, 16.0f);
            ImGui::Checkbox("UseInverseSquaredFalloff", &spotLight->m_UseInverseSquaredFalloff);
            ImGui::DragFloat("Inner Cut Off", &spotLight->m_InnerCutOff, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Outer Cut Off", &spotLight->m_OuterCutOff, 0.01f, 0.0f, 1.0f);
        }

        ImGui::Checkbox("Cast Shadows", &light->m_ShadowsEnabled);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto skyLight = m_Actor->GetComponent<SkyLight>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Sky Light");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<SkyLight>();

        std::vector<std::string> paths = skyLight->GetPaths();
        for (auto path : paths)
        {
            std::string name = path.substr(path.find_last_of("/") + 1);
            if (ImGui::BeginCombo("Path", name.c_str()))
            {
                std::vector<std::string> extensions = std::vector<std::string>();
                extensions.push_back("png");
                DisplayResources(skyLight, extensions);

                ImGui::EndCombo();
            }
        }
        
        ImGui::Checkbox("Sky Visibility", &skyLight->m_SkyVisibility);
        ImGui::ColorEdit3("Color", glm::value_ptr(skyLight->m_Color), ImGuiColorEditFlags_Float);
        ImGui::DragFloat("Intensity", &skyLight->m_Intensity, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Weight", &skyLight->m_Weight, 0.01f, 0.0f, 1.0f);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto particles = m_Actor->GetComponent<ParticleSystemComponent>())
    {
        ImGui::PushID(particles.get());

        ImGui::Text("Particle System");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<ParticleSystemComponent>();

        if (ImGui::TreeNodeEx("Particle System", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat("Duration", &particles->m_Duration, 1.0f, 0.0f, 1000.0f);
            ImGui::Checkbox("Looping", &particles->m_Looping);

            std::string path = particles->m_Sprite->GetPath();
            std::string name = path.substr(path.find_last_of("/") + 1);
            if (ImGui::BeginCombo("Sprite", name.c_str()))
            {
                std::vector<std::string> extensions;
                extensions.push_back("png");
                DisplayResources(particles, extensions);

                ImGui::EndCombo();
            }

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Emission", ImGuiTreeNodeFlags_OpenOnArrow))
        {
            int maxParticles = particles->m_MaxParticles;
            ImGui::DragInt("Max Particles", &maxParticles, 10, 0, 10000);
            if (maxParticles != particles->m_MaxParticles)
                particles->SetMaxParticles(maxParticles);

            ImGui::DragFloat("Rate Over Time", &particles->m_EmissionRateOverTime, 0.1f, 0.0f, 100.0f);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Emitter Shape", ImGuiTreeNodeFlags_OpenOnArrow))
        {
            const char* shapeName = "";
            if (Cast<ParticleEmitterBox>(particles->m_EmitterShape))
                shapeName = "Box";
            if (Cast<ParticleEmitterSphere>(particles->m_EmitterShape))
                shapeName = "Sphere";

            if (ImGui::BeginCombo("Shape", shapeName))
            {
                if (ImGui::Selectable("Box"))
                    particles->m_EmitterShape = CreateRef<ParticleEmitterBox>();
                if (ImGui::Selectable("Sphere"))
                    particles->m_EmitterShape = CreateRef<ParticleEmitterSphere>();

                ImGui::EndCombo();
            }

            if (auto box = Cast<ParticleEmitterBox>(particles->m_EmitterShape))
                ImGui::DragFloat3("Size", glm::value_ptr(box->m_Size), 0.1f, 0.0f, 50.0f);
            if (auto sphere = Cast<ParticleEmitterSphere>(particles->m_EmitterShape))
                ImGui::DragFloat("Radius", &sphere->m_Radius, 0.1f, 0.0f, 50.0f);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Life Time", ImGuiTreeNodeFlags_OpenOnArrow))
        {
            ImGui::DragFloat("Min Life Time", &particles->m_MinParticleLifeTime, 0.1f, 0.0f, 100.0f);
            ImGui::DragFloat("Max Life Time", &particles->m_MaxParticleLifeTime, 0.1f, 0.0f, 100.0f);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Velocity Over Life Time", ImGuiTreeNodeFlags_OpenOnArrow))
        {
            ImGui::DragFloat3("Min Velocity", glm::value_ptr(particles->m_MinParticleVelocity), 0.1f, 0.0f, 100.0f);
            ImGui::DragFloat3("Max Velocity", glm::value_ptr(particles->m_MaxParticleVelocity), 0.1f, 0.0f, 100.0f);
            ImGui::DragFloat3("End Velocity", glm::value_ptr(particles->m_EndParticleVelocity), 0.1f, 0.0f, 100.0f);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Color Over Life Time", ImGuiTreeNodeFlags_OpenOnArrow))
        {
            ImGui::ColorPicker4("Start Color", glm::value_ptr(particles->m_StartParticleColor), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaBar);
            ImGui::ColorPicker4("End Color", glm::value_ptr(particles->m_EndParticleColor), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaBar);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Size Over Life Time", ImGuiTreeNodeFlags_OpenOnArrow))
        {
            ImGui::DragFloat("Min Size", &particles->m_MinParticleSize, 0.1f, 0.0f, 100.0f);
            ImGui::DragFloat("Max Size", &particles->m_MaxParticleSize, 0.1f, 0.0f, 100.0f);
            ImGui::DragFloat("End Size", &particles->m_EndParticleSize, 0.1f, 0.0f, 100.0f);

            ImGui::TreePop();
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::PopID();
        componentIndex++;
    }

    if (auto player = m_Actor->GetComponent<Player>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Player");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<Player>();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto postFX = m_Actor->GetComponent<PostProcessingVolume>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Post Processing Volume");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<PostProcessingVolume>();

        ImGui::Text("Bloom");
        ImGui::Checkbox("Bloom Enabled", &postFX->m_Settings.BloomEnabled);
        ImGui::DragFloat("Bloom Threshold", &postFX->m_Settings.BloomThreshold, 0.1f, 0.1f, 20.0f);
        ImGui::DragFloat("Bloom Limit", &postFX->m_Settings.BloomLimit, 1.0f, 1.0f, 100.0f);
        ImGui::DragFloat("Bloom Bloom Intensity", &postFX->m_Settings.BloomIntensity, 0.1f, 0.1f, 20.0f);
        ImGui::DragFloat("Bloom Blur Sigma", &postFX->m_Settings.BloomBlurSigma, 0.1f, 0.1f, 20.0f);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::Text("General");
        ImGui::DragFloat("Gamma", &postFX->m_Settings.Gamma, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat("Gain", &postFX->m_Settings.Gain, 0.01f, -1.0f, 1.f);
        ImGui::DragFloat("Exposure (inactive)", &postFX->m_Settings.Exposure, 0.1f, 0.0f, 10.0f);
        ImGui::DragFloat("Contrast", &postFX->m_Settings.Contrast, 0.01f, 0.0f, 1.9f);
        ImGui::DragFloat("Contrast Pivot", &postFX->m_Settings.ContrastPivot, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Offset", &postFX->m_Settings.Offset, 0.005f, -0.1f, 0.1f);
        ImGui::DragFloat("Lift", &postFX->m_Settings.Lift, 0.005f, -0.1f, 0.1f);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::Text("Color Grading");
        ImGui::DragFloat("Saturation", &postFX->m_Settings.Saturation, 0.1f, 0.0f, 2.0f);
        ImGui::DragFloat("Temperature", &postFX->m_Settings.Temperature, 1.f, 15.0f, 150.0f);
        ImGui::DragFloat("Hue", &postFX->m_Settings.Hue, 1.0f, 0.0f, 360.0f);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto cc = m_Actor->GetComponent<CharacterController>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Character Controller");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<CharacterController>();

        ImGui::DragFloat("Walk Speed", &cc->m_WalkSpeed, 0.5f, 0.0f, 100.0f);
        ImGui::DragFloat("Run Speed", &cc->m_RunSpeed, 0.5f, 0.0f, 100.0f);
        ImGui::DragFloat("Rotate Speed", &cc->m_RotateSpeed, 0.5f, 0.0f, 100.0f);
        ImGui::DragFloat("Look Up Limit", &cc->m_LookUpLimit, 0.5f, 0.0f, 90.0f);
        ImGui::DragFloat("Jump Height", &cc->m_JumpHeight, 0.5f, 0.0f, 100.0f);
        ImGui::DragFloat("Jump Max Height Time", &cc->m_JumpMaxHeightTime, 0.5f, 0.0f, 100.0f);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto platform = m_Actor->GetComponent<Platform>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Platform");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<Platform>();

        ImGui::DragFloat3("Move Direction", glm::value_ptr(platform->m_Direction), 0.1f, 0.0f, 1.0f);
        ImGui::DragFloat("Move Distance", &platform->m_Distance, 0.5f, 0.0f, 100.0f);
        ImGui::DragFloat("Move Speed", &platform->m_Speed, 0.5f, 0.0f, 100.0f);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto button = m_Actor->GetComponent<Button>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Button");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<Button>();

        size_t maxSize = 128;
        std::string idStr = std::to_string(button->m_PlatformID);
        char* id = (char*)idStr.c_str();
        ImGui::InputText("Connected Platform ID", id, maxSize);
        button->m_PlatformID = std::atoll(id);

        int i = 0;
        for (auto& connectedButtonID : button->m_ConnectedButtonsIDs)
        {
            ImGui::PushID(i);
            size_t maxSize = 128;
            std::string buttonIDStr = std::to_string(connectedButtonID);
            char* buttonIDText = (char*)buttonIDStr.c_str();
            ImGui::InputText("Connected Button ID", buttonIDText, maxSize);
            connectedButtonID = std::atoll(buttonIDText);
            ImGui::PopID();

            i++;
        }
        if (ImGui::Button("Add new Connected Button"))
            button->m_ConnectedButtonsIDs.push_back(0);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto ghost = m_Actor->GetComponent<Ghost>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Ghost");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<Ghost>();

        size_t maxSize = 128;
        std::string playerIDStr = std::to_string(ghost->m_PlayerID);
        char* id = (char*)playerIDStr.c_str();
        ImGui::InputText("Player ID", id, maxSize);
        ghost->m_PlayerID = std::atoll(id);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto checkpoint = m_Actor->GetComponent<Checkpoint>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Checkpoint");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<Checkpoint>();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto obelisk = m_Actor->GetComponent<Obelisk>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Obelisk");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<Obelisk>();

        const char* effectName = "";
        switch (obelisk->m_Effect)
        {
        case ObeliskEffect::Corrupt:
            effectName = "Corrupt";
            break;
        case ObeliskEffect::Heal:
            effectName = "Heal";
            break;
        case ObeliskEffect::GiveTeleportSkill:
            effectName = "Give Teleport Skill";
            break;
        }

        if (ImGui::BeginCombo("Effect", effectName))
        {
            if (ImGui::Selectable("Corrupt"))
                obelisk->m_Effect = ObeliskEffect::Corrupt;
            if (ImGui::Selectable("Heal"))
                obelisk->m_Effect = ObeliskEffect::Heal;
            if (ImGui::Selectable("Give Teleport Skill"))
                obelisk->m_Effect = ObeliskEffect::GiveTeleportSkill;

            ImGui::EndCombo();
        }

        size_t maxSize = 128;
        std::string idStr = std::to_string(obelisk->m_PostFXID);
        char* id = (char*)idStr.c_str();
        ImGui::InputText("Post FX Image ID", id, maxSize);
        obelisk->m_PostFXID = std::atoll(id);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto deathArea = m_Actor->GetComponent<DeathArea>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Death Area");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<DeathArea>();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto image = m_Actor->GetComponent<ImageComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Image");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<ImageComponent>();

        std::string path = image->m_Image->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Image", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("png");
            DisplayResources(image, extensions);

            ImGui::EndCombo();
        }
        ImGui::ColorEdit4("Image Color", glm::value_ptr(image->m_Color));
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto text = m_Actor->GetComponent<TextComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Text");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<TextComponent>();

        std::string path = text->m_Font->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Font", name.c_str()))
        {
            std::vector<std::string> extensions;
            extensions.push_back("ttf");
            DisplayResources(text, extensions);

            ImGui::EndCombo();
        }
        ImGui::DragFloat("Font Size", &text->m_FontSize, 0.1f, 0.0f, 100.0f);

        size_t maxSize = 2048;
        char* displayedText = (char*)text->m_Text.c_str();
        ImGui::InputText("Displayed Text", displayedText, maxSize, ImGuiInputTextFlags_CtrlEnterForNewLine);
        text->m_Text = displayedText;

        ImGui::ColorEdit4("Text Normal Color", glm::value_ptr(text->m_NormalColor));
        ImGui::ColorEdit4("Text Hovered Color", glm::value_ptr(text->m_HoveredColor));
        ImGui::ColorEdit4("Text Pressed Color", glm::value_ptr(text->m_PressedColor));
        ImGui::ColorEdit4("Text Disabled Color", glm::value_ptr(text->m_DisabledColor));

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto button = m_Actor->GetComponent<ButtonComponent>())
    {
        ImGui::PushID(componentIndex);

        const char* buttonState = "";
        switch (button->m_CurrentState)
        {
        case ButtonState::Normal:
            buttonState = "Normal";
            break;
        case ButtonState::Hovered:
            buttonState = "Hovered";
            break;
        case ButtonState::Pressed:
            buttonState = "Pressed";
            break;
        case ButtonState::Disabled:
            buttonState = "Disabled";
            break;
        }

        ImGui::Text("Button");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<ButtonComponent>();

        ImGui::Checkbox("Enabled", &button->m_Enabled);
        ImGui::Text("Current State: ");
        ImGui::SameLine();
        ImGui::Text(buttonState);
        std::string path = button->m_Image->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Image", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("png");
            DisplayResources(button, extensions);

            ImGui::EndCombo();
        }

        ImGui::ColorEdit4("Normal Color", glm::value_ptr(button->m_NormalColor));
        ImGui::ColorEdit4("Hovered Color", glm::value_ptr(button->m_HoveredColor));
        ImGui::ColorEdit4("Pressed Color", glm::value_ptr(button->m_PressedColor));
        ImGui::ColorEdit4("Disabled Color", glm::value_ptr(button->m_DisabledColor));
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto boxCollider = m_Actor->GetComponent<BoxColliderComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Box Collider");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<BoxColliderComponent>();

        ImGui::PushID(1);
        ImGui::Checkbox("Is Trigger", &boxCollider->m_IsTrigger);

        glm::vec3 center = boxCollider->m_Center;
        ImGui::DragFloat3("Center", glm::value_ptr(center), 0.05f, -100.0f, 100.0f);
        if (center != boxCollider->m_Center)
            boxCollider->SetCenter(center);

        glm::vec3 size = boxCollider->m_Size;
        ImGui::DragFloat3("Size", glm::value_ptr(size), 0.05f, 0.0f, 100.0f);
        if (size != boxCollider->m_Size)
            boxCollider->SetSize(size);

        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto sphereCollider = m_Actor->GetComponent<SphereColliderComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("Sphere Collider");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<SphereColliderComponent>();

        ImGui::PushID(2);
        ImGui::Checkbox("Is Trigger", &sphereCollider->m_IsTrigger);
        
        glm::vec3 center = sphereCollider->m_Center;
        ImGui::DragFloat3("Center", glm::value_ptr(center), 0.05f, -100.0f, 100.0f);
        if (center != sphereCollider->m_Center)
            sphereCollider->SetCenter(center);

        float size = sphereCollider->m_Size;
        ImGui::DragFloat("Size", &size);
        if (size != sphereCollider->m_Size)
            sphereCollider->SetSize(size);

        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto rigidBody = m_Actor->GetComponent<RigidBodyComponent>())
    {
        ImGui::PushID(componentIndex);

        ImGui::Text("RigidBody");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<RigidBodyComponent>();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto audioListener = m_Actor->GetComponent<AudioListenerComponent>()) {
        ImGui::PushID(componentIndex);

        ImGui::Text("Audio Listener");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<AudioListenerComponent>();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
        componentIndex++;
    }

    if (auto audioSource = m_Actor->GetComponent<AudioSourceComponent>()) {
        ImGui::PushID(componentIndex);

        ImGui::Text("Audio Source");
        ImGui::SameLine();
        if (ImGui::Button("X"))
            m_Actor->RemoveComponent<AudioSourceComponent>();

        std::string path = audioSource->m_Sound;
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Sound", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("mp3");
            extensions.push_back("wav");
            extensions.push_back("ogg");
            DisplayResources(audioSource, extensions);

            ImGui::EndCombo();
        }
        switch (audioSource->m_Channel) {
            case MUSIC:
                name = "Music";
                break;
            case SFX:
                name = "SFX";
                break;
        }
        if (ImGui::BeginCombo("Channel", name.c_str()))
        {
            if (ImGui::Selectable("Music"))
                audioSource->m_Channel = CHANNEL_GROUP::MUSIC;
            if (ImGui::Selectable("SFX"))
                audioSource->m_Channel = CHANNEL_GROUP::SFX;
            ImGui::EndCombo();
        }
        if (ImGui::DragFloat("Volume", &audioSource->m_Volume, 0.01f, 0.0f, 1.0f))
            audioSource->SetVolume(audioSource->m_Volume);
        if (ImGui::Checkbox("Is 3D", &audioSource->m_3d))
            audioSource->UpdateMode();
        if (ImGui::Checkbox("Looping", &audioSource->m_Looping))
            audioSource->UpdateMode();
        ImGui::Checkbox("Play on Start", &audioSource->m_PlayOnStart);
        if (ImGui::Button("Play", ImVec2(60,20)))
            audioSource->PlaySound();
        if (ImGui::Button("Stop", ImVec2(60,20)))
            audioSource->StopSound();

        ImGui::PopID();
        componentIndex++;
    }

    bool addComponent = false;
    bool staticMesh = false;
    bool foliage = false;
    bool skeletalMesh = false;
    bool lodGroup = false;
    bool dirLight = false;
    bool pointLight = false;
    bool spotLight = false;
    bool skyLight = false;
    bool particleSystem = false;
    bool camera = false;
    bool image = false;
    bool buttonUI = false;
    bool text = false;
    bool boxCollider = false;
    bool sphereCollider = false;
    bool rigidBody = false;

    bool player = false;
    bool characterController = false;
    bool cameraController = false;
    bool platform = false;
    bool button = false;
    bool ghost = false;
    bool obelisk = false;
    bool checkpoint = false;
    bool deathArea = false;
    bool postFX = false;
    bool audioSource = false;
    bool audioListener = false;

    if (m_Actor->GetComponent<TransformComponent>())
    {
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::BeginMenu("Add Component"))
            {
                ImGui::MenuItem("Static Mesh", "", &staticMesh);
                ImGui::MenuItem("Foliage", "", &foliage);
                ImGui::MenuItem("Skeletal Mesh", "", &skeletalMesh);
                ImGui::MenuItem("LOD Group", "", &lodGroup);
                if (ImGui::BeginMenu("Light"))
                {
                    ImGui::MenuItem("Directional Light", "", &dirLight);
                    ImGui::MenuItem("Point Light", "", &pointLight);
                    ImGui::MenuItem("Spot Light", "", &spotLight);
                    ImGui::MenuItem("Sky Light", "", &skyLight);

                    ImGui::EndMenu();
                }
                ImGui::MenuItem("Particle System", "", &particleSystem);
                ImGui::MenuItem("Camera", "", &camera);

                if (ImGui::BeginMenu("Collider"))
                {
                    ImGui::MenuItem("Box Collider", "", &boxCollider);
                    ImGui::MenuItem("Sphere Collider", "", &sphereCollider);

                    ImGui::EndMenu();
                }

                ImGui::MenuItem("RigidBody", "", &rigidBody);

                ImGui::MenuItem("Audio Source", "", &audioSource);
                ImGui::MenuItem("Audio Listener", "", &audioListener);

                if (ImGui::BeginMenu("Game Components"))
                {
                    ImGui::MenuItem("Player", "", &player);
                    ImGui::MenuItem("Character Controller", "", &characterController);
                    ImGui::MenuItem("Camera Controller", "", &cameraController);
                    ImGui::MenuItem("Platform", "", &platform);
                    ImGui::MenuItem("Button", "", &button);
                    ImGui::MenuItem("Ghost", "", &ghost);
                    ImGui::MenuItem("Checkpoint", "", &checkpoint);
                    ImGui::MenuItem("Death Area", "", &deathArea);
                    ImGui::MenuItem("Obelisk", "", &obelisk);
                    ImGui::MenuItem("Post Processing Volume", "", &postFX);

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }
    }
    else if (m_Actor->GetComponent<RectTransformComponent>())
    {
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::BeginMenu("Add Component"))
            {
                if (ImGui::BeginMenu("UI"))
                {
                    ImGui::MenuItem("Image", "", &image);
                    ImGui::MenuItem("Button", "", &buttonUI);
                    ImGui::MenuItem("Text", "", &text);

                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }

    }
    

    if (staticMesh)
        m_Actor->AddComponent<StaticMeshComponent>();
    if (foliage)
        m_Actor->AddComponent<FoliageComponent>();
    if (skeletalMesh)
        m_Actor->AddComponent<SkeletalMeshComponent>();
    if (lodGroup)
        m_Actor->AddComponent<LODGroupComponent>();
    if (dirLight)
        m_Actor->AddComponent<DirectionalLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
    if (pointLight)
        m_Actor->AddComponent<PointLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
    if (spotLight)
        m_Actor->AddComponent<SpotLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
    if (skyLight)
    {
        std::vector<std::string> defaultPaths;
        defaultPaths.push_back("../../../Content/Textures/Sky/Default/px.png");
        defaultPaths.push_back("../../../Content/Textures/Sky/Default/nx.png");
        defaultPaths.push_back("../../../Content/Textures/Sky/Default/py.png");
        defaultPaths.push_back("../../../Content/Textures/Sky/Default/ny.png");
        defaultPaths.push_back("../../../Content/Textures/Sky/Default/nz.png");
        defaultPaths.push_back("../../../Content/Textures/Sky/Default/pz.png");

        auto sk = m_Actor->AddComponent<SkyLight>(defaultPaths);
        m_Actor->GetScene()->m_SkyLight = sk;

    }
    if (particleSystem)
        m_Actor->AddComponent<ParticleSystemComponent>();
    if (camera)
        m_Actor->AddComponent<CameraComponent>();
    if (image)
        m_Actor->AddComponent<ImageComponent>();
    if (buttonUI)
        m_Actor->AddComponent<ButtonComponent>();
    if (text)
        m_Actor->AddComponent<TextComponent>();
    if (boxCollider)
        m_Actor->AddComponent<BoxColliderComponent>();
    if (sphereCollider)
        m_Actor->AddComponent<SphereColliderComponent>();
    if (rigidBody)
        m_Actor->AddComponent<RigidBodyComponent>();

    if (player)
        m_Actor->AddComponent<Player>();
    if (characterController)
        m_Actor->AddComponent<CharacterController>();
    if (cameraController)
        m_Actor->AddComponent<CameraController>();
    if (platform)
        m_Actor->AddComponent<Platform>();
    if (button)
        m_Actor->AddComponent<Button>();
    if (ghost)
        m_Actor->AddComponent<Ghost>();
    if (obelisk)
        m_Actor->AddComponent<Obelisk>();
    if (checkpoint)
        m_Actor->AddComponent<Checkpoint>();
    if (deathArea)
        m_Actor->AddComponent<DeathArea>();
    if (postFX)
        m_Actor->AddComponent<PostProcessingVolume>();
    if (audioSource)
        m_Actor->AddComponent<AudioSourceComponent>();
    if (audioListener)
        m_Actor->AddComponent<AudioListenerComponent>();


    if (ImGui::Button("Close"))
    {
        m_Editor->HideDetails();
        m_Editor->GetSceneHierarchyPanel()->UnselectActor();
    }

	ImGui::End();
}

void ActorDetailsPanel::DisplayResources(Ref<Component> component, std::vector<std::string> extensions, int index, int lod)
{
    for (auto& p : std::filesystem::recursive_directory_iterator(AssetManager::ContentDirectory))
    {
        std::stringstream ss;
        ss << p.path();
        std::string path = ss.str();
        CorrectPath(path);
        std::string name = path.substr(path.find_last_of("/") + 1);
        std::string ext = name.substr(name.find_first_of('.') + 1);

        for (auto extension : extensions)
        {
            if (extension == ext)
            {
                const bool isSelected = false;
                if (ImGui::Selectable(name.c_str(), &isSelected))
                {
                    if (ext == "obj" || ext == "fbx" || ext == "3ds" || ext == "dae")
                    {
                        if (auto mesh = Cast<MeshComponent>(component))
                            mesh->ChangeMesh(path);

                        if (auto lodGroup = Cast<LODGroupComponent>(component))
                            lodGroup->m_LODs[lod].Model = AssetManager::LoadModel(path);

                        if (auto foliage = Cast<FoliageComponent>(component))
                            foliage->ChangeMesh(path);

                    }
                    else if (ext == "mat")
                    {
                        if (auto mesh = Cast<MeshComponent>(component))
                            mesh->ChangeMaterial(index, path);

                        if (auto foliage = Cast<FoliageComponent>(component))
                            foliage->ChangeMaterial(path);

                        if (auto lodGroup = Cast<LODGroupComponent>(component))
                            lodGroup->m_LODs[lod].Materials[index] = AssetManager::LoadMaterial(path);
                    }
                    else if (ext == "png")
                    {
                        if (auto image = Cast<ImageComponent>(component))
                        {
                            image->ChangeImage(path);
                        }
                        if (auto button = Cast<ButtonComponent>(component))
                        {
                            button->ChangeImage(path);
                        }
                        if (auto skyLight = Cast<SkyLight>(component))
                        {
                            //skyLight->Load(path);
                        }
                        if (auto particles = Cast<ParticleSystemComponent>(component))
                        {
                            particles->ChangeSprite(path);
                        }
                    }
                    else if (ext == "mp3" || ext == "wav" || ext == "ogg")
                    {
                        if (auto audio = Cast<AudioSourceComponent>(component))
                        {
                            audio->ChangeSound(AssetManager::ContentDirectory + path);
                        }
                    }
                    else if (ext == "ttf")
                    {
                        if (auto text = Cast<TextComponent>(component))
                        {
                            text->ChangeFont(path);
                        }
                    }
                }
            }
        }
    }

}

void ActorDetailsPanel::CorrectPath(std::string& path)
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

    path = path.substr(AssetManager::ContentDirectory.size());
}
