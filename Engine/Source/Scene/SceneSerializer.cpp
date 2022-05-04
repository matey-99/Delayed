#include "SceneSerializer.h"

#include "Assets/AssetManager.h"

#include "yaml/yaml.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Animation/SkeletalMeshComponent.h"
#include "Scene/Component/LODGroupComponent.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Scene/Component/Light/SkyLight.h"
#include "Scene/Component/Particle/ParticleSystemComponent.h"
#include "Scene/Component/Particle/ParticleEmitterSphere.h"
#include "Scene/Component/Particle/ParticleEmitterBox.h"
#include "Scene/Component/Collider/BoxColliderComponent.h"
#include <Scene/Component/Collider/SphereColliderComponent.h>
#include <Scene/Component/RigidBodyComponent.h>
#include "Scene/Component/UI/ImageComponent.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Scene/Component/UI/RectTransformComponent.h"

#include "Renderer/RenderPass/SSAOPass.h"
#include "Renderer/RenderPass/DepthFogPass.h"
#include "Renderer/RenderPass/PostProcessingPass.h"
#include "Renderer/RenderPass/FXAAPass.h"
#include "Renderer/RenderPass/VignettePass.h"
#include "Renderer/RenderPass/DepthOfFieldPass.h"

#include "Game/MainMenu.h"
#include "Game/Player.h"
#include "Game/Button.h"
#include "Game/Ghost.h"
#include "Game/DeathArea.h"
#include "Game/Checkpoint.h"

void SceneSerializer::Serialize(Ref<Scene> scene, std::string destinationPath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << scene->m_Name;
	out << YAML::Key << "CurrentCamera" << YAML::Value << scene->m_CurrentCamera->GetOwner()->GetID();

	auto renderer = Renderer::GetInstance();
	out << YAML::Key << "RendererSettings" << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "DepthFog" << YAML::Value << renderer->GetSettings().DepthFogEnabled;
	out << YAML::Key << "PostProcessing" << YAML::Value << renderer->GetSettings().PostProcessingEnabled;
	out << YAML::Key << "FXAA" << YAML::Value << renderer->GetSettings().FXAAEnabled;
	out << YAML::Key << "Vignette" << YAML::Value << renderer->GetSettings().VignetteEnabled;
	out << YAML::Key << "DepthOfField" << YAML::Value << renderer->GetSettings().DepthOfFieldEnabled;

	out << YAML::Key << "SSAOSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Intensity" << YAML::Value << renderer->m_SSAOPass->m_Settings.Intensity;
	out << YAML::Key << "KernelSize" << YAML::Value << renderer->m_SSAOPass->m_Settings.KernelSize;
	out << YAML::Key << "Radius" << YAML::Value << renderer->m_SSAOPass->m_Settings.Radius;
	out << YAML::Key << "Bias" << YAML::Value << renderer->m_SSAOPass->m_Settings.Bias;
	out << YAML::EndMap;

	out << YAML::Key << "DepthFogSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "MinDistance" << YAML::Value << renderer->m_DepthFogPass->m_Settings.MinDistance;
	out << YAML::Key << "MaxDistance" << YAML::Value << renderer->m_DepthFogPass->m_Settings.MaxDistance;
	out << YAML::Key << "Density" << YAML::Value << renderer->m_DepthFogPass->m_Settings.Density;
	out << YAML::Key << "Color" << YAML::Value << renderer->m_DepthFogPass->m_Settings.Color;
	out << YAML::EndMap;

	out << YAML::Key << "PostProcessingSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Gamma" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Gamma;
	out << YAML::Key << "Gain" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Gain;
	out << YAML::Key << "Lift" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Lift;
	out << YAML::Key << "Offset" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Offset;
	out << YAML::Key << "Exposure" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Exposure;
	out << YAML::Key << "Contrast" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Contrast;
	out << YAML::Key << "ContrastPivot" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.ContrastPivot;
	out << YAML::Key << "BloomEnabled" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.BloomEnabled;
	out << YAML::Key << "BloomThreshold" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.BloomThreshold;
	out << YAML::Key << "BloomLimit" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.BloomLimit;
	out << YAML::Key << "BloomIntensity" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.BloomIntensity;
	out << YAML::Key << "BloomBlurSigma" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.BloomBlurSigma;
	out << YAML::Key << "Saturation" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Saturation;
	out << YAML::Key << "Temperature" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Temperature;
	out << YAML::Key << "Hue" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Hue;
	out << YAML::EndMap;

	out << YAML::Key << "VignetteSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Intensity" << YAML::Value << renderer->m_VignettePass->m_Settings.Intensity;
	out << YAML::Key << "Size" << YAML::Value << renderer->m_VignettePass->m_Settings.Size;
	out << YAML::EndMap;

	out << YAML::Key << "DepthOfFieldSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "MinDistance" << YAML::Value << renderer->m_DepthOfFieldPass->m_Settings.MinDistance;
	out << YAML::Key << "MaxDistance" << YAML::Value << renderer->m_DepthOfFieldPass->m_Settings.MaxDistance;
	out << YAML::Key << "Size" << YAML::Value << renderer->m_DepthOfFieldPass->m_Settings.Size;
	out << YAML::EndMap;

	out << YAML::EndMap;

	out << YAML::Key << "Actors" << YAML::Value << YAML::BeginSeq;
	for (auto actor : scene->GetActors())
	{
		SerializeActor(out, actor);
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream file(destinationPath);
	file << out.c_str();
	file.close();
}

Ref<Scene> SceneSerializer::Deserialize(std::string path)
{
	Ref<Scene> scene = CreateRef<Scene>();

	std::ifstream file(path);
	std::stringstream ss;
	ss << file.rdbuf();

	YAML::Node data = YAML::Load(ss.str());
	if (!data["Scene"])
	{
		std::cout << "Cannot load scene!" << std::endl;
		return Ref<Scene>();
	}
	
	std::string sceneName = data["Scene"].as<std::string>();
	scene->m_Name = sceneName;

	auto renderer = Renderer::GetInstance();

#pragma region Renderer

	/* Renderer settings */
	bool depthFog = data["RendererSettings"]["DepthFog"].as<bool>();
	bool postProcessing = data["RendererSettings"]["PostProcessing"].as<bool>();
	bool fxaa = data["RendererSettings"]["FXAA"].as<bool>();
	bool vignette = data["RendererSettings"]["Vignette"].as<bool>();
	bool depthOfField = data["RendererSettings"]["DepthOfField"].as<bool>();

	/* SSAO settings */
	float ssaoIntensity = data["RendererSettings"]["SSAOSettings"]["Intensity"].as<float>();
	float ssaoKernelSize = data["RendererSettings"]["SSAOSettings"]["KernelSize"].as<int>();
	float ssaoRadius = data["RendererSettings"]["SSAOSettings"]["Radius"].as<float>();
	float ssaoBias = data["RendererSettings"]["SSAOSettings"]["Bias"].as<float>();

	/* Depth Fog settings */
	float dfMinDistance = data["RendererSettings"]["DepthFogSettings"]["MinDistance"].as<float>();
	float dfMaxDistance = data["RendererSettings"]["DepthFogSettings"]["MaxDistance"].as<float>();
	float dfDensity = data["RendererSettings"]["DepthFogSettings"]["Density"].as<float>();
	glm::vec3 dfColor = data["RendererSettings"]["DepthFogSettings"]["Color"].as<glm::vec3>();

	/* Post Processing settings */
	float gamma = data["RendererSettings"]["PostProcessingSettings"]["Gamma"].as<float>();
	float gain = data["RendererSettings"]["PostProcessingSettings"]["Gain"].as<float>();
	float lift = data["RendererSettings"]["PostProcessingSettings"]["Lift"].as<float>();
	float offset = data["RendererSettings"]["PostProcessingSettings"]["Offset"].as<float>();
	float exposure = data["RendererSettings"]["PostProcessingSettings"]["Exposure"].as<float>();
	float contrast = data["RendererSettings"]["PostProcessingSettings"]["Contrast"].as<float>();
	float contrastPivot = data["RendererSettings"]["PostProcessingSettings"]["ContrastPivot"].as<float>();
	bool bloomEnabled = data["RendererSettings"]["PostProcessingSettings"]["BloomEnabled"].as<bool>();
	float bloomThreshold = data["RendererSettings"]["PostProcessingSettings"]["BloomThreshold"].as<float>();
	float bloomLimit = data["RendererSettings"]["PostProcessingSettings"]["BloomLimit"].as<float>();
	float bloomIntensity = data["RendererSettings"]["PostProcessingSettings"]["BloomIntensity"].as<float>();
	float bloomBlurSigma = data["RendererSettings"]["PostProcessingSettings"]["BloomBlurSigma"].as<float>();
	float saturation = data["RendererSettings"]["PostProcessingSettings"]["Saturation"].as<float>();
	float temperature = data["RendererSettings"]["PostProcessingSettings"]["Temperature"].as<float>();
	float hue = data["RendererSettings"]["PostProcessingSettings"]["Hue"].as<float>();

	/* Vignette settings */
	float vignetteIntensity = data["RendererSettings"]["VignetteSettings"]["Intensity"].as<float>();
	float vignetteSize = data["RendererSettings"]["VignetteSettings"]["Size"].as<float>();

	/* Depth Of Field settings */
	float dofMinDistance = data["RendererSettings"]["DepthOfFieldSettings"]["MinDistance"].as<float>();
	float dofMaxDistance = data["RendererSettings"]["DepthOfFieldSettings"]["MaxDistance"].as<float>();
	float dofSize = data["RendererSettings"]["DepthOfFieldSettings"]["Size"].as<int>();

	/* Setup Renderer settings */
	Renderer::RendererSettings settings;
	settings.DepthFogEnabled = depthFog;
	settings.PostProcessingEnabled = postProcessing;
	settings.FXAAEnabled = fxaa;
	settings.VignetteEnabled = vignette;
	settings.DepthOfFieldEnabled = depthOfField;
	renderer->SetSettings(settings);

	/* Setup SSAO settings */
	renderer->m_SSAOPass->m_Settings.Intensity = ssaoIntensity;
	renderer->m_SSAOPass->m_Settings.KernelSize = ssaoKernelSize;
	renderer->m_SSAOPass->m_Settings.Radius = ssaoRadius;
	renderer->m_SSAOPass->m_Settings.Bias = ssaoBias;

	/* Setup Depth Fog settings */
	renderer->m_DepthFogPass->m_Settings.MinDistance = dfMinDistance;
	renderer->m_DepthFogPass->m_Settings.MaxDistance = dfMaxDistance;
	renderer->m_DepthFogPass->m_Settings.Density = dfDensity;
	renderer->m_DepthFogPass->m_Settings.Color = dfColor;

	/* Setup Post Processing settings */
	renderer->m_PostProcessingPass->m_Settings.Gamma = gamma;
	renderer->m_PostProcessingPass->m_Settings.Gain = gain;
	renderer->m_PostProcessingPass->m_Settings.Lift = lift;
	renderer->m_PostProcessingPass->m_Settings.Offset = offset;
	renderer->m_PostProcessingPass->m_Settings.Exposure = exposure;
	renderer->m_PostProcessingPass->m_Settings.Contrast = contrast;
	renderer->m_PostProcessingPass->m_Settings.ContrastPivot = contrastPivot;
	renderer->m_PostProcessingPass->m_Settings.BloomEnabled = bloomEnabled;
	renderer->m_PostProcessingPass->m_Settings.BloomThreshold = bloomThreshold;
	renderer->m_PostProcessingPass->m_Settings.BloomLimit = bloomLimit;
	renderer->m_PostProcessingPass->m_Settings.BloomIntensity = bloomIntensity;
	renderer->m_PostProcessingPass->m_Settings.BloomBlurSigma = bloomBlurSigma;
	renderer->m_PostProcessingPass->m_Settings.Saturation = saturation;
	renderer->m_PostProcessingPass->m_Settings.Temperature = temperature;
	renderer->m_PostProcessingPass->m_Settings.Hue = hue;

	/* Setup Vignette settings */
	renderer->m_VignettePass->m_Settings.Intensity = vignetteIntensity;
	renderer->m_VignettePass->m_Settings.Size = vignetteSize;

	/* Setup Depth Of Field settings */
	renderer->m_DepthOfFieldPass->m_Settings.MinDistance = dofMinDistance;
	renderer->m_DepthOfFieldPass->m_Settings.MaxDistance = dofMaxDistance;
	renderer->m_DepthOfFieldPass->m_Settings.Size = dofSize;

#pragma endregion

	YAML::Node actors = data["Actors"];
	if (actors)
	{
		auto parentsIDs = std::vector<int64_t>();

		for (auto actor : actors)
		{
			if (actor["ID"].as<uint64_t>() == 0 || actor["ID"].as<uint64_t>() == 1)
				continue;

			Ref<Actor> a = Ref<Actor>();

			YAML::Node components = actor["Components"];
			if (components)
			{
				for (auto component : components)
				{
					if (auto transform = component["Transform"])
					{
						a = scene->AddActor(actor["ID"].as<uint64_t>(), actor["Name"].as<std::string>());

						if (auto dynamic = actor["Dynamic"])
							a->SetDynamic(dynamic.as<bool>());
						else
							a->SetDynamic(false);

						auto t = a->GetComponent<TransformComponent>();
						t->SetLocalPosition(transform["LocalPosition"].as<glm::vec3>());
						t->SetLocalRotation(transform["LocalRotation"].as<glm::vec3>());
						t->SetLocalScale(transform["LocalScale"].as<glm::vec3>());

						if (auto parent = transform["Parent"])
						{
							parentsIDs.push_back(parent.as<uint64_t>());
						}
					}
					if (auto rectTransform = component["RectTransform"])
					{
						a = scene->AddUIActor(actor["ID"].as<uint64_t>(), actor["Name"].as<std::string>());
						
						if (auto dynamic = actor["Dynamic"])
							a->SetDynamic(dynamic.as<bool>());
						else
							a->SetDynamic(false);

						auto t = a->GetComponent<RectTransformComponent>();
						t->SetAnchor((AnchorType)rectTransform["Anchor"].as<uint16_t>());
						t->SetLocalPosition(rectTransform["LocalPosition"].as<glm::vec3>());
						t->SetLocalRotation(rectTransform["LocalRotation"].as<glm::vec3>());
						t->SetLocalScale(rectTransform["LocalScale"].as<glm::vec3>());

						if (auto parent = rectTransform["Parent"])
						{
							parentsIDs.push_back(parent.as<uint64_t>());
						}
					}

					if (auto mesh = component["StaticMesh"])
					{
						std::string path = mesh["Mesh"].as<std::string>();
						std::vector<std::string> materialsPaths;
						YAML::Node materials = mesh["Materials"];
						for (auto material : materials)
						{
							materialsPaths.push_back(material["Path"].as<std::string>());
						}
						a->AddComponent<StaticMeshComponent>(path.c_str(), materialsPaths);
					}

					if (auto mesh = component["SkeletalMesh"])
					{
						std::string path = mesh["SkeletalMesh"].as<std::string>();
						std::vector<std::string> materialsPaths;
						YAML::Node materials = mesh["Materials"];
						for (auto material : materials)
						{
							materialsPaths.push_back(material["Path"].as<std::string>());
						}
						a->AddComponent<SkeletalMeshComponent>(path.c_str(), materialsPaths);
					}

					if (auto lodGroup = component["LODGroup"])
					{
						auto lodGroupComponent = a->AddComponent<LODGroupComponent>();

						YAML::Node lods = lodGroup["LODs"];
						for (auto lod : lods)
						{
							uint16_t level = lod["Level"].as<uint16_t>();
							float distance = lod["MaxDistance"].as<float>();
							std::string modelPath = lod["Model"].as<std::string>();
							std::vector<std::string> materialsPaths;
							YAML::Node materials = lod["Materials"];
							for (auto material : materials)
							{
								materialsPaths.push_back(material["Path"].as<std::string>());
							}

							LOD temp;
							temp.Level = level;
							temp.MaxDistance = distance;
							temp.Model = AssetManager::LoadModel(modelPath);

							for (auto materialPath : materialsPaths)
								temp.Materials.push_back(AssetManager::LoadMaterial(materialPath));

							lodGroupComponent->m_LODs.push_back(temp);
						}

						
					}

					if (auto dirLight = component["DirectionalLight"])
					{
						glm::vec3 color = dirLight["Color"].as<glm::vec3>();

						auto l = a->AddComponent<DirectionalLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->SetColor(color);
					}

					if (auto pointLight = component["PointLight"])
					{
						glm::vec3 color = pointLight["Color"].as<glm::vec3>();

						auto l = a->AddComponent<PointLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->SetColor(color);
					}

					if (auto spotLight = component["SpotLight"])
					{
						float innerCutOff = spotLight["InnerCutOff"].as<float>();
						float outerCutOff = spotLight["OuterCutOff"].as<float>();
						glm::vec3 color = spotLight["Color"].as<glm::vec3>();

						auto l = a->AddComponent<SpotLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->SetInnerCutOff(innerCutOff);
						l->SetOuterCutOff(outerCutOff);
						l->SetColor(color);
					}

					if (auto skyLight = component["SkyLight"])
					{
						std::vector<std::string> paths;
						auto p = skyLight["Paths"];
						uint16_t index = 0;
						for (auto path : p)
							paths.push_back(path.second.as<std::string>());

						glm::vec3 color = skyLight["Color"].as<glm::vec3>();
						float intensity = skyLight["Intensity"].as<float>();

						auto l = a->AddComponent<SkyLight>(paths);
						l->m_Color = color;
						l->m_Intensity = intensity;
					}

					if (auto particle = component["ParticleSystem"])
					{
						float duration = particle["Duration"].as<float>();
						bool looping = particle["Looping"].as<bool>();
						std::string spritePath = particle["Sprite"].as<std::string>();
						int emitterShape = particle["EmitterShape"].as<int>();
						uint32_t maxParticles = particle["MaxParticles"].as<uint32_t>();
						float minParticleSize = particle["MinParticleSize"].as<float>();
						float maxParticleSize = particle["MaxParticleSize"].as<float>();
						float endParticleSize = particle["EndParticleSize"].as<float>();
						glm::vec3 minParticleVelocity = particle["MinParticleVelocity"].as<glm::vec3>();
						glm::vec3 maxParticleVelocity = particle["MaxParticleVelocity"].as<glm::vec3>();
						glm::vec3 endParticleVelocity = particle["EndParticleVelocity"].as<glm::vec3>();
						float minParticleLifeTime = particle["MinParticleLifeTime"].as<float>();
						float maxParticleLifeTime = particle["MaxParticleLifeTime"].as<float>();
						glm::vec4 startParticleColor = particle["StartParticleColor"].as<glm::vec4>();
						glm::vec4 endParticleColor = particle["EndParticleColor"].as<glm::vec4>();

						auto p = a->AddComponent<ParticleSystemComponent>();
						p->m_Duration = duration;
						p->m_Looping = looping;
						p->m_Sprite = AssetManager::LoadTexture(spritePath);

						if (emitterShape == 0)
						{
							auto sphere = CreateRef<ParticleEmitterSphere>();
							sphere->m_Radius = particle["EmitterSphereRadius"].as<float>();
							p->m_EmitterShape = sphere;
						}
						else if (emitterShape == 1)
						{
							auto box = CreateRef<ParticleEmitterBox>();
							box->m_Size = particle["EmitterBoxSize"].as<glm::vec3>();
							p->m_EmitterShape = box;
						}
						else
							DEBUG_LOG("Unknown emitter shape in Particle System with ID: " + p->GetOwner()->GetID());

						p->m_MaxParticles = maxParticles;
						p->m_MinParticleSize = minParticleSize;
						p->m_MaxParticleSize = maxParticleSize;
						p->m_EndParticleSize = endParticleSize;
						p->m_EndParticleVelocity = minParticleVelocity;
						p->m_MaxParticleVelocity = maxParticleVelocity;
						p->m_EndParticleVelocity = endParticleVelocity;
						p->m_MinParticleLifeTime = minParticleLifeTime;
						p->m_MaxParticleLifeTime = maxParticleLifeTime;
						p->m_StartParticleColor = startParticleColor;
						p->m_EndParticleColor = endParticleColor;

						p->Reset();
					}

					if (auto camera = component["Camera"])
					{
						glm::vec2 aspectRatio = camera["AspectRatio"].as<glm::vec2>();
						float fieldOfView = camera["FieldOfView"].as<float>();
						float nearClipPlane = camera["NearClipPlane"].as<float>();
						float farClipPlane = camera["FarClipPlane"].as<float>();

						auto c = a->AddComponent<CameraComponent>();
						c->m_AspectRatio = aspectRatio;
						c->m_FieldOfView = fieldOfView;
						c->m_NearClipPlane = nearClipPlane;
						c->m_FarClipPlane = farClipPlane;
					}

					if (auto boxCollider = component["BoxCollider"])
					{
						bool isTrigger = boxCollider["IsTrigger"].as<bool>();
						glm::vec3 center = boxCollider["Center"].as<glm::vec3>();
						glm::vec3 size = boxCollider["Size"].as<glm::vec3>();

						auto b = a->AddComponent<BoxColliderComponent>();
						b->m_IsTrigger = isTrigger;
						b->m_Center = center;
						b->m_Size = size;
					}

					if (auto sphereCollider = component["SphereCollider"]) {
						bool isTrigger = sphereCollider["IsTrigger"].as<bool>();
                        glm::vec3 center = sphereCollider["Center"].as<glm::vec3>();
                        float size = sphereCollider["Size"].as<float>();

                        auto b = a->AddComponent<SphereColliderComponent>();
						b->m_IsTrigger = isTrigger;
                        b->m_Center = center;
                        b->m_Size = size;
                    }

					if (auto rigidBody = component["RigidBody"])
                    {
                        float gravity = rigidBody["Gravity"].as<float>();
                        float mass = rigidBody["Mass"].as<float>();
                        float drag = rigidBody["Drag"].as<float>();

                        auto r = a->AddComponent<RigidBodyComponent>();
                        r->m_Gravity = gravity;
                        r->m_Mass = mass;
                        r->m_Drag = drag;
                    }

					if (auto image = component["Image"])
					{
						std::string path = image["Path"].as<std::string>();
						glm::vec4 color = image["Color"].as<glm::vec4>();

						auto i = a->AddComponent<ImageComponent>();
						i->m_Image = AssetManager::LoadTexture(path);
						i->m_Color = color;
					}

					if (auto button = component["UIButton"])
					{
						bool enabled = button["Enabled"].as<bool>();
						std::string path = button["ImagePath"].as<std::string>();
						glm::vec4 normalColor = button["NormalColor"].as<glm::vec4>();
						glm::vec4 hoveredColor = button["HoveredColor"].as<glm::vec4>();
						glm::vec4 pressedColor = button["PressedColor"].as<glm::vec4>();
						glm::vec4 disabledColor = button["DisabledColor"].as<glm::vec4>();

						auto b = a->AddComponent<ButtonComponent>();
						b->m_Enabled = enabled;
						b->m_Image = AssetManager::LoadTexture(path);
						b->m_NormalColor = normalColor;
						b->m_HoveredColor = hoveredColor;
						b->m_PressedColor = pressedColor;
						b->m_DisabledColor = disabledColor;
					}

					/* --- GAME COMPONENTS --- */

					if (auto menu = component["MainMenu"])
					{
						uint64_t playButtonActorID = menu["PlayButton"].as<uint64_t>();
						uint64_t optionsButtonActorID = menu["OptionsButton"].as<uint64_t>();
						uint64_t exitButtonActorID = menu["ExitButton"].as<uint64_t>();

						auto m = a->AddComponent<MainMenu>();
						m->m_PlayButtonID = playButtonActorID;
						m->m_OptionsButtonID = optionsButtonActorID;
						m->m_ExitButtonID = exitButtonActorID;
					}

					if (auto player = component["Player"])
					{
						uint64_t cameraActorID = player["Camera"].as<uint64_t>();

						auto p = a->AddComponent<Player>();
						p->m_CameraID = cameraActorID;
					}

					if (auto button = component["Button"])
					{
						uint64_t platformActorID = button["Platform"].as<uint64_t>();

						auto b = a->AddComponent<Button>();
						b->m_PlatformID = platformActorID;
					}

					if (auto platform = component["Platform"])
					{
						glm::vec3 direction = platform["Direction"].as<glm::vec3>();
						float distance = platform["Distance"].as<float>();
						float speed = platform["Speed"].as<float>();

						auto p = a->AddComponent<Platform>();
						p->m_Direction = direction;
						p->m_Distance = distance;
						p->m_Speed = speed;
					}

					if (auto ghost = component["Ghost"])
					{
						uint64_t playerActorID = ghost["Player"].as<uint64_t>();

						auto g = a->AddComponent<Ghost>();
						g->m_PlayerID = playerActorID;
					}

					if (auto deathArea = component["DeathArea"])
					{
						a->AddComponent<DeathArea>();
					}

					if (auto checkpoint = component["Checkpoint"])
					{
						a->AddComponent<Checkpoint>();
					}
				}
			}
		}

		for (int i = 2; i < scene->GetActors().size(); i++)
		{
			scene->GetActors().at(i)->GetComponent<TransformBaseComponent>()->SetParent(scene->FindActor(parentsIDs[i - 2])->GetComponent<TransformBaseComponent>().get());
		}

		uint64_t currentCameraID = data["CurrentCamera"].as<uint64_t>();
		scene->m_CurrentCamera = scene->FindActor(currentCameraID)->GetComponent<CameraComponent>();
	}

	file.close();
	return scene;
}

void SceneSerializer::SerializeActor(YAML::Emitter& out, Ref<Actor> actor)
{
	// Root and UI Root ID's
	if (actor->GetID() == 0 || actor->GetID() == 1)
		return;

	out << YAML::BeginMap;
	out << YAML::Key << "Actor" << YAML::Value << actor->GetID();
	out << YAML::Key << "ID" << YAML::Value << actor->GetID();
	out << YAML::Key << "Name" << YAML::Value << actor->GetName();
	out << YAML::Key << "Dynamic" << YAML::Value << actor->IsDynamic();

	out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

	/* --- ENGINE COMPONENTS --- */

	if (auto transform = actor->GetComponent<TransformComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Transform";

		out << YAML::BeginMap;
		out << YAML::Key << "LocalPosition" << YAML::Value << transform->m_LocalPosition;
		out << YAML::Key << "LocalRotation" << YAML::Value << transform->m_LocalRotation;
		out << YAML::Key << "LocalScale" << YAML::Value << transform->m_LocalScale;

		if (transform->GetParent())
			out << YAML::Key << "Parent" << YAML::Value << transform->GetParent()->GetOwner()->GetID();

		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto rectTransform = actor->GetComponent<RectTransformComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "RectTransform";

		out << YAML::BeginMap;
		out << YAML::Key << "Anchor" << YAML::Value << (uint16_t)rectTransform->m_AnchorType;
		out << YAML::Key << "LocalPosition" << YAML::Value << rectTransform->m_LocalPosition;
		out << YAML::Key << "LocalRotation" << YAML::Value << rectTransform->m_LocalRotation;
		out << YAML::Key << "LocalScale" << YAML::Value << rectTransform->m_LocalScale;

		if (rectTransform->GetParent())
			out << YAML::Key << "Parent" << YAML::Value << rectTransform->GetParent()->GetOwner()->GetID();

		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto mesh = actor->GetComponent<StaticMeshComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "StaticMesh";

		out << YAML::BeginMap;
		out << YAML::Key << "Mesh" << YAML::Value << mesh->GetPath();
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < mesh->GetMaterialsPaths().size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Material" << YAML::Value << i;
			out << YAML::Key << "Path" << YAML::Value << mesh->GetMaterialsPaths().at(i);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto mesh = actor->GetComponent<SkeletalMeshComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SkeletalMesh";

		out << YAML::BeginMap;
		out << YAML::Key << "SkeletalMesh" << YAML::Value << mesh->GetPath();
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		//out << YAML::Key << "Bones" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < mesh->GetMaterialsPaths().size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Material" << YAML::Value << i;
			out << YAML::Key << "Path" << YAML::Value << mesh->GetMaterialsPaths().at(i);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto lodGroup = actor->GetComponent<LODGroupComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "LODGroup";
		out << YAML::BeginMap;
		out << YAML::Key << "LODs" << YAML::Value << YAML::BeginSeq;
		for (auto& lod : lodGroup->m_LODs)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Level" << YAML::Value << lod.Level;
			out << YAML::Key << "Model" << YAML::Value << lod.Model->GetPath();
			
			out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
			for (int i = 0; i < lod.Materials.size(); i++)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Material" << YAML::Value << i;
				out << YAML::Key << "Path" << YAML::Value << lod.Materials.at(i)->GetPath();
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;

			out << YAML::Key << "MaxDistance" << YAML::Value << lod.MaxDistance;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto dirLight = actor->GetComponent<DirectionalLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "DirectionalLight";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << dirLight->GetColor();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto pointLight = actor->GetComponent<PointLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "PointLight";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << pointLight->GetColor();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto spotLight = actor->GetComponent<SpotLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SpotLight";
		out << YAML::BeginMap;
		out << YAML::Key << "InnerCutOff" << YAML::Value << spotLight->GetInnerCutOff();
		out << YAML::Key << "OuterCutOff" << YAML::Value << spotLight->GetOuterCutOff();
		out << YAML::Key << "Color" << YAML::Value << spotLight->GetColor();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto skyLight = actor->GetComponent<SkyLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SkyLight";
		out << YAML::BeginMap;

		out << YAML::Key << "Paths" << YAML::Value << YAML::BeginMap;
		for (uint16_t i = 0; i < skyLight->GetPaths().size(); i++)
		{
			out << YAML::Key << i << YAML::Value << skyLight->GetPaths()[i];
		}
		out << YAML::EndMap;

		out << YAML::Key << "Color" << YAML::Value << skyLight->m_Color;
		out << YAML::Key << "Intensity" << YAML::Value << skyLight->m_Intensity;

		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto particleSystem = actor->GetComponent<ParticleSystemComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "ParticleSystem";
		out << YAML::BeginMap;
		out << YAML::Key << "Duration" << YAML::Value << particleSystem->m_Duration;
		out << YAML::Key << "Looping" << YAML::Value << particleSystem->m_Looping;
		out << YAML::Key << "Sprite" << YAML::Value << particleSystem->m_Sprite->GetPath();

		if (auto sphere = Cast<ParticleEmitterSphere>(particleSystem->m_EmitterShape))
		{
			out << YAML::Key << "EmitterShape" << YAML::Value << 0;
			out << YAML::Key << "EmitterSphereRadius" << YAML::Value << sphere->m_Radius;
		}
		else if (auto box = Cast<ParticleEmitterBox>(particleSystem->m_EmitterShape))
		{
			out << YAML::Key << "EmitterShape" << YAML::Value << 1;
			out << YAML::Key << "EmitterBoxSize" << YAML::Value << box->m_Size;
		}

		out << YAML::Key << "MaxParticles" << YAML::Value << particleSystem->m_MaxParticles;
		out << YAML::Key << "MinParticleSize" << YAML::Value << particleSystem->m_MinParticleSize;
		out << YAML::Key << "MaxParticleSize" << YAML::Value << particleSystem->m_MaxParticleSize;
		out << YAML::Key << "EndParticleSize" << YAML::Value << particleSystem->m_EndParticleSize;
		out << YAML::Key << "MinParticleVelocity" << YAML::Value << particleSystem->m_MinParticleVelocity;
		out << YAML::Key << "MaxParticleVelocity" << YAML::Value << particleSystem->m_MaxParticleVelocity;
		out << YAML::Key << "EndParticleVelocity" << YAML::Value << particleSystem->m_EndParticleVelocity;
		out << YAML::Key << "MinParticleLifeTime" << YAML::Value << particleSystem->m_MinParticleLifeTime;
		out << YAML::Key << "MaxParticleLifeTime" << YAML::Value << particleSystem->m_MaxParticleLifeTime;
		out << YAML::Key << "StartParticleColor" << YAML::Value << particleSystem->m_StartParticleColor;
		out << YAML::Key << "EndParticleColor" << YAML::Value << particleSystem->m_EndParticleColor;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto camera = actor->GetComponent<CameraComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Camera";
		out << YAML::BeginMap;
		out << YAML::Key << "AspectRatio" << YAML::Value << camera->m_AspectRatio;
		out << YAML::Key << "FieldOfView" << YAML::Value << camera->m_FieldOfView;
		out << YAML::Key << "NearClipPlane" << YAML::Value << camera->m_NearClipPlane;
		out << YAML::Key << "FarClipPlane" << YAML::Value << camera->m_FarClipPlane;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto boxCollider = actor->GetComponent<BoxColliderComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "BoxCollider";
		out << YAML::BeginMap;
		out << YAML::Key << "IsTrigger" << YAML::Value << boxCollider->m_IsTrigger;
		out << YAML::Key << "Center" << YAML::Value << boxCollider->m_Center;
		out << YAML::Key << "Size" << YAML::Value << boxCollider->m_Size;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto sphereCollider = actor->GetComponent<SphereColliderComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SphereCollider";
		out << YAML::BeginMap;
		out << YAML::Key << "IsTrigger" << YAML::Value << sphereCollider->m_IsTrigger;
		out << YAML::Key << "Center" << YAML::Value << sphereCollider->m_Center;
		out << YAML::Key << "Size" << YAML::Value << sphereCollider->m_Size;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto rigidBody = actor->GetComponent<RigidBodyComponent>())
    {
        out << YAML::BeginMap;
        out << YAML::Key << "RigidBody";
        out << YAML::BeginMap;
        out << YAML::Key << "Gravity" << YAML::Value << rigidBody->m_Gravity;
        out << YAML::Key << "Mass" << YAML::Value << rigidBody->m_Mass;
        out << YAML::Key << "Drag" << YAML::Value << rigidBody->m_Drag;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }
	
	if (auto image = actor->GetComponent<ImageComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Image";
		out << YAML::BeginMap;
		out << YAML::Key << "Path" << YAML::Value << image->m_Image->GetPath();
		out << YAML::Key << "Color" << YAML::Value << image->m_Color;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto button = actor->GetComponent<ButtonComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "UIButton";
		out << YAML::BeginMap;
		out << YAML::Key << "Enabled" << YAML::Value << button->m_Enabled;
		out << YAML::Key << "ImagePath" << YAML::Value << button->m_Image->GetPath();
		out << YAML::Key << "NormalColor" << YAML::Value << button->m_NormalColor;
		out << YAML::Key << "HoveredColor" << YAML::Value << button->m_HoveredColor;
		out << YAML::Key << "PressedColor" << YAML::Value << button->m_PressedColor;
		out << YAML::Key << "DisabledColor" << YAML::Value << button->m_DisabledColor;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	/* --- GAME COMPONENTS --- */

	if (auto menu = actor->GetComponent<MainMenu>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "MainMenu";
		out << YAML::BeginMap;
		out << YAML::Key << "PlayButton" << YAML::Value << menu->m_PlayButton->GetOwner()->GetID();
		out << YAML::Key << "OptionsButton" << YAML::Value << menu->m_OptionsButton->GetOwner()->GetID();
		out << YAML::Key << "ExitButton" << YAML::Value << menu->m_ExitButton->GetOwner()->GetID();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto player = actor->GetComponent<Player>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Player";
		out << YAML::BeginMap;
		out << YAML::Key << "Camera" << YAML::Value << player->m_Camera->GetOwner()->GetID();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto button = actor->GetComponent<Button>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Button";
		out << YAML::BeginMap;
		out << YAML::Key << "Platform" << YAML::Value << button->m_PlatformID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto platform = actor->GetComponent<Platform>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Platform";
		out << YAML::BeginMap;
		out << YAML::Key << "Direction" << YAML::Value << platform->m_Direction;
		out << YAML::Key << "Distance" << YAML::Value << platform->m_Distance;
		out << YAML::Key << "Speed" << YAML::Value << platform->m_Speed;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto ghost = actor->GetComponent<Ghost>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Ghost";
		out << YAML::BeginMap;
		out << YAML::Key << "Player" << YAML::Value << ghost->m_PlayerActor->GetID();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto deathArea = actor->GetComponent<DeathArea>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "DeathArea";
		out << YAML::BeginMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto checkpoint = actor->GetComponent<Checkpoint>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Checkpoint";
		out << YAML::BeginMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;
}
