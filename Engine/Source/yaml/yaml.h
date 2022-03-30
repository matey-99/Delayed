#pragma once

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs);

		static bool decode(const Node& node, glm::vec2& rhs);
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs);

		static bool decode(const Node& node, glm::vec3& rhs);
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs);

		static bool decode(const Node& node, glm::vec4& rhs);
	};

	Emitter& operator<<(Emitter& out, const glm::vec2& v);

	Emitter& operator<<(Emitter& out, const glm::vec3& v);

	Emitter& operator<<(Emitter& out, const glm::vec4& v);
}