#include "AnimationImporter.h"

AnimationImporter::AnimationImporter()
{
	
}

std::vector<Ref<Animation>> AnimationImporter::ImportAnimations(std::string path, Ref<Rig> rig)
{

	if (m_ImportedAnimations.find(path) != m_ImportedAnimations.end())
		return m_ImportedAnimations.at(path);

	std::vector<Ref<Animation>> importedAnimations;

	// [...]
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
	
	assert(scene && scene->mRootNode);

	if (scene->HasAnimations())
		for (int index = 0; index < scene->mNumAnimations; index++)
		{
			aiAnimation* animation = scene->mAnimations[index];
			const aiNode* root = scene->mRootNode;

			importedAnimations.push_back(CreateRef<Animation>( root, animation, rig ));
		}


	m_ImportedAnimations.insert({ path, importedAnimations });
	return importedAnimations;
}
