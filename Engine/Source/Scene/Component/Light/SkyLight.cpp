#include "SkyLight.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Content/ContentHelper.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderTools.h"

SkyLight::SkyLight(Actor* owner)
    : RenderComponent(owner)
{
    m_Shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Skybox, "Skybox");

    m_ID = 0;
    m_SkyVisibility = false;
    m_Color = glm::vec3(1.0f);
    m_Intensity = 1.0f;
}

SkyLight::SkyLight(Actor* owner, std::vector<std::string> paths)
    : RenderComponent(owner), m_Paths(paths)
{
	m_Shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Skybox, "Skybox");

    m_ID = 0;
    m_SkyVisibility = true;
    m_Color = glm::vec3(1.0f);
    m_Intensity = 1.0f;

    Load(m_Paths);
}

SkyLight::~SkyLight()
{
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

void SkyLight::Render()
{
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
    }

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    int width, height, channelsNumber;
    uint8_t* data;
    for (uint32_t i = 0; i < paths.size(); i++)
    {
        data = stbi_load(ContentHelper::GetAssetPath(paths[i]).c_str(), &width, &height, &channelsNumber, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
}
