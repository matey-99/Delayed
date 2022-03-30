#include "RendererSettingsPanel.h"

#include "Renderer/RenderPass/PostProcessingPass.h"
#include "Renderer/RenderPass/SSAOPass.h"
#include "Renderer/RenderPass/DepthOfFieldPass.h"
#include "Renderer/RenderPass/UIPass.h"

RendererSettingsPanel::RendererSettingsPanel(Ref<Editor> editor, Ref<Renderer> renderer)
    : m_Editor(editor), m_Renderer(renderer)
{

}

void RendererSettingsPanel::Render()
{
    ImGui::Begin("Renderer Settings");

    ImGui::Checkbox("Post Processing Enabled", &m_Renderer->m_Settings.PostProcessingEnabled);
    ImGui::Checkbox("FXAA Enabled", &m_Renderer->m_Settings.FXAAEnabled);
    ImGui::Checkbox("Depth Of Field Enabled", &m_Renderer->m_Settings.DepthOfFieldEnabled);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("SSAO");
    ImGui::DragFloat("SSAO Intensity", &m_Renderer->m_SSAOPass->m_Settings.Intensity, 0.1f, 0.1f, 20.0f);
    ImGui::DragInt("Kernel Size", &m_Renderer->m_SSAOPass->m_Settings.KernelSize, 1, 2, 256);
    ImGui::DragFloat("Radius", &m_Renderer->m_SSAOPass->m_Settings.Radius, 0.1f, 0.1f, 20.0f);
    ImGui::DragFloat("Bias", &m_Renderer->m_SSAOPass->m_Settings.Bias, 0.001f, 0.0f, 1.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Post Processing");
    ImGui::Text("Bloom");
    ImGui::Checkbox("Enabled", &m_Renderer->m_PostProcessingPass->m_Settings.BloomEnabled);
    ImGui::DragFloat("Threshold", &m_Renderer->m_PostProcessingPass->m_Settings.BloomThreshold, 0.1f, 0.1f, 20.0f);
    ImGui::DragFloat("Limit", &m_Renderer->m_PostProcessingPass->m_Settings.BloomLimit, 1.0f, 1.0f, 100.0f);
    ImGui::DragFloat("Bloom Intensity", &m_Renderer->m_PostProcessingPass->m_Settings.BloomIntensity, 0.1f, 0.1f, 20.0f);
    ImGui::DragFloat("Blur Sigma", &m_Renderer->m_PostProcessingPass->m_Settings.BloomBlurSigma, 0.1f, 0.1f, 20.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Depth Of Field");
    ImGui::DragFloat("Min Distance", &m_Renderer->m_DepthOfFieldPass->m_Settings.MinDistance, 0.1f, 0.1f, 20.0f);
    ImGui::DragFloat("Max Distance", &m_Renderer->m_DepthOfFieldPass->m_Settings.MaxDistance, 0.1f, 0.1f, 100.0f);
    ImGui::DragInt("Blur Size", &m_Renderer->m_DepthOfFieldPass->m_Settings.Size, 1, 1, 4);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("General");
    ImGui::DragFloat("Gamma", &m_Renderer->m_PostProcessingPass->m_Settings.Gamma, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Exposure", &m_Renderer->m_PostProcessingPass->m_Settings.Exposure, 0.1f, 0.0f, 10.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("UI Post Processing");
    ImGui::Text("UI General");
    ImGui::DragFloat("UI Gamma", &m_Renderer->m_UIPass->m_Settings.Gamma, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("UI Exposure", &m_Renderer->m_UIPass->m_Settings.Exposure, 0.1f, 0.0f, 10.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));


    ImGui::End();
}
