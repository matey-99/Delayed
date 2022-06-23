#pragma once

#include "GameComponent.h"
#include "Saveable.h"

class StaticMeshComponent;
class MaterialInstance;

class Mushroom : public GameComponent, public Saveable {

public:
    Mushroom(Actor *owner);

    void Start() override;

    void OnTriggerEnter(ColliderComponent *other) override;

    const SaveData Save() override;

    void Load(const SaveData &data) override;

private:

    void SetOn();
    void SetOff();

public:

    static int m_GlobalActivePath;

private:

    int m_Path;
    bool m_Enabled;

    Ref<StaticMeshComponent> m_StaticMesh;
    Ref<MaterialInstance> m_Material;

    friend class SceneSerializer;
    friend class ActorDetailsPanel;
};

