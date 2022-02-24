#pragma once

#include "Scene.h"

#include <yaml-cpp/yaml.h>

class SceneSerializer
{
public:
	static void Serialize(Ref<Scene> scene);
	static Ref<Scene> Deserialize(std::string path);

private:
	static void SerializeEntity(YAML::Emitter& out, Ref<Entity> entity);
};