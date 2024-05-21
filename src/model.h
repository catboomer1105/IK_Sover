#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


#include "mesh.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


using namespace std;

class Model
{
  public:
    /*  函数   */
    Model(const char *path)
    {
        loadModel(path);
    }
    void Draw(Shader &shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

  private:
    /*  模型数据  */
    vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't
                                     // loaded more than once.
    vector<Mesh> meshes;
    string directory;
    /*  函数   */
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif