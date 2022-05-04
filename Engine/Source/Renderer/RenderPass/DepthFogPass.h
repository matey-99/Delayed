#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class DepthFogPass {
private:

    struct DepthFogSettings {
        float MinDistance = 10.0f;
        float MaxDistance = 100.0f;
        float Density = 0.2f;
        glm::vec3 Color = glm::vec3(0.4f);
    };

public:
    DepthFogPass();

    ~DepthFogPass();

    void Render(uint32_t input);
    void UpdateRenderTarget(uint32_t width, uint32_t height);

    inline const DepthFogSettings& GetSettings() const { return m_Settings; }

    inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
    DepthFogSettings m_Settings;

    Ref<RenderTarget> m_RenderTarget;

    friend class SceneSerializer;
    friend class RendererSettingsPanel;
};