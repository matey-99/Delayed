#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "typedefs.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

class Material
{
public:
	enum class BlendMode
	{
		Opaque, Transparent
	};

private:
	uint64_t m_ID;
	std::string m_Name;
	std::string m_Path;
	BlendMode m_BlendMode;
	Ref<Shader> m_Shader;

protected:
	std::unordered_map<std::string, bool> m_BoolParameters;
	std::unordered_map<std::string, float> m_FloatParameters;
	std::unordered_map<std::string, glm::vec3> m_Vec3Parameters;
	std::unordered_map<std::string, Ref<Texture>> m_Texture2DParameters;

public:
	Material(std::string name = "Default", std::string path = "Materials/Default.mat", Ref<Shader> shader = Ref<Shader>());
	Material(uint64_t id, std::string name = "Default", std::string path = "Materials/Default.mat", Ref<Shader> shader = Ref<Shader>());

	static Ref<Material> Create(std::string name, std::string path, Ref<Shader> shader);
	static Ref<Material> Create(std::string name, std::string path, std::string shaderName);
	static Ref<Material> Create(uint64_t id, std::string name, std::string path, std::string shaderName);

	void LoadParameters();
	void Use();

	inline uint64_t GetID() const { return m_ID; }
	inline std::string GetName() const { return m_Name; }
	inline std::string GetPath() const { return m_Path; }
	inline BlendMode GetBlendMode() const { return m_BlendMode; }
	inline Ref<Shader> GetShader() const { return m_Shader; }

	inline std::unordered_map<std::string, bool> GetBoolParameters() const { return m_BoolParameters; }
	inline std::unordered_map<std::string, float> GetFloatParameters() const { return m_FloatParameters; }
	inline std::unordered_map<std::string, glm::vec3> GetVec3Parameters() const { return m_Vec3Parameters; }
	inline std::unordered_map<std::string, Ref<Texture>> GetTexture2DParameters() const { return m_Texture2DParameters; }

	inline void SetBlendMode(BlendMode mode) { m_BlendMode = mode; }

	friend class MaterialEditorPanel;
	friend class MaterialSerializer;
};