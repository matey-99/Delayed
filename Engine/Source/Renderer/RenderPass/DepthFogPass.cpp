#include "DepthFogPass.h"

#include "GBufferPass.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/RenderTools.h"
#include "Camera/CameraManager.h"

DepthFogPass::DepthFogPass() {
    RenderTarget::Config depthFogConfig;
    depthFogConfig.Attachment = RenderTarget::Attachment::Color;
    depthFogConfig.Type = RenderTarget::Type::Texture2D;
    depthFogConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;

    m_RenderTarget = RenderTarget::Create(depthFogConfig, 1920, 1080);
}

DepthFogPass::~DepthFogPass() {
}

void DepthFogPass::Render(uint32_t input) {
    m_RenderTarget->Bind();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    auto g = Renderer::GetInstance()->m_GBufferPass;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, input);

    glm::vec3 camPos = CameraManager::GetInstance()->GetMainCamera()->GetWorldPosition();
    glm::vec4 color(0.4f, 0.4f, 0.4f, 1.0f);

    auto depthFogShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "DepthFog");
    depthFogShader->Use();
    depthFogShader->SetInt("u_GBufferPosition", 0);
    depthFogShader->SetInt("u_Screen", 1);
    depthFogShader->SetFloat("u_FogMinimalDistance", 10.0f);
    depthFogShader->SetFloat("u_FogMaximumDistance", 100.0f);
    depthFogShader->SetFloat("u_Density", 0.2f);
    depthFogShader->SetVec4("u_FogColor", color);

    RenderTools::GetInstance()->RenderQuad();

    glDisable(GL_FOG);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_RenderTarget->Unbind();
}

void DepthFogPass::UpdateRenderTargets(uint32_t width, uint32_t height) {
    m_RenderTarget->Update(width, height);
}
