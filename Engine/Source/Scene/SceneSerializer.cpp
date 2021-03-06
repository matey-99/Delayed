#include "SceneSerializer.h"

#include "Assets/AssetManager.h"

#include "yaml/yaml.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/FoliageComponent.h"
#include "Scene/Component/Animation/SkeletalMeshComponent.h"
#include "Scene/Component/Animation/Animator.h"
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
#include <Game/CameraOrbit.h>
#include <Game/Footsteps.h>
#include <Game/Intro.h>
#include <Game/Mushroom.h>
#include "Scene/Component/UI/ImageComponent.h"
#include "Scene/Component/UI/ButtonComponent.h"
#include "Scene/Component/UI/TextComponent.h"
#include "Scene/Component/UI/RectTransformComponent.h"
#include "Scene/Component/AudioSourceComponent.h"
#include "Scene/Component/AudioListenerComponent.h"

#include "Renderer/RenderPass/SSAOPass.h"
#include "Renderer/RenderPass/SSRPass.h"
#include "Renderer/RenderPass/DepthFogPass.h"
#include "Renderer/RenderPass/PostProcessingPass.h"
#include "Renderer/RenderPass/FXAAPass.h"
#include "Renderer/RenderPass/DepthOfFieldPass.h"

#include "Game/MainMenu.h"
#include "Game/Player.h"
#include "Game/CameraController.h"
#include "Game/Button.h"
#include "Game/Ghost.h"
#include "Game/DeathArea.h"
#include "Game/Checkpoint.h"
#include "Game/Trail.h"
#include "Game/BlockTrigger.h"
#include "Game/TPPPlayer.h"
#include "Game/PickableSkill.h"
#include "Game/Obelisks/Obelisk.h"
#include "Game/PostProcessingVolume.h"
#include "Game/GameManager.h"
#include "Game/NotificationManager.h"
#include "Game/TutorialManager.h"
#include "Game/SceneTransition.h"
#include "Game/Moving.h"
#include "Game/InteractionPanel.h"
#include "Game/PickableSpaceshipPart.h"
#include "Game/Spaceship.h"
#include "Game/SpaceshipPart.h"
#include "Game/TutorialTrigger.h"
#include "Game/ControllerBasedIcon.h"
#include "Game/PerspectiveController.h"

void SceneSerializer::Serialize(Ref<Scene> scene, std::string destinationPath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << scene->m_Name;
	out << YAML::Key << "CurrentCamera" << YAML::Value << scene->m_CurrentCamera->GetOwner()->GetID();

	auto renderer = Renderer::GetInstance();
	out << YAML::Key << "RendererSettings" << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "SSAO" << YAML::Value << renderer->GetSettings().SSAOEnabled;
	out << YAML::Key << "SSR" << YAML::Value << renderer->GetSettings().SSREnabled;
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

	out << YAML::Key << "SSRSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "MaxSteps" << YAML::Value << renderer->m_SSRPass->m_Settings.MaxSteps;
	out << YAML::Key << "MinRayStep" << YAML::Value << renderer->m_SSRPass->m_Settings.MinRayStep;
	out << YAML::Key << "RayStep" << YAML::Value << renderer->m_SSRPass->m_Settings.RayStep;
	out << YAML::Key << "NumBinarySearchSteps" << YAML::Value << renderer->m_SSRPass->m_Settings.NumBinarySearchSteps;
	out << YAML::Key << "ReflectionSpecularFalloffExponent" << YAML::Value << renderer->m_SSRPass->m_Settings.ReflectionSpecularFalloffExponent;
	out << YAML::EndMap;

	out << YAML::Key << "DepthFogSettings" << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Density" << YAML::Value << renderer->m_DepthFogPass->m_Settings.Density;
    out << YAML::Key << "Height" << YAML::Value << renderer->m_DepthFogPass->m_Settings.Height;
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
    out << YAML::Key << "AberrationEnabled" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.AberrationEnabled;
    out << YAML::Key << "AberrationShift" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.AberrationShift;
    out << YAML::Key << "FisheyeEnabled" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.FisheyeEnabled;
    out << YAML::Key << "Scale" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.Scale;
    out << YAML::Key << "VignetteEnabled" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.VignetteEnabled;
    out << YAML::Key << "VignetteColor" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.VignetteColor;
    out << YAML::Key << "VignetteIntensity" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.VignetteIntensity;
    out << YAML::Key << "VignetteSize" << YAML::Value << renderer->m_PostProcessingPass->m_Settings.VignetteSize;
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
	bool ssao = data["RendererSettings"]["SSAO"].as<bool>();
	bool ssr = data["RendererSettings"]["SSR"].as<bool>();
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

	/* SSR settings */
	float ssrMaxSteps = data["RendererSettings"]["SSRSettings"]["MaxSteps"].as<int>();
	float ssrMinRayStep = data["RendererSettings"]["SSRSettings"]["MinRayStep"].as<float>();
	float ssrRayStep = data["RendererSettings"]["SSRSettings"]["RayStep"].as<float>();
	float ssrReflectionSpecularFalloffExponent = data["RendererSettings"]["SSRSettings"]["ReflectionSpecularFalloffExponent"].as<float>();
	float ssrNumBinarySearchSteps = data["RendererSettings"]["SSRSettings"]["NumBinarySearchSteps"].as<int>();

	/* Depth Fog settings */
	float dfDensity = data["RendererSettings"]["DepthFogSettings"]["Density"].as<float>();
    float dfHeight = data["RendererSettings"]["DepthFogSettings"]["Height"].as<float>();
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
	bool aberration = data["RendererSettings"]["PostProcessingSettings"]["AberrationEnabled"].as<bool>();
    glm::vec3 aberrationShift = data["RendererSettings"]["PostProcessingSettings"]["AberrationShift"].as<glm::vec3>();
    bool fisheye = data["RendererSettings"]["PostProcessingSettings"]["FisheyeEnabled"].as<bool>();
    float scale = data["RendererSettings"]["PostProcessingSettings"]["Scale"].as<float>();
    bool vignetteEnabled = data["RendererSettings"]["PostProcessingSettings"]["VignetteEnabled"].as<bool>();
    glm::vec3 vignetteColor = data["RendererSettings"]["PostProcessingSettings"]["VignetteColor"].as<glm::vec3>();
    float vignetteIntensity = data["RendererSettings"]["PostProcessingSettings"]["VignetteIntensity"].as<float>();
    float vignetteSize = data["RendererSettings"]["PostProcessingSettings"]["VignetteSize"].as<float>();

	/* Depth Of Field settings */
	float dofMinDistance = data["RendererSettings"]["DepthOfFieldSettings"]["MinDistance"].as<float>();
	float dofMaxDistance = data["RendererSettings"]["DepthOfFieldSettings"]["MaxDistance"].as<float>();
	float dofSize = data["RendererSettings"]["DepthOfFieldSettings"]["Size"].as<int>();

	/* Setup Renderer settings */
	Renderer::RendererSettings settings;
	settings.SSAOEnabled = ssao;
	settings.SSREnabled = ssr;
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

	/* Setup SSR settings */
	renderer->m_SSRPass->m_Settings.MaxSteps = ssrMaxSteps;
	renderer->m_SSRPass->m_Settings.RayStep = ssrRayStep;
	renderer->m_SSRPass->m_Settings.MinRayStep = ssrMinRayStep;
	renderer->m_SSRPass->m_Settings.NumBinarySearchSteps = ssrNumBinarySearchSteps;
	renderer->m_SSRPass->m_Settings.ReflectionSpecularFalloffExponent = ssrReflectionSpecularFalloffExponent;

	/* Setup Depth Fog settings */
	renderer->m_DepthFogPass->m_Settings.Density = dfDensity;
	renderer->m_DepthFogPass->m_Settings.Height = dfHeight;
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
	renderer->m_PostProcessingPass->m_Settings.AberrationEnabled = aberration;
	renderer->m_PostProcessingPass->m_Settings.AberrationShift = aberrationShift;
    renderer->m_PostProcessingPass->m_Settings.FisheyeEnabled = fisheye;
    renderer->m_PostProcessingPass->m_Settings.Scale = scale;
    renderer->m_PostProcessingPass->m_Settings.VignetteEnabled = vignetteEnabled;
    renderer->m_PostProcessingPass->m_Settings.VignetteColor = vignetteColor;
    renderer->m_PostProcessingPass->m_Settings.VignetteIntensity = vignetteIntensity;
    renderer->m_PostProcessingPass->m_Settings.VignetteSize = vignetteSize;

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

						if (auto enabled = actor["Enabled"])
							a->SetEnabled(enabled.as<bool>());
						else
							a->SetEnabled(true);

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

						if (auto enabled = actor["Enabled"])
							a->SetEnabled(enabled.as<bool>());
						else
							a->SetEnabled(true);

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

						bool castShadow = mesh["CastShadow"].as<bool>();

						auto m = a->CreateComponent<StaticMeshComponent>(path.c_str(), materialsPaths);
						m->m_CastShadow = castShadow;
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

						bool castShadow = mesh["CastShadow"].as<bool>();

						auto m = a->CreateComponent<SkeletalMeshComponent>(path.c_str(), materialsPaths);
						m->m_CastShadow = castShadow;
					}

					if (auto animator = component["Animator"])
					{
						a->CreateComponent<Animator>();
					}

					if (auto foliage = component["Foliage"])
					{
						std::string path = foliage["Model"].as<std::string>();
						std::string materialPath = foliage["Material"].as<std::string>();
						uint32_t instancesCount = foliage["InstancesCount"].as<uint32_t>();
						float radius = foliage["Radius"].as<float>();
						float minScale = foliage["MinInstanceScale"].as<float>();
						float maxScale = foliage["MaxInstanceScale"].as<float>();
						uint64_t seed = foliage["Seed"].as<uint64_t>();
						bool castShadows = foliage["CastShadows"].as<bool>();

						auto f = a->CreateComponent<FoliageComponent>();
						f->ChangeMesh(path);
						f->ChangeMaterial(materialPath);
						f->m_InstancesCount = instancesCount;
						f->m_Radius = radius;
						f->m_MinInstanceScale = minScale;
						f->m_MaxInstanceScale = maxScale;
						f->m_Seed = seed;
						f->Generate();

						f->m_CastShadows = castShadows;
					}

					if (auto lodGroup = component["LODGroup"])
					{
						auto lodGroupComponent = a->CreateComponent<LODGroupComponent>();

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
						float intensity = dirLight["Intensity"].as<float>();

						auto l = a->CreateComponent<DirectionalLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->m_Color = color;
						l->m_Intensity = intensity;
					}

					if (auto pointLight = component["PointLight"])
					{
						glm::vec3 color = pointLight["Color"].as<glm::vec3>();
						float intensity = pointLight["Intensity"].as<float>();
						float radius = pointLight["Radius"].as<float>();
						float falloffExponent = pointLight["FalloffExponent"].as<float>();
						bool useInverseSquaredFaloff = pointLight["UseInverseSquaredFalloff"].as<bool>();

						auto l = a->CreateComponent<PointLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->m_Color = color;
						l->m_Intensity = intensity;
						l->m_Radius = radius;
						l->m_FalloffExponent = falloffExponent;
						l->m_UseInverseSquaredFalloff = useInverseSquaredFaloff;
					}

					if (auto spotLight = component["SpotLight"])
					{
						glm::vec3 color = spotLight["Color"].as<glm::vec3>();
						float intensity = spotLight["Intensity"].as<float>();
						float radius = spotLight["Radius"].as<float>();
						float falloffExponent = spotLight["FalloffExponent"].as<float>();
						bool useInverseSquaredFaloff = spotLight["UseInverseSquaredFalloff"].as<bool>();
						float innerCutOff = spotLight["InnerCutOff"].as<float>();
						float outerCutOff = spotLight["OuterCutOff"].as<float>();

						auto l = a->CreateComponent<SpotLight>(scene->m_LightsVertexUniformBuffer, scene->m_LightsFragmentUniformBuffer);
						l->m_Color = color;
						l->m_Intensity = intensity;
						l->m_Radius = radius;
						l->m_FalloffExponent = falloffExponent;
						l->m_UseInverseSquaredFalloff = useInverseSquaredFaloff;
						l->m_InnerCutOff = innerCutOff;
						l->m_OuterCutOff = outerCutOff;
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
						float weight = skyLight["Weight"].as<float>();

						auto l = a->CreateComponent<SkyLight>(paths);
						l->m_Color = color;
						l->m_Intensity = intensity;
						l->m_Weight = weight;
					}

					if (auto particle = component["ParticleSystem"])
					{
						float duration = particle["Duration"].as<float>();
						bool looping = particle["Looping"].as<bool>();
						std::string spritePath = particle["Sprite"].as<std::string>();
						int emitterShape = particle["EmitterShape"].as<int>();
						uint32_t maxParticles = particle["MaxParticles"].as<uint32_t>();
						float emissionRateOverTime = particle["EmissionRateOverTime"].as<float>();
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

						auto p = a->CreateComponent<ParticleSystemComponent>();
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
							ENGINE_WARN("Unknown emitter shape in Particle System with ID: " + p->GetOwner()->GetID());

						p->m_MaxParticles = maxParticles;
						p->m_EmissionRateOverTime = emissionRateOverTime;
						p->m_MinParticleSize = minParticleSize;
						p->m_MaxParticleSize = maxParticleSize;
						p->m_EndParticleSize = endParticleSize;
						p->m_MinParticleVelocity = minParticleVelocity;
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

						auto c = a->CreateComponent<CameraComponent>();
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

						auto b = a->CreateComponent<BoxColliderComponent>();
						b->m_IsTrigger = isTrigger;
						b->m_Center = center;
						b->m_Size = size;
					}

					if (auto sphereCollider = component["SphereCollider"]) {
						bool isTrigger = sphereCollider["IsTrigger"].as<bool>();
                        glm::vec3 center = sphereCollider["Center"].as<glm::vec3>();
                        float size = sphereCollider["Size"].as<float>();

                        auto b = a->CreateComponent<SphereColliderComponent>();
						b->m_IsTrigger = isTrigger;
                        b->m_Center = center;
                        b->m_Size = size;
                    }

					if (auto rigidBody = component["RigidBody"])
                    {
                        float gravity = rigidBody["Gravity"].as<float>();
                        float mass = rigidBody["Mass"].as<float>();
                        float drag = rigidBody["Drag"].as<float>();

                        auto r = a->CreateComponent<RigidBodyComponent>();
                        r->m_Gravity = gravity;
                        r->m_Mass = mass;
                        r->m_Drag = drag;
                    }

					if (auto image = component["Image"])
					{
						std::string path = image["Path"].as<std::string>();
						glm::vec4 color = image["Color"].as<glm::vec4>();

						auto i = a->CreateComponent<ImageComponent>();
						i->m_Image = AssetManager::LoadTexture(path);
						i->m_Color = color;
					}

					if (auto textComp = component["Text"])
					{
						std::string fontPath = textComp["Font"].as<std::string>();
						float fontSize = textComp["FontSize"].as<float>();
						std::string text = textComp["Text"].as<std::string>();
						glm::vec4 normalColor = textComp["NormalColor"].as<glm::vec4>();
						glm::vec4 hoveredColor = textComp["HoveredColor"].as<glm::vec4>();
						glm::vec4 pressedColor = textComp["PressedColor"].as<glm::vec4>();
						glm::vec4 disabledColor = textComp["DisabledColor"].as<glm::vec4>();

						auto t = a->CreateComponent<TextComponent>();
						t->m_Font = AssetManager::LoadFont(fontPath);
						t->m_FontSize = fontSize;
						t->m_Text = text;
						t->m_NormalColor = normalColor;
						t->m_HoveredColor = hoveredColor;
						t->m_PressedColor = pressedColor;
						t->m_DisabledColor = disabledColor;
					}

					if (auto button = component["UIButton"])
					{
						bool enabled = button["Enabled"].as<bool>();
						std::string path = button["ImagePath"].as<std::string>();
						glm::vec4 normalColor = button["NormalColor"].as<glm::vec4>();
						glm::vec4 hoveredColor = button["HoveredColor"].as<glm::vec4>();
						glm::vec4 pressedColor = button["PressedColor"].as<glm::vec4>();
						glm::vec4 disabledColor = button["DisabledColor"].as<glm::vec4>();

						auto b = a->CreateComponent<ButtonComponent>();
						b->m_Enabled = enabled;
						b->m_Image = AssetManager::LoadTexture(path);
						b->m_NormalColor = normalColor;
						b->m_HoveredColor = hoveredColor;
						b->m_PressedColor = pressedColor;
						b->m_DisabledColor = disabledColor;
					}

                    if (auto audioSource = component["AudioSource"]) {
                        std::string path = audioSource["SoundPath"].as<std::string>();
                        int channel = audioSource["Channel"].as<int>();
                        float volume = audioSource["Volume"].as<float>();
                        bool is3d = audioSource["3d"].as<bool>();
                        bool looping = audioSource["Looping"].as<bool>();
                        bool playOnStart = audioSource["PlayOnStart"].as<bool>();

                        auto as = a->CreateComponent<AudioSourceComponent>();
                        as->m_Sound = path;
                        as->m_Channel = static_cast<CHANNEL_GROUP>(channel);
                        as->m_Volume = volume;
                        as->m_3d = is3d;
                        as->m_Looping = looping;
                        as->m_PlayOnStart = playOnStart;
                    }

                    if (auto audioListener = component["AudioListener"]) {
                        a->CreateComponent<AudioListenerComponent>();
                    }

					/* --- GAME COMPONENTS --- */

					if (auto gameManager = component["GameManager"])
					{
						uint64_t mainMenuID = gameManager["MainMenu"].as<uint64_t>();

						auto g = a->CreateComponent<GameManager>();
						g->m_MainMenuID = mainMenuID;
					}

					if (auto notificationManager = component["NotificationManager"])
					{
						uint64_t notificationTextID = notificationManager["NotificationText"].as<uint64_t>();

						auto n = a->CreateComponent<NotificationManager>();
						n->m_NotificationTextID = notificationTextID;
					}

					if (auto tutorialManager = component["TutorialManager"])
					{
						uint64_t movementTutorialID = tutorialManager["MovementTutorial"].as<uint64_t>();
						uint64_t jumpTutorialID = tutorialManager["JumpTutorial"].as<uint64_t>();
						uint64_t sprintTutorialID = tutorialManager["SprintTutorial"].as<uint64_t>();
						uint64_t doubleJumpTutorialID = tutorialManager["DoubleJumpTutorial"].as<uint64_t>();
						uint64_t dashTutorialID = tutorialManager["DashTutorial"].as<uint64_t>();
						uint64_t teleportTutorialID = tutorialManager["TeleportTutorial"].as<uint64_t>();

						auto t = a->CreateComponent<TutorialManager>();
						t->m_MovementTutorialID = movementTutorialID;
						t->m_JumpTutorialID = jumpTutorialID;
						t->m_SprintTutorialID = sprintTutorialID;
						t->m_DoubleJumpTutorialID = doubleJumpTutorialID;
						t->m_DashTutorialID = dashTutorialID;
						t->m_TeleportTutorialID = teleportTutorialID;
					}

					if (auto perspective = component["PerspectiveController"])
					{
						uint64_t playerFPP = perspective["PlayerFPP"].as<uint64_t>();
						uint64_t cameraFPP = perspective["CameraFPP"].as<uint64_t>();
						uint64_t playerTPP = perspective["PlayerTPP"].as<uint64_t>();
						uint64_t cameraTPP = perspective["CameraTPP"].as<uint64_t>();
						uint64_t cameraController = perspective["CameraController"].as<uint64_t>();

						auto p = a->CreateComponent<PerspectiveController>();
						p->m_PlayerFPPID = playerFPP;
						p->m_CameraFPPID = cameraFPP;
						p->m_PlayerTPPID = playerTPP;
						p->m_CameraTPPID = cameraTPP;
						p->m_CameraControllerID = cameraController;
					}

					if (auto menu = component["MainMenu"])
					{
						uint64_t defaultPanelID = menu["DefaultPanel"].as<uint64_t>();
						uint64_t playButtonActorID = menu["PlayButton"].as<uint64_t>();
						uint64_t resetButtonActorID = menu["ResetButton"].as<uint64_t>();
						uint64_t optionsButtonActorID = menu["OptionsButton"].as<uint64_t>();
						uint64_t creditsButtonActorID = menu["CreditsButton"].as<uint64_t>();
						uint64_t exitButtonActorID = menu["ExitButton"].as<uint64_t>();

						uint64_t optionsPanelID = menu["OptionsPanel"].as<uint64_t>();
						uint64_t masterVolumeTextID = menu["MasterVolumeText"].as<uint64_t>();
						uint64_t masterVolumeSliderID = menu["MasterVolumeSlider"].as<uint64_t>();
						uint64_t increaseMasterVolumeButtonID = menu["IncreaseMasterVolumeButton"].as<uint64_t>();
						uint64_t decreaseMasterVolumeButtonID = menu["DecreaseMasterVolumeButton"].as<uint64_t>();
						uint64_t musicVolumeTextID = menu["MusicVolumeText"].as<uint64_t>();
						uint64_t musicVolumeSliderID = menu["MusicVolumeSlider"].as<uint64_t>();
						uint64_t increaseMusicVolumeButtonID = menu["IncreaseMusicVolumeButton"].as<uint64_t>();
						uint64_t decreaseMusicVolumeButtonID = menu["DecreaseMusicVolumeButton"].as<uint64_t>();
						uint64_t soundsVolumeTextID = menu["SoundsVolumeText"].as<uint64_t>();
						uint64_t soundsVolumeSliderID = menu["SoundsVolumeSlider"].as<uint64_t>();
						uint64_t increaseSoundsVolumeButtonID = menu["IncreaseSoundsVolumeButton"].as<uint64_t>();
						uint64_t decreaseSoundsVolumeButtonID = menu["DecreaseSoundsVolumeButton"].as<uint64_t>();
						uint64_t backFromOptionsButtonID = menu["BackFromOptionsButton"].as<uint64_t>();

						uint64_t creditsPanelID = menu["CreditsPanel"].as<uint64_t>();
						uint64_t backFromCreditsButtonID = menu["BackFromCreditsButton"].as<uint64_t>();


						auto m = a->CreateComponent<MainMenu>();
						m->m_DefaultPanelID = defaultPanelID;
						m->m_PlayButtonID = playButtonActorID;
						m->m_ResetButtonID = resetButtonActorID;
						m->m_OptionsButtonID = optionsButtonActorID;
						m->m_CreditsButtonID = creditsButtonActorID;
						m->m_ExitButtonID = exitButtonActorID;

						m->m_OptionsPanelID = optionsPanelID;
						m->m_MasterVolumeTextID = masterVolumeTextID;
						m->m_MasterVolumeSliderID = masterVolumeSliderID;
						m->m_IncreaseMasterVolumeButtonID = increaseMasterVolumeButtonID;
						m->m_DecreaseMasterVolumeButtonID = decreaseMasterVolumeButtonID;
						m->m_MusicVolumeTextID = musicVolumeTextID;
						m->m_MusicVolumeSliderID = musicVolumeSliderID;
						m->m_IncreaseMusicVolumeButtonID = increaseMusicVolumeButtonID;
						m->m_DecreaseMusicVolumeButtonID = decreaseMusicVolumeButtonID;
						m->m_SoundsVolumeTextID = soundsVolumeTextID;
						m->m_SoundsVolumeSliderID = soundsVolumeSliderID;
						m->m_IncreaseSoundsVolumeButtonID = increaseSoundsVolumeButtonID;
						m->m_DecreaseSoundsVolumeButtonID = decreaseSoundsVolumeButtonID;
						m->m_BackFromOptionsButtonID = backFromOptionsButtonID;

						m->m_CreditsPanelID = creditsPanelID;
						m->m_BackFromCreditsButtonID = backFromCreditsButtonID;
					}

					if (auto player = component["Player"])
					{
						uint64_t cameraActorID = player["Camera"].as<uint64_t>();
						uint64_t ghostActorID = player["Ghost"].as<uint64_t>();
						uint64_t trailActorID = player["Trail"].as<uint64_t>();
						uint64_t interactionPanelActorID = player["InteractionPanel"].as<uint64_t>();

						auto p = a->CreateComponent<Player>();
						p->m_CameraID = cameraActorID;
						p->m_GhostID = ghostActorID;
						p->m_TrailID = trailActorID;
						p->m_InteractionPanelID = interactionPanelActorID;
					}

					if (auto tppPlayer = component["TPPPlayer"])
					{
                        uint64_t cameraControllerID = tppPlayer["CameraController"].as<uint64_t>();
                        uint64_t ghostActorID = tppPlayer["Ghost"].as<uint64_t>();
                        uint64_t trailActorID = tppPlayer["Trail"].as<uint64_t>();
                        uint64_t interactionPanelActorID = tppPlayer["InteractionPanel"].as<uint64_t>();

						auto p = a->CreateComponent<TPPPlayer>();

                        p->m_CameraControllerID = cameraControllerID;
                        p->m_GhostID = ghostActorID;
                        p->m_TrailID = trailActorID;
                        p->m_InteractionPanelID = interactionPanelActorID;
					}

					if (auto panel = component["InteractionPanel"])
					{
						auto p = a->CreateComponent<InteractionPanel>();
					}

					if (auto camera = component["CameraController"])
					{
						uint64_t targetID = camera["Target"].as<uint64_t>();
						uint64_t cameraID = camera["Camera"].as<uint64_t>();

						auto c = a->CreateComponent<CameraController>();
						c->m_TargetID = targetID;
						c->m_CameraID = cameraID;
					}

					if (auto button = component["Button"])
					{
						uint64_t platformActorID = button["Platform"].as<uint64_t>();
						float platformDelayTime = button["PlatformDelayTime"].as<float>();

						std::vector<uint64_t> connectedButtonsIDs;
						YAML::Node connectedButtons = button["ConnectedButtons"];
						for (auto button : connectedButtons)
						{
							connectedButtonsIDs.push_back(button["ID"].as<uint64_t>());
						}

						auto b = a->CreateComponent<Button>();
						b->m_PlatformDelayTime = platformDelayTime;
						b->m_PlatformID = platformActorID;
						b->m_ConnectedButtonsIDs = connectedButtonsIDs;
					}

					if (auto platform = component["Platform"])
					{
						glm::vec3 direction = platform["Direction"].as<glm::vec3>();
						float distance = platform["Distance"].as<float>();
						float speed = platform["Speed"].as<float>();

						auto p = a->CreateComponent<Platform>();
						p->m_Direction = direction;
						p->m_Distance = distance;
						p->m_Speed = speed;
					}

					if (auto moving = component["Moving"])
					{
						glm::vec3 direction = moving["Direction"].as<glm::vec3>();
						float distance = moving["Distance"].as<float>();
						float smoothness = moving["Smoothness"].as<float>();
						float error = moving["ErrorInReachingTarget"].as<float>();

						auto m = a->CreateComponent<Moving>();
						m->m_Direction = direction;
						m->m_Distance = distance;
						m->m_Smoothness = smoothness;
						m->m_ErrorInReachingTarget = error;
					}

					if (auto ghost = component["Ghost"])
					{
						uint64_t playerActorID = ghost["Player"].as<uint64_t>();

						auto g = a->CreateComponent<Ghost>();
						g->m_PlayerID = playerActorID;
					}

					if (auto deathArea = component["DeathArea"])
					{
						a->CreateComponent<DeathArea>();
					}

					if (auto checkpoint = component["Checkpoint"])
					{
						a->CreateComponent<Checkpoint>();
					}

					if (auto blockTrigger = component["BlockTrigger"])
					{
                        auto ghostMaterial = blockTrigger["GhostMaterial"].as<std::string>();
                        auto solidMaterial = blockTrigger["SolidMaterial"].as<std::string>();

						auto bt = a->CreateComponent<BlockTrigger>();

                        bt->m_GhostMaterial = AssetManager::LoadMaterial(ghostMaterial);
                        bt->m_SolidMaterial = AssetManager::LoadMaterial(solidMaterial);
					}

					if (auto controllerBasedIcon = component["ControllerBasedIcon"])
					{
						auto keyboardIcon = controllerBasedIcon["KeyboardIcon"].as<std::string>();
						auto gamepadIcon = controllerBasedIcon["GamepadIcon"].as<std::string>();

						auto c = a->CreateComponent<ControllerBasedIcon>();

						c->m_KeyboardIcon = AssetManager::LoadTexture(keyboardIcon);
						c->m_GamepadIcon = AssetManager::LoadTexture(gamepadIcon);
					}

					if (auto trail = component["Trail"])
					{
						a->CreateComponent<Trail>();
					}

					if (auto skill = component["PickableSkill"])
					{
						int skillType = skill["SkillType"].as<int>();

						auto s = a->CreateComponent<PickableSkill>();
						s->m_SkillType = (SkillType)skillType;
					}

					if (auto tutorial = component["TutorialTrigger"])
					{
						int type = tutorial["Type"].as<int>();

						auto t = a->CreateComponent<TutorialTrigger>();
						t->m_Type = (TutorialType)type;
					}

					if (auto spaceship = component["Spaceship"])
					{
						a->CreateComponent<Spaceship>();
					}

					if (auto spaceshipPart = component["SpaceshipPart"])
					{
						int type = spaceshipPart["Type"].as<int>();
						std::string material = spaceshipPart["Material"].as<std::string>();

						auto s = a->CreateComponent<SpaceshipPart>();
						s->m_Type = (SpaceshipPartType)type;
						s->m_Material = AssetManager::LoadMaterial(material);
					}

					if (auto spaceshipPart = component["PickableSpaceshipPart"])
					{
						int part = spaceshipPart["SpaceshipPart"].as<int>();

						auto s = a->CreateComponent<PickableSpaceshipPart>();
						s->m_SpaceshipPart = (SpaceshipPartType)part;
					}

					if (auto obelisk = component["Obelisk"])
					{
						int effect = obelisk["Effect"].as<int>();
						uint64_t postFXID = obelisk["PostFX"].as<uint64_t>();

						auto o = a->CreateComponent<Obelisk>();
						o->m_Effect = (ObeliskEffect)effect;
						o->m_PostFXID = postFXID;
					}

					if (auto postFX = component["PostProcessingVolume"])
					{
						PostProcessingPass::PostProcessingSettings settings;
						settings.Gamma = postFX["Gamma"].as<float>();
						settings.Gain = postFX["Gain"].as<float>();
						settings.Lift = postFX["Lift"].as<float>();
						settings.Offset = postFX["Offset"].as<float>();
						settings.Exposure = postFX["Exposure"].as<float>();
						settings.Contrast = postFX["Contrast"].as<float>();
						settings.ContrastPivot = postFX["ContrastPivot"].as<float>();
						settings.BloomEnabled = postFX["BloomEnabled"].as<bool>();
						settings.BloomThreshold = postFX["BloomThreshold"].as<float>();
						settings.BloomLimit = postFX["BloomLimit"].as<float>();
						settings.BloomIntensity = postFX["BloomIntensity"].as<float>();
						settings.BloomBlurSigma = postFX["BloomBlurSigma"].as<float>();
						settings.Saturation = postFX["Saturation"].as<float>();
						settings.Temperature = postFX["Temperature"].as<float>();
						settings.Hue = postFX["Hue"].as<float>();

						auto comp = a->CreateComponent<PostProcessingVolume>();
						comp->m_Settings = settings;
					}

                    if (auto cameraOrbit = component["CameraOrbit"]) {
                        float speed = cameraOrbit["Speed"].as<float>();

                        auto cam = a->CreateComponent<CameraOrbit>();

                        cam->m_Speed = speed;
                    }

                    if (auto sceneTransition = component["SceneTransition"]) {
                        std::string scene = sceneTransition["Scene"].as<std::string>();

                        auto tr = a->CreateComponent<SceneTransition>();

                        tr->m_Scene = scene;
                    }

                    if (auto footsteps = component["Footsteps"]) {
                        uint32_t player = footsteps["PlayerID"].as<uint32_t>();
                        std::vector<std::string> sounds = std::vector<std::string>();
                        std::string name;
                        for (int i = 0; i < 12; ++i) {
                            name = "Sound"+std::to_string(i);
                            sounds.push_back(footsteps[name].as<std::string>());
                        }

                        auto fs = a->CreateComponent<Footsteps>();

                        fs->m_PlayerID = player;
                        fs->m_Sounds = sounds;
                    }

                    if (auto intro = component["Intro"]) {
                        uint32_t fadeui = intro["FadeUIID"].as<uint32_t>();
                        uint32_t spaceship = intro["SpaceshipID"].as<uint32_t>();
                        uint32_t meteor = intro["MeteorID"].as<uint32_t>();
                        uint32_t planet = intro["PlanetID"].as<uint32_t>();
                        uint32_t thruster = intro["ThrusterID"].as<uint32_t>();

                        auto i = a->CreateComponent<Intro>();

                        i->m_FadeUIID = fadeui;
                        i->m_SpaceshipID = spaceship;
                        i->m_MeteorID = meteor;
                        i->m_PlanetID = planet;
                        i->m_ThrusterID = thruster;
                    }

                    if (auto mushroom = component["Mushroom"]) {

                        auto path = mushroom["Path"].as<int>();

                        auto mush = a->CreateComponent<Mushroom>();

                        mush->m_Path = path;
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
	out << YAML::Key << "Enabled" << YAML::Value << actor->IsEnabled();
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
		out << YAML::Key << "CastShadow" << YAML::Value << mesh->m_CastShadow;
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
		out << YAML::Key << "CastShadow" << YAML::Value << mesh->m_CastShadow;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto animator = actor->GetComponent<Animator>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Animator";
		out << YAML::BeginMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto foliage = actor->GetComponent<FoliageComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Foliage";
		out << YAML::BeginMap;
		out << YAML::Key << "Model" << YAML::Value << foliage->m_Path;
		out << YAML::Key << "Material" << YAML::Value << foliage->m_MaterialPath;
		out << YAML::Key << "InstancesCount" << YAML::Value << foliage->m_InstancesCount;
		out << YAML::Key << "Radius" << YAML::Value << foliage->m_Radius;
		out << YAML::Key << "MinInstanceScale" << YAML::Value << foliage->m_MinInstanceScale;
		out << YAML::Key << "MaxInstanceScale" << YAML::Value << foliage->m_MaxInstanceScale;
		out << YAML::Key << "Seed" << YAML::Value << foliage->m_Seed;
		out << YAML::Key << "CastShadows" << YAML::Value << foliage->m_CastShadows;
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
		out << YAML::Key << "Color" << YAML::Value << dirLight->m_Color;
		out << YAML::Key << "Intensity" << YAML::Value << dirLight->m_Intensity;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto pointLight = actor->GetComponent<PointLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "PointLight";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << pointLight->m_Color;
		out << YAML::Key << "Intensity" << YAML::Value << pointLight->m_Intensity;
		out << YAML::Key << "Radius" << YAML::Value << pointLight->m_Radius;
		out << YAML::Key << "FalloffExponent" << YAML::Value << pointLight->m_FalloffExponent;
		out << YAML::Key << "UseInverseSquaredFalloff" << YAML::Value << pointLight->m_UseInverseSquaredFalloff;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	if (auto spotLight = actor->GetComponent<SpotLight>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SpotLight";
		out << YAML::BeginMap;
		out << YAML::Key << "Color" << YAML::Value << spotLight->m_Color;
		out << YAML::Key << "Intensity" << YAML::Value << spotLight->m_Intensity;
		out << YAML::Key << "Radius" << YAML::Value << spotLight->m_Radius;
		out << YAML::Key << "FalloffExponent" << YAML::Value << spotLight->m_FalloffExponent;
		out << YAML::Key << "UseInverseSquaredFalloff" << YAML::Value << spotLight->m_UseInverseSquaredFalloff;
		out << YAML::Key << "InnerCutOff" << YAML::Value << spotLight->m_InnerCutOff;
		out << YAML::Key << "OuterCutOff" << YAML::Value << spotLight->m_OuterCutOff;
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
		out << YAML::Key << "Weight" << YAML::Value << skyLight->m_Weight;

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
		out << YAML::Key << "EmissionRateOverTime" << YAML::Value << particleSystem->m_EmissionRateOverTime;
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

	if (auto textComp = actor->GetComponent<TextComponent>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Text";
		out << YAML::BeginMap;
		out << YAML::Key << "Font" << YAML::Value << textComp->m_Font->GetPath();
		out << YAML::Key << "FontSize" << YAML::Value << textComp->m_FontSize;
		out << YAML::Key << "Text" << YAML::Value << textComp->m_Text;
		out << YAML::Key << "NormalColor" << YAML::Value << textComp->m_NormalColor;
		out << YAML::Key << "HoveredColor" << YAML::Value << textComp->m_HoveredColor;
		out << YAML::Key << "PressedColor" << YAML::Value << textComp->m_PressedColor;
		out << YAML::Key << "DisabledColor" << YAML::Value << textComp->m_DisabledColor;
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

    if (auto audioSource = actor->GetComponent<AudioSourceComponent>()) {
        out << YAML::BeginMap;
        out << YAML::Key << "AudioSource";
        out << YAML::BeginMap;
        out << YAML::Key << "SoundPath" << YAML::Value << audioSource->m_Sound;
        out << YAML::Key << "Channel" << YAML::Value << static_cast<int>(audioSource->m_Channel);
        out << YAML::Key << "Volume" << YAML::Value << audioSource->m_Volume;
        out << YAML::Key << "3d" << YAML::Value << audioSource->m_3d;
        out << YAML::Key << "Looping" << YAML::Value << audioSource->m_Looping;
        out << YAML::Key << "PlayOnStart" << YAML::Value << audioSource->m_PlayOnStart;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    if (auto audioListener = actor->GetComponent<AudioListenerComponent>()) {
        out << YAML::BeginMap;
        out << YAML::Key << "AudioListener";
        out << YAML::BeginMap;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

	/* --- GAME COMPONENTS --- */

	if (auto menu = actor->GetComponent<MainMenu>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "MainMenu";
		out << YAML::BeginMap;
		out << YAML::Key << "DefaultPanel" << YAML::Value << menu->m_DefaultPanelID;
		out << YAML::Key << "PlayButton" << YAML::Value << menu->m_PlayButtonID;
		out << YAML::Key << "ResetButton" << YAML::Value << menu->m_ResetButtonID;
		out << YAML::Key << "OptionsButton" << YAML::Value << menu->m_OptionsButtonID;
		out << YAML::Key << "CreditsButton" << YAML::Value << menu->m_CreditsButtonID;
		out << YAML::Key << "ExitButton" << YAML::Value << menu->m_ExitButtonID;

		out << YAML::Key << "OptionsPanel" << YAML::Value << menu->m_OptionsPanelID;
		out << YAML::Key << "MasterVolumeText" << YAML::Value << menu->m_MasterVolumeTextID;
		out << YAML::Key << "MasterVolumeSlider" << YAML::Value << menu->m_MasterVolumeSliderID;
		out << YAML::Key << "IncreaseMasterVolumeButton" << YAML::Value << menu->m_IncreaseMasterVolumeButtonID;
		out << YAML::Key << "DecreaseMasterVolumeButton" << YAML::Value << menu->m_DecreaseMasterVolumeButtonID;
		out << YAML::Key << "MusicVolumeText" << YAML::Value << menu->m_MusicVolumeTextID;
		out << YAML::Key << "MusicVolumeSlider" << YAML::Value << menu->m_MusicVolumeSliderID;
		out << YAML::Key << "IncreaseMusicVolumeButton" << YAML::Value << menu->m_IncreaseMusicVolumeButtonID;
		out << YAML::Key << "DecreaseMusicVolumeButton" << YAML::Value << menu->m_DecreaseMusicVolumeButtonID;
		out << YAML::Key << "SoundsVolumeText" << YAML::Value << menu->m_SoundsVolumeTextID;
		out << YAML::Key << "SoundsVolumeSlider" << YAML::Value << menu->m_SoundsVolumeSliderID;
		out << YAML::Key << "IncreaseSoundsVolumeButton" << YAML::Value << menu->m_IncreaseSoundsVolumeButtonID;
		out << YAML::Key << "DecreaseSoundsVolumeButton" << YAML::Value << menu->m_DecreaseSoundsVolumeButtonID;
		out << YAML::Key << "BackFromOptionsButton" << YAML::Value << menu->m_BackFromOptionsButtonID;

		out << YAML::Key << "CreditsPanel" << YAML::Value << menu->m_CreditsPanelID;
		out << YAML::Key << "BackFromCreditsButton" << YAML::Value << menu->m_BackFromCreditsButtonID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto tppPlayer = actor->GetComponent<TPPPlayer>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "TPPPlayer";
		out << YAML::BeginMap;
        out << YAML::Key << "CameraController" << YAML::Value << tppPlayer->m_CameraControllerID;
        out << YAML::Key << "Ghost" << YAML::Value << tppPlayer->m_GhostID;
        out << YAML::Key << "Trail" << YAML::Value << tppPlayer->m_TrailID;
        out << YAML::Key << "InteractionPanel" << YAML::Value << tppPlayer->m_InteractionPanelID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	} else if (auto player = actor->GetComponent<Player>())
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Player";
        out << YAML::BeginMap;
        out << YAML::Key << "Camera" << YAML::Value << player->m_CameraID;
        out << YAML::Key << "Ghost" << YAML::Value << player->m_GhostID;
        out << YAML::Key << "Trail" << YAML::Value << player->m_TrailID;
        out << YAML::Key << "InteractionPanel" << YAML::Value << player->m_InteractionPanelID;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

	if (auto panel = actor->GetComponent<InteractionPanel>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "InteractionPanel";
		out << YAML::BeginMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto camera = actor->GetComponent<CameraController>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "CameraController";
		out << YAML::BeginMap;
		out << YAML::Key << "Target" << YAML::Value << camera->m_TargetID;
		out << YAML::Key << "Camera" << YAML::Value << camera->m_CameraID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto button = actor->GetComponent<Button>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Button";
		out << YAML::BeginMap;
		out << YAML::Key << "Platform" << YAML::Value << button->m_PlatformID;
		out << YAML::Key << "PlatformDelayTime" << YAML::Value << button->m_PlatformDelayTime;
		out << YAML::Key << "ConnectedButtons" << YAML::Value << YAML::BeginSeq;
		for (int i = 0; i < button->m_ConnectedButtonsIDs.size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "ID" << YAML::Value << button->m_ConnectedButtonsIDs[i];
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
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

	if (auto moving = actor->GetComponent<Moving>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Moving";
		out << YAML::BeginMap;
		out << YAML::Key << "Direction" << YAML::Value << moving->m_Direction;
		out << YAML::Key << "Distance" << YAML::Value << moving->m_Distance;
		out << YAML::Key << "Smoothness" << YAML::Value << moving->m_Smoothness;
		out << YAML::Key << "ErrorInReachingTarget" << YAML::Value << moving->m_ErrorInReachingTarget;
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

	if (auto gameManager = actor->GetComponent<GameManager>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "GameManager";
		out << YAML::BeginMap;
		out << YAML::Key << "MainMenu" << YAML::Value << gameManager->m_MainMenuID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto notificationManager = actor->GetComponent<NotificationManager>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "NotificationManager";
		out << YAML::BeginMap;
		out << YAML::Key << "NotificationText" << YAML::Value << notificationManager->m_NotificationTextID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto tutorialManager = actor->GetComponent<TutorialManager>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "TutorialManager";
		out << YAML::BeginMap;
		out << YAML::Key << "MovementTutorial" << YAML::Value << tutorialManager->m_MovementTutorialID;
		out << YAML::Key << "JumpTutorial" << YAML::Value << tutorialManager->m_JumpTutorialID;
		out << YAML::Key << "SprintTutorial" << YAML::Value << tutorialManager->m_SprintTutorialID;
		out << YAML::Key << "DoubleJumpTutorial" << YAML::Value << tutorialManager->m_DoubleJumpTutorialID;
		out << YAML::Key << "DashTutorial" << YAML::Value << tutorialManager->m_DashTutorialID;
		out << YAML::Key << "TeleportTutorial" << YAML::Value << tutorialManager->m_TeleportTutorialID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto perspective = actor->GetComponent<PerspectiveController>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "PerspectiveController";
		out << YAML::BeginMap;
		out << YAML::Key << "PlayerFPP" << YAML::Value << perspective->m_PlayerFPPID;
		out << YAML::Key << "CameraFPP" << YAML::Value << perspective->m_CameraFPPID;
		out << YAML::Key << "PlayerTPP" << YAML::Value << perspective->m_PlayerTPPID;
		out << YAML::Key << "CameraTPP" << YAML::Value << perspective->m_CameraTPPID;
		out << YAML::Key << "CameraController" << YAML::Value << perspective->m_CameraControllerID;
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

	if (auto blockTrigger = actor->GetComponent<BlockTrigger>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "BlockTrigger";
		out << YAML::BeginMap;
        out << YAML::Key << "GhostMaterial" << YAML::Value << blockTrigger->m_GhostMaterial->GetPath();
        out << YAML::Key << "SolidMaterial" << YAML::Value << blockTrigger->m_SolidMaterial->GetPath();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto controllerBasedIcon = actor->GetComponent<ControllerBasedIcon>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "ControllerBasedIcon";
		out << YAML::BeginMap;
		out << YAML::Key << "KeyboardIcon" << YAML::Value << controllerBasedIcon->m_KeyboardIcon->GetPath();
		out << YAML::Key << "GamepadIcon" << YAML::Value << controllerBasedIcon->m_GamepadIcon->GetPath();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto trail = actor->GetComponent<Trail>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Trail";
		out << YAML::BeginMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto skill = actor->GetComponent<PickableSkill>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "PickableSkill";
		out << YAML::BeginMap;
		out << YAML::Key << "SkillType" << YAML::Value << (int)skill->m_SkillType;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto tutorial = actor->GetComponent<TutorialTrigger>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "TutorialTrigger";
		out << YAML::BeginMap;
		out << YAML::Key << "Type" << YAML::Value << (int)tutorial->m_Type;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto spaceship = actor->GetComponent<Spaceship>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Spaceship";
		out << YAML::BeginMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto spaceshipPart = actor->GetComponent<SpaceshipPart>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SpaceshipPart";
		out << YAML::BeginMap;
		out << YAML::Key << "Type" << YAML::Value << (int)spaceshipPart->m_Type;
		out << YAML::Key << "Material" << YAML::Value << spaceshipPart->m_Material->GetPath();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto pickableSpaceshipPart = actor->GetComponent<PickableSpaceshipPart>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "PickableSpaceshipPart";
		out << YAML::BeginMap;
		out << YAML::Key << "SpaceshipPart" << YAML::Value << (int)pickableSpaceshipPart->m_SpaceshipPart;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto obelisk = actor->GetComponent<Obelisk>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Obelisk";
		out << YAML::BeginMap;
		out << YAML::Key << "Effect" << YAML::Value << (int)obelisk->m_Effect;
		out << YAML::Key << "PostFX" << YAML::Value << obelisk->m_PostFXID;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	if (auto postFX = actor->GetComponent<PostProcessingVolume>())
	{
		out << YAML::BeginMap;
		out << YAML::Key << "PostProcessingVolume";
		out << YAML::BeginMap;
		out << YAML::Key << "Gamma" << YAML::Value << postFX->m_Settings.Gamma;
		out << YAML::Key << "Gain" << YAML::Value << postFX->m_Settings.Gain;
		out << YAML::Key << "Lift" << YAML::Value << postFX->m_Settings.Lift;
		out << YAML::Key << "Offset" << YAML::Value << postFX->m_Settings.Offset;
		out << YAML::Key << "Exposure" << YAML::Value << postFX->m_Settings.Exposure;
		out << YAML::Key << "Contrast" << YAML::Value << postFX->m_Settings.Contrast;
		out << YAML::Key << "ContrastPivot" << YAML::Value << postFX->m_Settings.ContrastPivot;
		out << YAML::Key << "BloomEnabled" << YAML::Value << postFX->m_Settings.BloomEnabled;
		out << YAML::Key << "BloomThreshold" << YAML::Value << postFX->m_Settings.BloomThreshold;
		out << YAML::Key << "BloomLimit" << YAML::Value << postFX->m_Settings.BloomLimit;
		out << YAML::Key << "BloomIntensity" << YAML::Value << postFX->m_Settings.BloomIntensity;
		out << YAML::Key << "BloomBlurSigma" << YAML::Value << postFX->m_Settings.BloomBlurSigma;
		out << YAML::Key << "Saturation" << YAML::Value << postFX->m_Settings.Saturation;
		out << YAML::Key << "Temperature" << YAML::Value << postFX->m_Settings.Temperature;
		out << YAML::Key << "Hue" << YAML::Value << postFX->m_Settings.Hue;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

    if (auto cameraOrbit = actor->GetComponent<CameraOrbit>())
    {
        out << YAML::BeginMap;
        out << YAML::Key << "CameraOrbit";
        out << YAML::BeginMap;
        out << YAML::Key << "Speed" << YAML::Value << cameraOrbit->m_Speed;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    if (auto sceneTransition = actor->GetComponent<SceneTransition>()) {
        out << YAML::BeginMap;
        out << YAML::Key << "SceneTransition";
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << sceneTransition->m_Scene;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    if (auto footsteps = actor->GetComponent<Footsteps>())
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Footsteps";
        out << YAML::BeginMap;
        out << YAML::Key << "PlayerID" << YAML::Value << footsteps->m_PlayerID;
        std::string name;
        for (int i = 0; i < 12; ++i) {
            name = "Sound" + std::to_string(i);
            out << YAML::Key << name << YAML::Value << footsteps->m_Sounds.at(i);
        }
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    if (auto intro = actor->GetComponent<Intro>()) {
        out << YAML::BeginMap;
        out << YAML::Key << "Intro";
        out << YAML::BeginMap;
        out << YAML::Key << "FadeUIID" << YAML::Value << intro->m_FadeUIID;
        out << YAML::Key << "SpaceshipID" << YAML::Value << intro->m_SpaceshipID;
        out << YAML::Key << "MeteorID" << YAML::Value << intro->m_MeteorID;
        out << YAML::Key << "PlanetID" << YAML::Value << intro->m_PlanetID;
        out << YAML::Key << "ThrusterID" << YAML::Value << intro->m_ThrusterID;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    if (auto mushroom = actor->GetComponent<Mushroom>())
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Mushroom";
        out << YAML::BeginMap;
        out << YAML::Key << "Path" << YAML::Value << mushroom->m_Path;
        out << YAML::EndMap;
        out << YAML::EndMap;
    }

	out << YAML::EndSeq;
	out << YAML::EndMap;
}
