#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "typedefs.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

//template <typename T>
//struct MaterialParameter
//{
//	std::string uniformName;
//	std::string name;
//	T value;
//};
//
//struct MaterialBoolParameter : MaterialParameter<bool> {};
//struct MaterialFloatParameter : MaterialParameter<float> {};
//struct MaterialVec3Parameter : MaterialParameter<glm::vec3> {};
//struct MaterialTexture2DParameter : MaterialParameter<Ref<Texture>> {};

class Material
{
private:
	uint64_t m_ID;
	std::string m_Name;
	Ref<Shader> m_Shader;

	std::unordered_map<std::string, bool> m_BoolParameters;
	std::unordered_map<std::string, float> m_FloatParameters;
	std::unordered_map<std::string, glm::vec3> m_Vec3Parameters;
	std::unordered_map<std::string, Ref<Texture>> m_Texture2DParameters;

public:
	Material(std::string name = "Default", Ref<Shader> shader = Ref<Shader>());
	Material(uint64_t id, std::string name = "Default", Ref<Shader> shader = Ref<Shader>());

	static Ref<Material> Create(std::string name, Ref<Shader> shader);
	static Ref<Material> Create(std::string name, std::string shaderName);

	void LoadParameters();
	void Use();

	inline uint64_t GetID() const { return m_ID; }
	inline std::string GetName() const { return m_Name; }
	inline Ref<Shader> GetShader() const { return m_Shader; }

	friend class MaterialEditorPanel;
	friend class MaterialSerializer;
};