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

	std::vector<std::string> m_Paths;
	bool m_SkyVisibility;
	glm::vec3 m_Color;
	float m_Intensity;

	Ref<Shader> m_Shader;

public:
	SkyLight(Actor* owner);
	SkyLight(Actor* owner, std::vector<std::string> paths);
	~SkyLight();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	void Load(std::vector<std::string> paths);

	inline std::vector<std::string> GetPaths() const { return m_Paths; }
	inline float GetIntensity() const { return m_Intensity; }
	inline glm::vec3 GetColor() const { return m_Color; }
	inline uint32_t GetID() const { return m_ID; }

private:
	void SetupMesh();

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};