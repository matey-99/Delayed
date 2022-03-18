#include "RendererSettingsPanel.h"

#include "Renderer/RenderPass/PostProcessingPass.h"

RendererSettingsPanel::RendererSettingsPanel(Ref<Editor> editor, Ref<Renderer> renderer)
    : m_Editor(editor), m_Renderer(renderer)
{

}

void RendererSettingsPanel::Render()
{
    ImGui::Begin("Renderer Settings");

    ImGui::Text("Post Processing");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Bloom");
    ImGui::Checkbox("Enabled", &m_Renderer->m_PostProcessingPass->m_Settings.BloomEnabled);
    ImGui::DragFloat("Threshold", &m_Renderer->m_PostProcessingPass->m_Settings.BloomThreshold, 0.1f, 0.1f, 20.0f);
    ImGui::DragFloat("Limit", &m_Renderer->m_PostProcessingPass->m_Settings.BloomLimit, 1.0f, 1.0f, 100.0f);
    ImGui::DragFloat("Intensity", &m_Renderer->m_PostProcessingPass->m_Settings.BloomIntensity, 0.1f, 0.1f, 20.0f);
    ImGui::DragFloat("Blur Sigma", &m_Renderer->m_PostProcessingPass->m_Settings.BloomBlurSigma, 0.1f, 0.1f, 20.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::DragFloat("Gamma", &m_Renderer->m_PostProcessingPass->m_Settings.Gamma, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Exposure", &m_Renderer->m_PostProcessingPass->m_Settings.Exposure, 0.1f, 0.0f, 10.0f);

    ImGui::End();
}
