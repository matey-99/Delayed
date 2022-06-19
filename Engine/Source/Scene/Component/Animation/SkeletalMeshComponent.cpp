#include "SkeletalMeshComponent.h"

#include "Assets/AssetManager.h"
#include "Importer/MaterialImporter.h"
#include "Camera/CameraManager.h"
#include "Scene/Component/Animation/Rig.h"
#include "Scene/Component/Animation/Animation.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Math/Transform.h"

#include <glad/glad.h>

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	: SkeletalMeshComponent(owner, "Models/Skeletal/SK_Astronaut.fbx")
{

}

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner, std::string path)
	: MeshComponent(owner, path)
{
	LoadMesh(path);

	for (int i = 0; i < m_SkeletalModel->GetMeshes().size(); i++)
		LoadMaterial("Materials/Default.mat");
}

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner, std::string path, std::vector<std::string> materialsPaths)
	: MeshComponent(owner, path, materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(AssetManager::LoadMaterial(path));
}

void SkeletalMeshComponent::Render(Material::BlendMode blendMode)
{
}

std::vector<Ref<MeshBase>> SkeletalMeshComponent::GetMeshes() const
{
	std::vector<Ref<MeshBase>> result;
	for (auto mesh : m_SkeletalModel->GetMeshes())
		result.push_back(mesh);

	return result;
}

uint32_t SkeletalMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;

	for (auto mesh : m_SkeletalModel->GetMeshes())
	{
		vertices += mesh->GetVertices().size();
	}

	return vertices;
}

Ref<Animation> SkeletalMeshComponent::GetAnimation(int index)
{
	if (index < m_Animations.size() && index >= 0)
	{
		return m_Animations[index];
	}
	else
	{
		return nullptr;
	}

}

Ref<BoneMap> SkeletalMeshComponent::FindBoneInRig(std::string boneName)
{
	return m_SkeletalModel->GetRig()->FindBone(boneName);
}

void SkeletalMeshComponent::PropagateBoneTransforms(std::vector<glm::mat4> boneMatrices)
{
	m_SkeletalModel->PropagateBoneTransforms(boneMatrices);
}

uint32_t SkeletalMeshComponent::GetBoneCount() const
{
	return m_SkeletalModel->GetRig()->HowManyBones();
}

void SkeletalMeshComponent::LoadMesh(std::string path)
{
	m_Path = path;
	// Below assimp could be loading only once from the path, but it reads twice
	m_SkeletalModel = AssetManager::LoadSkeletalModel(path);
	m_Animations = AssetManager::LoadAnimations(path, m_SkeletalModel->GetRig());
}

void SkeletalMeshComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);

	m_Materials.clear();
	m_MaterialsPaths.clear();

	for (int i = 0; i < m_SkeletalModel->GetMeshes().size(); i++)
		LoadMaterial("Materials/Default.mat");
}

void SkeletalMeshComponent::ChangeModel(Ref<ModelBase> modelBase)
{
	// Is it enough?
	if (auto model = StaticCast<SkeletalModel>(modelBase))
		m_SkeletalModel = model;
}

Ref<ModelBase> SkeletalMeshComponent::GetModel() const
{
	return m_SkeletalModel;
}

void SkeletalMeshComponent::UpdateBoundingBox()
{
	std::vector<glm::vec3> pointsFromAllMeshes;
	for (auto mesh : m_SkeletalModel->GetMeshes())
	{
		auto points = mesh->GetBoundingBox().GetPoints();
		for (auto& point : points)
		{
			// character bounding box fix - should be done better
			glm::mat4 world = m_Owner->GetTransform()->GetWorldModelMatrix();
			world = glm::scale(world, glm::vec3(100.0f));

			point = world * glm::vec4(point, 1.0f);
			pointsFromAllMeshes.push_back(point);
		}
	}

	m_BoundingBox = BoundingBox(pointsFromAllMeshes);
}

void SkeletalMeshComponent::UpdateBoundingSphere() {
	BoundingSphere s0, s1, s = m_SkeletalModel->GetMeshes()[0]->GetBoundingSphere();
	glm::vec3 d;
	float dist2, dist, r;

	s.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s.Center, 1.0f);
	s.Radius *= m_Owner->GetTransform()->GetWorldModelMatrix()[0][0];

	for (auto mesh : m_SkeletalModel->GetMeshes())
	{
		s0 = s;
		s1 = mesh->GetBoundingSphere();
		s1.Center = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(s1.Center, 1.0f);
		s1.Radius *= m_Owner->GetTransform()->GetWorldModelMatrix()[0][0];

		d = s1.Center - s0.Center;
		dist2 = glm::dot(d, d);

		r = s1.Radius - s0.Radius;

		if ((r * r) >= dist2) {
			if (s1.Radius >= s0.Radius)
				s = s1;
		}
		else {
			dist = glm::sqrt(dist2);
			s.Radius = (dist + s0.Radius + s1.Radius) * 0.5f;
			s.Center = s0.Center;
			if (dist > (s0.Radius + s1.Radius))
				s.Center += ((s.Radius - s0.Center) / dist) * d;
		}

	}

	m_BoundingSphere = s;
}