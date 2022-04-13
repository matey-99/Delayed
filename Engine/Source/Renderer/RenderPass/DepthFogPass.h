#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class DepthFogPass {
private:

    struct DepthFogSettings {

    };

public:
    DepthFogPass();

    ~DepthFogPass();

    void Render(uint32_t input);
    void UpdateRenderTargets(uint32_t width, uint32_t height);

    inline const DepthFogSettings& GetSettings() const { return m_Settings; }

    inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
    DepthFogSettings m_Settings;

    Ref<RenderTarget> m_RenderTarget;

    friend class RendererSettingsPanel;
};