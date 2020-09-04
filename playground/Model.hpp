#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Texture.hpp"

class Model{
public:
    Model();
    ~Model();

    void LoadModel(const std::string& filename );
    void RenderModel();
    void ClearModel();

private:
    std::vector<Mesh*> m_meshes;
    std::vector<Texture*> m_textures;
    std::vector<unsigned int> m_meshToTexIdxList;

    void LoadNode(aiNode* node, const aiScene* scene);
    void LoadMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterial(const aiScene* scene);
};