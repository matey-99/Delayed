#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class WaterPass {
public:
    WaterPass();
    ~WaterPass();

    void Render(Ref<Scene> scene);

    inline Ref<RenderTarget> GetReflectionRenderTarget() const { return m_ReflectionRenderTarget; }
    inline Ref<RenderTarget> GetRefractionRenderTarget() const { return m_RefractionRenderTarget; }

private:
    Ref<RenderTarget> m_ReflectionRenderTarget;
    Ref<RenderTarget> m_RefractionRenderTarget;
};

