#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "GLCommon.h"
#include <string>
#include <vector>

#include <iostream>
#include "GLMHelpers.h"
#include "boneInfo.h"

#include "cVAOManager.h"


static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

static int static_idCounter = 0;

class asModel
{
public:
    asModel(std::string path, GLuint program)
    {
        modelId = static_idCounter++;
        loadModel(path, program);
    }

    void Draw(glm::mat4 matModel,
        GLint matModel_Location,
        GLint matModelInverseTranspose_Location,
        GLuint program,
        cVAOManager* pVAOManager);

    std::string getDirectory();
    std::string getName();
    // model data


    glm::vec3 mPosition;
    glm::vec3 mOrientation;
    glm::vec3 mScale;

    bool bDontLight = true;
    bool bIsWireframe = false;

    size_t mNumMeshes;

    std::vector<sModelDrawInfo> meshes;

    std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }

    int GetId() { return modelId; }
private:
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;

    std::string directory;
    std::string modelName;
    int modelId;

    void loadModel(std::string path, GLuint shaderProgramID);
};
