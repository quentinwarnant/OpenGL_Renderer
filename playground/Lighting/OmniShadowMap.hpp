#pragma once
#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap
{
public:
    OmniShadowMap();
    ~OmniShadowMap();


    bool Init(GLuint shadowMapWidth, GLuint shadowMapHeight) override;
    void Write() override;
    void Read(GLuint textureUnit) override;

};


