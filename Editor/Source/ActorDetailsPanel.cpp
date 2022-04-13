#include "ActorDetailsPanel.h"

#include "imgui.h"
#include "Editor.h"
#include "Content/ContentHelper.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Animation/SkeletalMeshComponent.h"
#include "Scene/Component/Animation/Animator.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Scene/Component/Light/SkyLight.h"
#include "Scene/Component/ParticleSystemComponent.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Scene/Component/UI/RectTransformComponent.h"
#include "Scene/Component/Collider/BoxColliderComponent.h"
#include "Game/Player.h"

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

    size_t maxSize = 128;
    char* name = (char*)m_Actor->m_Name.c_str();
    ImGui::InputText("##Name", name, maxSize);
    m_Actor->m_Name = name;
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
        ImGui::Text("Camera");

        ImGui::DragFloat2("Aspect Ratio", (float*)&camera->m_AspectRatio, 1.0f, 1.0f, 30.0f);
        ImGui::DragFloat("Field Of View", &camera->m_FieldOfView, 1.0f, 1.0f, 180.0f);
        ImGui::DragFloat("Near Clip Plane", &camera->m_NearClipPlane, 0.1f, 0.1f, 1000.0f);
        ImGui::DragFloat("Far Clip Plane", &camera->m_FarClipPlane, 0.1f, 0.1f, 1000.0f);
    }

    if (auto mesh = m_Actor->GetComponent<StaticMeshComponent>())
    {
        ImGui::Text("Static Mesh");

        std::string path = mesh->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Static Mesh", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("obj");
            extensions.push_back("fbx");
            extensions.push_back("3ds");
            extensions.push_back("dae");
            DisplayResources(extensions);

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
                DisplayResources(extensions, i);

                ImGui::EndCombo();
            }
            ImGui::PopID();
        }
    }

    if (auto animator = m_Actor->GetComponent<Animator>())
    {
        ImGui::Text("Animator Component");

        ImGui::Text("Animations available: %i", animator->HowManyAnimationsAreThere());

        ImGui::Text("Animation current time: %f", animator->GetCurrentAnimationTime());

        ImGui::Dummy(ImVec2(0.0, 10.0));
        // [...]
    }

    if (auto mesh = m_Actor->GetComponent<SkeletalMeshComponent>())
    {
        ImGui::Text("Skeletal Mesh");

        std::string path = mesh->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Skeletal Mesh", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("obj");
            extensions.push_back("fbx");
            extensions.push_back("3ds");
            extensions.push_back("dae");
            DisplayResources(extensions);

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
                DisplayResources(extensions, i);

                ImGui::EndCombo();
            }
            ImGui::PopID();
        }

        // Skeleton informations
        ImGui::Text("Skeleton informations");
        ImGui::Text("Bones: %i", mesh->GetBoneCount());

    }

    if (auto light = m_Actor->GetComponent<Light>())
    {
        ImGui::Text("Light");

        ImGui::Text("Light Type: ");
        const char* type = "";

        if (Cast<DirectionalLight>(light))
            type = "Directional";
        else if (Cast<PointLight>(light))
            type = "Point";
        else if (Cast<SpotLight>(light))
            type = "Spot";

        ImGui::SameLine();
        if (ImGui::BeginMenu(type))
        {
            if (ImGui::MenuItem("Directional"))
            {
                if (!Cast<DirectionalLight>(light))
                {
                    m_Actor->RemoveComponent<Light>();
                    m_Actor->AddComponent<DirectionalLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
                }
            }
            if (ImGui::MenuItem("Point"))
            {
                if (!Cast<PointLight>(light))
                {
                    m_Actor->RemoveComponent<Light>();
                    m_Actor->AddComponent<PointLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
                }
            }
            if (ImGui::MenuItem("Spot"))
            {
                if (!Cast<SpotLight>(light))
                {
                    m_Actor->RemoveComponent<Light>();
                    m_Actor->AddComponent<SpotLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
                }
            }

            ImGui::EndMenu();
        }

        ImGui::DragFloat3("Color", (float*)&light->m_Color, 0.1f, 0.0f, 100.0f);
        ImGui::Checkbox("Cast Shadows", &light->m_ShadowsEnabled);

        if (auto spotLight = m_Actor->GetComponent<SpotLight>())
        {
            float temp = spotLight->m_InnerCutOff;
            ImGui::DragFloat("Inner Cut Off", &temp, 0.01f, 0.0f, 1.0f);
            if (temp != spotLight->m_InnerCutOff)
                spotLight->SetInnerCutOff(temp);

            temp = spotLight->m_OuterCutOff;
            ImGui::DragFloat("Outer Cut Off", &temp, 0.01f, 0.0f, 1.0f);
            if (temp != spotLight->m_OuterCutOff)
                spotLight->SetOuterCutOff(temp);
        }
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto skyLight = m_Actor->GetComponent<SkyLight>())
    {
        ImGui::Text("Sky Light");

        std::vector<std::string> paths = skyLight->GetPaths();
        for (auto path : paths)
        {
            std::string name = path.substr(path.find_last_of("/") + 1);
            if (ImGui::BeginCombo("Path", name.c_str()))
            {
                std::vector<std::string> extensions = std::vector<std::string>();
                extensions.push_back("png");
                DisplayResources(extensions);

                ImGui::EndCombo();
            }
        }
        
        ImGui::Checkbox("Sky Visibility", &skyLight->m_SkyVisibility);
        ImGui::ColorEdit3("Color", glm::value_ptr(skyLight->m_Color), ImGuiColorEditFlags_Float);
        ImGui::DragFloat("Intensity", &skyLight->m_Intensity, 0.01f, 0.0f, 1.0f);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto particles = m_Actor->GetComponent<ParticleSystemComponent>())
    {
        ImGui::Text("Particle System");
        int count = particles->m_ParticlesCount;
        ImGui::DragInt("Particles Count", &count, 100, 0, 100000);
        if (count != particles->m_ParticlesCount)
            particles->SetParticlesCount(count);

        float lifeTime = particles->m_ParticleLifeTime;
        ImGui::DragFloat("Particle Life Time", &lifeTime, 0.01f, 0.01f, 10.0f);
        if (lifeTime != particles->m_ParticleLifeTime)
            particles->SetParticleLifeTime(lifeTime);

        float radius = particles->m_Radius;
        ImGui::DragFloat("Sphere Radius", &radius, 1.0f, 1.0f, 1000.0f);
        if (radius != particles->m_Radius)
            particles->SetRadius(radius);

        glm::vec3 minVel = particles->m_MinVelocity;
        ImGui::DragFloat3("Min Velocity", glm::value_ptr(minVel), 0.01f, 0.0f, 1.0f);
        if (minVel != particles->m_MinVelocity &&
            (minVel.x <= particles->m_MaxVelocity.x &&
             minVel.y <= particles->m_MaxVelocity.y &&
             minVel.z <= particles->m_MaxVelocity.z))
            particles->SetMinVelocity(minVel);

        glm::vec3 maxVel = particles->m_MaxVelocity;
        ImGui::DragFloat3("Max Velocity", glm::value_ptr(maxVel), 0.01f, 0.0f, 1.0f);
        if (maxVel != particles->m_MaxVelocity &&
            (maxVel.x >= particles->m_MinVelocity.x &&
             maxVel.y >= particles->m_MinVelocity.y &&
             maxVel.z >= particles->m_MinVelocity.z))
            particles->SetMaxVelocity(maxVel);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto player = m_Actor->GetComponent<Player>())
    {
        ImGui::Text("Player");
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }
    if (auto image = m_Actor->GetComponent<ImageComponent>())
    {
        ImGui::Text("Image");

        std::string path = image->m_Image->GetPath();
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (ImGui::BeginCombo("Image", name.c_str()))
        {
            std::vector<std::string> extensions = std::vector<std::string>();
            extensions.push_back("png");
            DisplayResources(extensions);

            ImGui::EndCombo();
        }
        ImGui::ColorEdit4("Image Color", glm::value_ptr(image->m_Color));
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }
    if (auto button = m_Actor->GetComponent<ButtonComponent>())
    {
        const char* buttonState = "";
        switch (button->m_CurrentState)
        {
        case ButtonComponent::ButtonState::Normal:
            buttonState = "Normal";
            break;
        case ButtonComponent::ButtonState::Hovered:
            buttonState = "Hovered";
            break;
        case ButtonComponent::ButtonState::Pressed:
            buttonState = "Pressed";
            break;
        case ButtonComponent::ButtonState::Disabled:
            buttonState = "Disabled";
            break;
        }

        ImGui::Text("Button");

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
            DisplayResources(extensions);

            ImGui::EndCombo();
        }

        ImGui::ColorEdit4("Normal Color", glm::value_ptr(button->m_NormalColor));
        ImGui::ColorEdit4("Hovered Color", glm::value_ptr(button->m_HoveredColor));
        ImGui::ColorEdit4("Pressed Color", glm::value_ptr(button->m_PressedColor));
        ImGui::ColorEdit4("Disabled Color", glm::value_ptr(button->m_DisabledColor));
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto boxCollider = m_Actor->GetComponent<BoxColliderComponent>())
    {
        ImGui::Text("Box Collider");
        ImGui::PushID(1);
        ImGui::Checkbox("Is Trigger", &boxCollider->m_IsTrigger);
        ImGui::DragFloat3("Center", glm::value_ptr(boxCollider->m_Center), 0.05f, -100.0f, 100.0f);
        ImGui::DragFloat3("Size", glm::value_ptr(boxCollider->m_Size), 0.05f, 0.0f, 100.0f);
        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto sphereCollider = m_Actor->GetComponent<SphereColliderComponent>())
    {
        ImGui::Text("Sphere Collider");
        ImGui::PushID(2);
        ImGui::Checkbox("Is Trigger", &sphereCollider->m_IsTrigger);
        ImGui::DragFloat3("Center", glm::value_ptr(sphereCollider->m_Center), 0.05f, -100.0f, 100.0f);
        ImGui::DragFloat("Size", &sphereCollider->m_Size);
        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    if (auto rigidBody = m_Actor->GetComponent<RigidBodyComponent>())
    {
        ImGui::Text("RigidBody");
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    bool addComponent = false;
    bool staticMesh = false;
    bool instanceRenderedMesh = false;
    bool skeletalMesh = false;
    bool animator = false;
    bool dirLight = false;
    bool pointLight = false;
    bool spotLight = false;
    bool skyLight = false;
    bool particleSystem = false;
    bool player = false;
    bool camera = false;
    bool image = false;
    bool button = false;
    bool boxCollider = false;
    bool sphereCollider = false;
    bool rigidBody = false;

    if (m_Actor->GetComponent<TransformComponent>())
    {
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::BeginMenu("Add Component"))
            {
                ImGui::MenuItem("Static Mesh", "", &staticMesh);
                ImGui::MenuItem("Instance Rendered Mesh", "", &instanceRenderedMesh);
                ImGui::MenuItem("Skeletal Mesh", "", &skeletalMesh);
                ImGui::MenuItem("Animator", "", &animator);
                if (ImGui::BeginMenu("Light"))
                {
                    ImGui::MenuItem("Directional Light", "", &dirLight);
                    ImGui::MenuItem("Point Light", "", &pointLight);
                    ImGui::MenuItem("Spot Light", "", &spotLight);
                    ImGui::MenuItem("Sky Light", "", &skyLight);

                    ImGui::EndMenu();
                }
                ImGui::MenuItem("Particle System", "", &particleSystem);
                ImGui::MenuItem("Player", "", &player);
                ImGui::MenuItem("Camera", "", &camera);

                if (ImGui::BeginMenu("Collider"))
                {
                    ImGui::MenuItem("Box Collider", "", &boxCollider);
                    ImGui::MenuItem("Sphere Collider", "", &sphereCollider);

                    ImGui::EndMenu();
                }

                ImGui::MenuItem("RigidBody", "", &rigidBody);

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
                    ImGui::MenuItem("Button", "", &button);

                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }

    }
    

    if (staticMesh)
        m_Actor->AddComponent<StaticMeshComponent>();
    if (skeletalMesh)
        m_Actor->AddComponent<SkeletalMeshComponent>();
    if (animator)
    {
        if (m_Actor->GetComponent<SkeletalMeshComponent>() == nullptr)
        {
            m_Actor->AddComponent<SkeletalMeshComponent>();
        }

        m_Actor->AddComponent<Animator>();
    }
    if (dirLight)
        m_Actor->AddComponent<DirectionalLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
    if (pointLight)
        m_Actor->AddComponent<PointLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
    if (spotLight)
        m_Actor->AddComponent<SpotLight>(m_Actor->m_Scene->m_LightsVertexUniformBuffer, m_Actor->m_Scene->m_LightsFragmentUniformBuffer);
    if (skyLight)
    {
        std::vector<std::string> defaultPaths;
        defaultPaths.push_back(ContentHelper::GetAssetPath("Textures/Sky/Default/px.png"));
        defaultPaths.push_back(ContentHelper::GetAssetPath("Textures/Sky/Default/nx.png"));
        defaultPaths.push_back(ContentHelper::GetAssetPath("Textures/Sky/Default/py.png"));
        defaultPaths.push_back(ContentHelper::GetAssetPath("Textures/Sky/Default/ny.png"));
        defaultPaths.push_back(ContentHelper::GetAssetPath("Textures/Sky/Default/nz.png"));
        defaultPaths.push_back(ContentHelper::GetAssetPath("Textures/Sky/Default/pz.png"));

        m_Actor->AddComponent<SkyLight>(defaultPaths);

    }
    if (particleSystem)
        m_Actor->AddComponent<ParticleSystemComponent>();
    if (player)
        m_Actor->AddComponent<Player>();
    if (camera)
        m_Actor->AddComponent<CameraComponent>();
    if (image)
        m_Actor->AddComponent<ImageComponent>();
    if (button)
        m_Actor->AddComponent<ButtonComponent>();
    if (boxCollider)
        m_Actor->AddComponent<BoxColliderComponent>();
    if (sphereCollider)
        m_Actor->AddComponent<SphereColliderComponent>();
    if (rigidBody)
        m_Actor->AddComponent<RigidBodyComponent>();

    if (ImGui::Button("Close"))
    {
        m_Editor->HideDetails();
        m_Editor->GetSceneHierarchyPanel()->UnselectActor();
    }

	ImGui::End();
}

void ActorDetailsPanel::DisplayResources(std::vector<std::string> extensions, int index)
{
    for (auto& p : std::filesystem::recursive_directory_iterator(ContentHelper::GetAssetPath("")))
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
                        if (auto mesh = m_Actor->GetComponent<MeshComponent>())
                            mesh->ChangeMesh(path);
                    }
                    else if (ext == "mat")
                    {
                        if (auto mesh = m_Actor->GetComponent<MeshComponent>())
                            mesh->ChangeMaterial(index, path);
                    }
                    else if (ext == "hdr")
                    {

                    }
                    else if (ext == "png")
                    {
                        if (auto image = m_Actor->GetComponent<ImageComponent>())
                        {
                            image->ChangeImage(path);
                        }
                        if (auto button = m_Actor->GetComponent<ButtonComponent>())
                        {
                            button->ChangeImage(path);
                        }
                        if (auto skyLight = m_Actor->GetComponent<SkyLight>())
                        {
                            //skyLight->Load(path);

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

    path = path.substr(ContentHelper::GetAssetPath("").size());
}
