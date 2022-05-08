#include "RendererSettingsPanel.h"

#include "Renderer/RenderPass/PostProcessingPass.h"
#include "Renderer/RenderPass/SSAOPass.h"
#include "Renderer/RenderPass/SSRPass.h"
#include "Renderer/RenderPass/DepthOfFieldPass.h"
#include "Renderer/RenderPass/UIPass.h"
#include "Renderer/RenderPass/DepthFogPass.h"
#include "Renderer/RenderPass/VignettePass.h"
#include <glm/gtc/type_ptr.hpp>

RendererSettingsPanel::RendererSettingsPanel(Ref<Editor> editor, Ref<Renderer> renderer)
    : m_Editor(editor), m_Renderer(renderer)
{

}

void RendererSettingsPanel::Render()
{
    ImGui::Begin("Renderer Settings");

    ImGui::Checkbox("Depth Fog Enabled", &m_Renderer->m_Settings.DepthFogEnabled);
    ImGui::Checkbox("Post Processing Enabled", &m_Renderer->m_Settings.PostProcessingEnabled);
    ImGui::Checkbox("FXAA Enabled", &m_Renderer->m_Settings.FXAAEnabled);
    ImGui::Checkbox("SSR Enabled", &m_Renderer->m_Settings.SSREnabled);
    ImGui::Checkbox("Vignette Enabled", &m_Renderer->m_Settings.VignetteEnabled);
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

    ImGui::Text("Screen Space Reflections");
    ImGui::DragFloat("Ray Step", &m_Renderer->m_SSRPass->m_Settings.RayStep, 0.01f, 0.1f, 1.0f);
    ImGui::DragFloat("Min Ray Step", &m_Renderer->m_SSRPass->m_Settings.MinRayStep, 0.01f, 0.1f, 1.0f);
    ImGui::DragInt("Max Steps", &m_Renderer->m_SSRPass->m_Settings.MaxSteps, 1, 1, 100);
    ImGui::DragInt("Binary Search Steps", &m_Renderer->m_SSRPass->m_Settings.NumBinarySearchSteps, 1, 1, 20);
    ImGui::DragFloat("Reflection Falloff", &m_Renderer->m_SSRPass->m_Settings.ReflectionSpecularFalloffExponent, 0.1f, 0.0f, 10.0f);
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

    ImGui::Text("Depth Fog");
    ImGui::DragFloat("Fog Min Distance", &m_Renderer->m_DepthFogPass->m_Settings.MinDistance, 0.1f, 0.1f, 20.0f);
    ImGui::DragFloat("Fog Max Distance", &m_Renderer->m_DepthFogPass->m_Settings.MaxDistance, 0.1f, 0.1f, 200.0f);
    ImGui::DragFloat("Fog Density", &m_Renderer->m_DepthFogPass->m_Settings.Density, 0.01f, 0.01f, 1.0f);
    ImGui::ColorEdit3("Fog Color", glm::value_ptr(m_Renderer->m_DepthFogPass->m_Settings.Color));
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Vignette");
    ImGui::DragFloat("Vignette Intensity", &m_Renderer->m_VignettePass->m_Settings.Intensity, 0.1f, 0.1f, 50.0f);
    ImGui::DragFloat("Vignette Size", &m_Renderer->m_VignettePass->m_Settings.Size, 0.01f, 0.01f, 1.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("General");
    ImGui::DragFloat("Gamma", &m_Renderer->m_PostProcessingPass->m_Settings.Gamma, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Gain", &m_Renderer->m_PostProcessingPass->m_Settings.Gain, 0.01f, -1.0f, 1.f);
    ImGui::DragFloat("Exposure (inactive)", &m_Renderer->m_PostProcessingPass->m_Settings.Exposure, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Contrast", &m_Renderer->m_PostProcessingPass->m_Settings.Contrast, 0.01f, 0.0f, 1.9f);
    ImGui::DragFloat("Contrast Pivot", &m_Renderer->m_PostProcessingPass->m_Settings.ContrastPivot, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Offset", &m_Renderer->m_PostProcessingPass->m_Settings.Offset, 0.005f, -0.1f, 0.1f);
    ImGui::DragFloat("Lift", &m_Renderer->m_PostProcessingPass->m_Settings.Lift, 0.005f, -0.1f, 0.1f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Color Grading");
    ImGui::DragFloat("Saturation", &m_Renderer->m_PostProcessingPass->m_Settings.Saturation, 0.1f, 0.0f, 2.0f);
    ImGui::DragFloat("Temperature", &m_Renderer->m_PostProcessingPass->m_Settings.Temperature, 1.f, 15.0f, 150.0f);
    ImGui::DragFloat("Hue", &m_Renderer->m_PostProcessingPass->m_Settings.Hue, 1.0f, 0.0f, 360.0f);
    
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("UI Post Processing");
    ImGui::Text("UI General");
    ImGui::DragFloat("UI Gamma", &m_Renderer->m_UIPass->m_Settings.Gamma, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("UI Exposure", &m_Renderer->m_UIPass->m_Settings.Exposure, 0.1f, 0.0f, 10.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));


    


    ImGui::End();
}
