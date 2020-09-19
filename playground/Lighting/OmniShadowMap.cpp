#include "OmniShadowMap.hpp"

OmniShadowMap::OmniShadowMap() : ShadowMap()
{
}

OmniShadowMap::~OmniShadowMap() = default;


bool OmniShadowMap::Init(GLuint shadowMapWidth, GLuint shadowMapHeight)
{
    m_shadowMapHeight = shadowMapHeight;
    m_shadowMapWidth = shadowMapWidth;

    glGenFramebuffers(1,&m_FBO);

    glGenTextures(1,&m_shadowMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMapID);

    //Generate each cubemap texture (ie: 6)
    for (size_t i = 0; i < 6; ++i)
    {
        // enum value + i = iterates over each side of cubemap
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     m_shadowMapWidth, m_shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    //not "2D"
    glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMapID, 0);

    //FrameBuffer won't read or draw from "color attachements "
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum err = glGetError();
    if( err != GL_NO_ERROR)
    {
        printf("error after setting up omniShadowmap framebuffer  \n");
    }
    else
    {
        printf("all fine  \n");
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("OmniShadowmap FrameBuffer init error %i", status);
        return false;
    }

    return true;
}

void OmniShadowMap::Write()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void OmniShadowMap::Read(GLuint textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMapID);
}
