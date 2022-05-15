#pragma once

#include <vector>
#include <string>

#include "Scene/Component/RenderComponent.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderTarget.h"
#include "typedefs.h"

class SkyLight : public RenderComponent
{
private:
	uint32_t m_ID;
	uint32_t m_IrradianceMap;
	uint32_t m_PrefilterMap;
	
	Ref<Texture> m_BRDF;

	uint32_t m_CaptureFBO;

	std::vector<std::string> m_Paths;
	bool m_SkyVisibility;
	glm::vec3 m_Color;
	float m_Intensity;
	float m_Weight;

	Ref<Shader> m_Shader;

public:
	SkyLight(Actor* owner);
	SkyLight(Actor* owner, std::vector<std::string> paths);
	~SkyLight();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render(Material::BlendMode blendMode) override;
	virtual void Destroy() override;

	void Load(std::vector<std::string> paths);

	inline std::vector<std::string> GetPaths() const { return m_Paths; }
	inline float GetIntensity() const { return m_Intensity; }
	inline float GetWeight() const { return m_Weight; }
	inline glm::vec3 GetColor() const { return m_Color; }
	inline uint32_t GetID() const { return m_ID; }
	inline uint32_t GetIrradianceMap() const { return m_IrradianceMap; }
	inline uint32_t GetPrefilterMap() const { return m_PrefilterMap; }
	inline Ref<Texture> GetBRDF() const { return m_BRDF; }

private:
	void SetupMesh();

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};