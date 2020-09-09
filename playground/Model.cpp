#include "Model.hpp"

Model::Model()
{

}

Model::~Model()
{

}
void Model::LoadModel(const std::string& filename )
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if( !scene )
    {
        printf("Model failed to load %s error: %s", filename.c_str(), importer.GetErrorString());
        return; 
    }

    LoadNode( scene->mRootNode, scene);

    LoadMaterial(scene);
}

void Model::RenderModel()
{
    for (size_t i = 0; i < m_meshes.size(); i++)
    {
        unsigned int texID = m_meshToTexIdxList[i];

        //Check not smaller than size
        if( texID < m_textures.size() && m_textures[texID])
        {
            m_textures[texID]->UseTexture();
        }

        m_meshes[i]->RenderMesh();
    }
 }

void Model::ClearModel()
{
    for (size_t i = 0; i < m_meshes.size(); i++)
    {
        if( m_meshes[i] )
        {
            delete m_meshes[i];
            m_meshes[i] = nullptr;
        }
    }

    for (size_t i = 0; i < m_textures.size(); i++)
    {
        if( m_textures[i] )
        {
            delete m_textures[i];
            m_textures [i] = nullptr;
        }
    }
}


void Model::LoadNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene); //Nodes only have mesh ID references, the actual meshes are stored in the scene (removes duplicate entries)
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i],scene);
    }   
}


void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        //Vertices
        GLfloat verts[] = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z } ;
        vertices.insert(vertices.end(), verts, verts + 3);

        //Check mesh has uvs
        if(mesh->mTextureCoords[0])
        {
            //Add uvs
            GLfloat uvs[] = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            vertices.insert(vertices.end(), uvs, uvs+2 );
        }
        else
        {
            //No uv - put empty values to keep formatting of "vertices" vector valid
            GLfloat uvs[] = { 0,0 };
            vertices.insert(vertices.end(), uvs, uvs+2 );
        }

        //Normals
        //Normals will exist because we've setup the importer with the smooth normal generated
        GLfloat normals[] = { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z };
        vertices.insert(vertices.end(), normals, normals +3); //inverting normals to make shader code intuitive
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) // Each face represnets a set of 3 indices
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
        
    }
    
    Mesh* newMesh = new Mesh();
    newMesh->CreateMesh(&vertices[0], vertices.size(), &indices[0], indices.size() );
    m_meshes.push_back(newMesh);
    m_meshToTexIdxList.push_back( mesh->mMaterialIndex);
    
}

void Model::LoadMaterial(const aiScene* scene)
{
    m_textures.resize(scene->mNumMaterials);
    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* mat = scene->mMaterials[i];
    
        m_textures[i] = nullptr;
        if(mat->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if( mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS )
            {
                std::string originalPathStr = std::string(path.data);
                int lastBackSlashPos = originalPathStr.rfind('\\');
                std::string fileName = originalPathStr.substr(lastBackSlashPos+1);
                std::string newPath = std::string("../assets/") + fileName;

                printf("loading material texture at path: %s\n", newPath.c_str());
                m_textures[i] = new Texture(newPath.c_str());

                if( m_textures[i]->LoadTextureWithAlpha() == false)
                {
                    printf("failed to load texture!");
                    delete m_textures[i];
                    m_textures[i] = nullptr;
                }
            }
        }

        if( m_textures[i] == nullptr)
        {
            //Fallback on default texture
            m_textures[i] = new Texture("../assets/defaultTex.png");
            m_textures[i]->LoadTextureNoAlpha(); 
        }
    }
    
}