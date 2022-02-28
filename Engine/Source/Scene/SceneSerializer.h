#pragma once

#include "Scene.h"

#include <yaml-cpp/yaml.h>

class SceneSerializer
{
public:
	static void Serialize(Ref<Scene> scene, std::string destinationPath);
	static Ref<Scene> Deserialize(std::string path);

private:
	static void SerializeActor(YAML::Emitter& out, Ref<Actor> actor);
};