#include "Component.h"

class RigidBodyComponent : public Component {
public:
    RigidBodyComponent(Actor *owner);

    void Start() override;

    void Update(float deltaTime) override;

    void FixedUpdate() override;

    void Destroy() override;

    void AddForce(glm::vec3 force);

private:

    float m_Gravity;
    float m_Mass;
    float m_Drag;

    glm::vec3 m_Velocity;
    glm::vec3 m_VelocityMax;
    glm::vec3 m_Acceleration;

    friend class SceneSerializer;
};

