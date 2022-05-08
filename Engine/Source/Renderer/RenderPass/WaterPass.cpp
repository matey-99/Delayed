#include "WaterPass.h"

WaterPass::WaterPass() {
    RenderTarget::Config reflectionConfig;
    reflectionConfig.Attachment = RenderTarget::Attachment::Color;

    RenderTarget::Config refractionConfig1;
    refractionConfig1.Attachment = RenderTarget::Attachment::Color;

    RenderTarget::Config refractionConfig2;
    refractionConfig2.Attachment = RenderTarget::Attachment::Depth;
    refractionConfig2.DepthInternalFormat = RenderTarget::DepthInternalFormat::Depth32;

    std::vector<RenderTarget::Config> refractionConfigs = {refractionConfig1, refractionConfig2};

    m_ReflectionRenderTarget = RenderTarget::Create(reflectionConfig, 1280, 720);
    m_RefractionRenderTarget = RenderTarget::CreateMRT(refractionConfigs, 1920, 1080);
}

WaterPass::~WaterPass() {

}

void WaterPass::Render(Ref<Scene> scene) {
    m_ReflectionRenderTarget->Bind();

    glEnable(GL_BLEND);
    glEnable(GL_CLIP_DISTANCE0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    scene->Render(Material::BlendMode::Opaque);

    glDisable(GL_CLIP_DISTANCE0);
    glDisable(GL_BLEND);

    m_ReflectionRenderTarget->Unbind();
}
