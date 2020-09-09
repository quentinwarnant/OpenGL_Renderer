//
// Created by Quentin Warnant on 09/09/2020.
//
#pragma once
#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap();
    ~ShadowMap();

    virtual bool Init(GLuint shadowMapWidth, GLuint shadowMapHeight);
    virtual void Write();
    virtual void Read(GLuint textureUnit);

    GLuint GetShadowWidth(){return m_shadowMapWidth;}
    GLuint GetShadowHeight(){return m_shadowMapHeight;}


protected:
    GLuint m_FBO; // shadowmap Frame Buffer Object
    GLuint m_shadowMapID;// texture ID

    GLuint m_shadowMapWidth;
    GLuint m_shadowMapHeight;


};


