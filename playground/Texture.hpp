#pragma once

#include <GL/glew.h>
#include "stb_image.h"


class Texture
{
public:
    Texture(const char* fileLocation);
    ~Texture();
    

    bool LoadTextureNoAlpha();
    bool LoadTextureWithAlpha();
    void UseTexture();
    void ClearTexture();
    GLuint GetTexID(){return m_texID;}

private:
    GLuint m_texID;
    int m_width, m_height, m_bitDepth;

    const char*  m_fileLocation;

};