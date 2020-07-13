#pragma once

#include <GL/glew.h>
#include "stb_image.h"


class Texture
{
public:
    Texture(const char* fileLocation);
    ~Texture();
    

    void LoadTexture();
    void UseTexture();
    void ClearTexture();

private:
    GLuint m_texID;
    int m_width, m_height, m_bitDepth;

    const char*  m_fileLocation;

};