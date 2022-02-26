#pragma once

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs);

		static bool decode(const Node& node, glm::vec3& rhs);
	};

	Emitter& operator<<(Emitter& out, const glm::vec3& v);

	Emitter& operator<<(Emitter& out, const glm::vec4& v);
}