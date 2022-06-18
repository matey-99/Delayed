#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Saveable.h"

#define SPACESHIP_PARTS_COUNT 3

class Material;

enum class SpaceshipPartType
{
	Part1, Part2, Part3
};

class SpaceshipPart : public GameComponent, public Saveable
{
public:
	SpaceshipPart(Actor* owner);
	~SpaceshipPart();

	virtual void Start() override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

	void Fix();

	inline SpaceshipPartType GetType() const { return m_Type; }

private:
	SpaceshipPartType m_Type;

	Ref<Material> m_Material;
	bool m_Placed;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};