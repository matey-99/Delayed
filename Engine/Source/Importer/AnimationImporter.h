#pragma once

#include "Core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Scene/Component/Animation/Animation.h"
#include "Scene/Component/Animation/Rig.h"

class AnimationImporter
{
public:
	AnimationImporter();

	std::vector<Ref<Animation>> ImportAnimations(std::string path, Ref<Rig> rig);

private:
	std::unordered_map<std::string, std::vector<Ref<Animation>>> m_ImportedAnimations;
};
