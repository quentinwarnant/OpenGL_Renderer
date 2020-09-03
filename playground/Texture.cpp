#include "Texture.hpp"

Texture::Texture(const char* fileLoc)
{
    m_texID = 0;
    m_width = 0;
    m_height = 0;
    m_bitDepth = 0;
    m_fileLocation = fileLoc;
}

Texture::~Texture()
{
    ClearTexture();
}


bool Texture::LoadTextureNoAlpha()
{
    int channel = 0;
    unsigned char* textureData = stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, channel);
    if( !textureData)
    {
        printf("Failed to find texture %s\n", m_fileLocation);
        return false; // early exit
    }

    //OpenGL part
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Texture is now loaded into memory, we can unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(textureData);

    return true;
}


bool Texture::LoadTextureWithAlpha()
{
    int channel = 0;
    unsigned char* textureData = stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, channel);
    if( !textureData)
    {
        printf("Failed to find texture %s\n", m_fileLocation);
        return false; // early exit
    }

    //OpenGL part
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Texture is now loaded into memory, we can unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(textureData);

    return true;
}

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0); // Set "texture unit"
    glBindTexture(GL_TEXTURE_2D, m_texID);
}

void Texture::ClearTexture()
{
    glDeleteTextures(1, &m_texID);

    m_texID = 0;
    m_width = 0;
    m_height = 0;
    m_bitDepth = 0;
    m_fileLocation = "";
}