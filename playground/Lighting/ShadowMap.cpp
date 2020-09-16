//
// Created by Quentin Warnant on 09/09/2020.
//

#include "ShadowMap.h"
ShadowMap::ShadowMap()
{
    m_FBO = 0;
    m_shadowMapID = 0;
}

ShadowMap::~ShadowMap()
{
    if( m_FBO )
    {
        glDeleteFramebuffers(1,&m_FBO);
    }

    if( m_shadowMapID)
    {
        glDeleteTextures(1, &m_shadowMapID);
    }
}

bool ShadowMap::Init(GLuint shadowMapWidth, GLuint shadowMapHeight)
{
    m_shadowMapHeight = shadowMapHeight;
    m_shadowMapWidth = shadowMapWidth;

    glGenFramebuffers(1,&m_FBO);

    glGenTextures(1,&m_shadowMapID);
    printf("Shadowmap init mapID: %i",m_shadowMapID );
    glBindTexture( GL_TEXTURE_2D, m_shadowMapID);
    glTexImage2D(GL_TEXTURE_2D,0, GL_DEPTH_COMPONENT, m_shadowMapWidth, m_shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 nullptr );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //Now we're binding our FBO to this texture so when we write to the FBO it will update this texture
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapID, 0);

    //FrameBuffer won't read or draw from "color attachements "
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum err = glGetError();
    if( err != GL_NO_ERROR)
    {
        printf("error after setting up shadowmap framebuffer  \n");
    }
    else
    {
        printf("all fine  \n");
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("FrameBuffer init error %i", status);
        return false;
    }

    return true;
}

void ShadowMap::Write()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void ShadowMap::Read(GLuint textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_shadowMapID);
}

