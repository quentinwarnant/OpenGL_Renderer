#include "Material.hpp"

Material::Material(GLfloat specularIntensity, GLfloat shininess)
{
    m_specularIntensity = specularIntensity;
    m_shininess = shininess;
}

Material::~Material()
{

}

void Material::UseMaterial(GLuint specIntensityLocation, GLuint shininessLocation)
{
    glUniform1f(specIntensityLocation, m_specularIntensity);
    glUniform1f(shininessLocation, m_shininess);
}