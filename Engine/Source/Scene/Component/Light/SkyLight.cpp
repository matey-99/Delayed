#include "SkyLight.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Material/ShaderLibrary.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderTools.h"
#include "Assets/AssetManager.h"

SkyLight::SkyLight(Actor* owner)
    : RenderComponent(owner)
{
    m_Shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Skybox, "Skybox");

    m_ID = 0;
    m_SkyVisibility = false;
    m_Color = glm::vec3(1.0f);
    m_Intensity = 1.0f;
    m_Weight = 1.0f;

    RenderTarget::Config brdfConfig;
    brdfConfig.Attachment = RenderTarget::Attachment::Color;
    brdfConfig.Type = RenderTarget::Type::Texture2D;
    brdfConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RG16F;
    brdfConfig.Filter = RenderTarget::Filter::Linear;

    m_BRDFRenderTarget = RenderTarget::Create(brdfConfig, 512, 512);

    glGenFramebuffers(1, &m_CaptureFBO);
}

SkyLight::SkyLight(Actor* owner, std::vector<std::string> paths)
    : RenderComponent(owner), m_Paths(paths)
{
	m_Shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Skybox, "Skybox");

    m_ID = 0;
    m_SkyVisibility = true;
    m_Color = glm::vec3(1.0f);
    m_Intensity = 1.0f;
    m_Weight = 1.0f;

    RenderTarget::Config brdfConfig;
    brdfConfig.Attachment = RenderTarget::Attachment::Color;
    brdfConfig.Type = RenderTarget::Type::Texture2D;
    brdfConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RG16F;
    brdfConfig.Filter = RenderTarget::Filter::Linear;

    m_BRDFRenderTarget = RenderTarget::Create(brdfConfig, 512, 512);

    glGenFramebuffers(1, &m_CaptureFBO);

    Load(m_Paths);
}

SkyLight::~SkyLight()
{
    glDeleteFramebuffers(1, &m_CaptureFBO);
}

void SkyLight::Start()
{
}

void SkyLight::Update(float deltaTime)
{
}

void SkyLight::PreRender()
{
}

void SkyLight::SetupMesh()
{
    
}

void SkyLight::Render(Material::BlendMode blendMode)
{
    // Render skybox only in Forward Pass
    if (blendMode == Material::BlendMode::Opaque)
        return;

    if (m_SkyVisibility)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        m_Shader->Use();

        RenderTools::GetInstance()->RenderSkybox();

        glDepthFunc(GL_LESS);
    }
}

void SkyLight::Destroy()
{
}

void SkyLight::Load(std::vector<std::string> paths)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_Paths = paths;

    if (m_ID)
    {
        glDeleteTextures(1, &m_ID);
        glDeleteTextures(1, &m_IrradianceMap);
        glDeleteTextures(1, &m_PrefilterMap);
    }

    /* Environment Cubemap */
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    int width, height, channelsNumber;
    uint8_t* data;
    for (uint32_t i = 0; i < paths.size(); i++)
    {
        std::string path = "../../../Content/" + paths[i];
        data = stbi_load(path.c_str(), &width, &height, &channelsNumber, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    /* Irradiance */
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  1.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  1.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    };

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glGenTextures(1, &m_IrradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
    for (uint16_t i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
    glViewport(0, 0, 32, 32);

    auto irradianceShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Skybox, "Irradiance");
    irradianceShader->Use();
    irradianceShader->SetInt("u_EnvironmentMap", 0);
    irradianceShader->SetMat4("u_Projection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    for (uint16_t i = 0; i < 6; i++)
    {
        irradianceShader->SetMat4("u_View", captureViews[i]);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderTools::GetInstance()->RenderSkybox();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /* Prefilter */
    uint32_t prefilterMapSize = 128;

    glGenTextures(1, &m_PrefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
    for (uint16_t i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, prefilterMapSize, prefilterMapSize, 0, GL_RGB, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    auto prefilterShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Skybox, "Prefilter");
    prefilterShader->Use();
    prefilterShader->SetInt("u_EnvironmentMap", 0);
    prefilterShader->SetMat4("u_Projection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
    uint32_t maxMipLevels = 5;
    for (uint32_t mip = 0; mip < maxMipLevels; mip++)
    {
        uint32_t mipWidth = prefilterMapSize * std::pow(0.5f, mip);
        uint32_t mipHeight = prefilterMapSize * std::pow(0.5f, mip);
        
        glViewport(0, 0, mipWidth, mipHeight);
        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader->SetFloat("u_Roughness", roughness);

        for (uint16_t i = 0; i < 6; i++)
        {
            prefilterShader->SetMat4("u_View", captureViews[i]);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            RenderTools::GetInstance()->RenderSkybox();
        }
    }

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);

    m_BRDFRenderTarget->Bind();
    auto brdfShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Calculations, "BRDF");
    brdfShader->Use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderTools::GetInstance()->RenderQuad();
    m_BRDFRenderTarget->Unbind();
}
