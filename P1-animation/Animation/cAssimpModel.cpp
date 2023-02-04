#include "cAssimpModel.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void asModel::Draw(glm::mat4 matModel,
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager)
{

    // Set up textures for this object
    //SetUpTextures(pCurrentMesh, program);


    // Alpha transparency
    glEnable(GL_BLEND);
    // Basic "alpha transparency"
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Get the uniform (should be outside of the draw call)
    GLint wholeObjectAlphaTransparency_LocID = glGetUniformLocation(program, "wholeObjectAlphaTransparency");
    // Set this value here
    glUniform1f(wholeObjectAlphaTransparency_LocID, 1.f);


    // *****************************************************
    // Translate or "move" the object somewhere
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        this->mPosition);
    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
        this->mOrientation.z,//(float)glfwGetTime(),
        glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = matModel * rotateZ;
    // *****************************************************

    // *****************************************************
    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
        this->mOrientation.y,
        glm::vec3(0.0f, 1.0f, 0.0f));
    //matModel = matModel * rotateY;
    // *****************************************************

    // *****************************************************
    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        this->mOrientation.x,
        glm::vec3(1.0f, 0.0f, 0.0f));
    //matModel = matModel * rotateX;
    // *****************************************************


    // *****************************************************
    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        glm::vec3(this->mScale.x,  // Scale in X
            this->mScale.y,  // Scale in Y
            this->mScale.z));// Scale in Z
//matModel = matModel * matScale;
// *****************************************************

// *****************************************************
    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;     // <-- mathematically, this is 1st

    // Now the matModel ("Model" or "World") matrix
    //  represents ALL the transformations we want, in ONE matrix.

    // Moved view and projection ("v" and "p") to outside draw scene loop.
    // (because they are the same for all objects)

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));


    // Inverse transpose of the model matrix
    // (Used to calculate the normal location in vertex space, using only rotation)
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));


    // Copy the whole object colour information to the sahder               

    // This is used for wireframe or whole object colour. 
    // If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
    GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
    GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");

    // If true, then the lighting contribution is NOT used. 
    // This is useful for wireframe object
    GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");

    // The "whole object" colour (diffuse and specular)
    GLint wholeObjectDiffuseColour_Location = glGetUniformLocation(program, "wholeObjectDiffuseColour");
    GLint bUseWholeObjectDiffuseColour_Location = glGetUniformLocation(program, "bUseWholeObjectDiffuseColour");
    GLint wholeObjectSpecularColour_Location = glGetUniformLocation(program, "wholeObjectSpecularColour");


    GLint wholeObjectAlpha_Location = glGetUniformLocation(program, "wholeObjectAlphaTransparency");

    glUniform1f(wholeObjectAlpha_Location, 1.f);



    GLint bIsAssimpModel = glGetUniformLocation(program, "bIsAssimpModel");

    glUniform1f(bIsAssimpModel, (float)GL_TRUE);


    glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);

   /* glUniform4f(wholeObjectSpecularColour_Location,
        pCurrentMesh->wholeObjectSpecularRGB.r,
        pCurrentMesh->wholeObjectSpecularRGB.g,
        pCurrentMesh->wholeObjectSpecularRGB.b,
        pCurrentMesh->wholeObjectShininess_SpecPower);*/


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    //if (pCurrentMesh->bUseObjectDebugColour)
    //{
    //    // Override the colour...
    //    glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
    //    glUniform4f(objectDebugColour_Location,
    //        pCurrentMesh->objectDebugColourRGBA.r,
    //        pCurrentMesh->objectDebugColourRGBA.g,
    //        pCurrentMesh->objectDebugColourRGBA.b,
    //        pCurrentMesh->objectDebugColourRGBA.a);
    //}
    //else
    //{
    //    // DON'T override the colour
    //    glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
    //}


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (this->bDontLight)
    {
        // Override the colour...
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }


    // Wireframe
    if (this->bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Draw everything with only lines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }




    // glPointSize(20.0f); sometimes this works... Hey, it's not my fault!



    //        if (gVAOManager.FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
    //        if (gVAOManager.FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))
    for (int i = 0; i < this->meshes.size(); i++)
    {
        glBindVertexArray(this->meshes[i].VAO_ID);

        glDrawElements(GL_TRIANGLES,
            this->meshes[i].numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0);

    }


    return;
}

std::string asModel::getDirectory()
{
    return directory;
}

std::string asModel::getName()
{
    return modelName;
}

const int MAX_BONE_INFLUENCE = 1000;



void asModel::loadModel(std::string path, GLuint shaderProgramID)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/') + 1);
    modelName = path.substr(path.find_last_of('/') + 1, path.size() - path.find_last_of('/') - 1);


    if (scene->HasMeshes())
    {

        int numOfMeshes = scene->mNumMeshes;
        for (int meshI = 0; meshI < numOfMeshes; meshI++)
        {
            sModelDrawInfo meshInfo;
            aiMesh* curMesh = scene->mMeshes[meshI];
            
            meshInfo.pVertices = new sVertex_XYZW_RGBA_N_UV_T_B[curMesh->mNumVertices];

            meshInfo.numberOfVertices = curMesh->mNumVertices;
            meshInfo.numberOfTriangles = curMesh->mNumFaces;
            
            std::vector<sVertex_XYZW_RGBA_N_UV_T_B> vecVertexArray;

            for (unsigned int vertexI = 0; vertexI < meshInfo.numberOfVertices; vertexI++)
            {
                sVertex_XYZW_RGBA_N_UV_T_B tempVertex;

                tempVertex.x = curMesh->mVertices[vertexI].x;
                tempVertex.y = curMesh->mVertices[vertexI].y;
                tempVertex.z = curMesh->mVertices[vertexI].z;
                tempVertex.w = 1.f;

                tempVertex.nx = curMesh->mNormals[vertexI].x;
                tempVertex.ny = curMesh->mNormals[vertexI].y;
                tempVertex.nz = curMesh->mNormals[vertexI].z;
                tempVertex.nw = 0.f;

                if (curMesh->mMaterialIndex >= 0)
                {
                    aiMaterial* curMat = scene->mMaterials[curMesh->mMaterialIndex];

                    aiColor3D color;
                    curMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                    float diffuseMultiplier = 1.f;
                    tempVertex.r = color.r * diffuseMultiplier;
                    tempVertex.g = color.g * diffuseMultiplier;
                    tempVertex.b = color.b * diffuseMultiplier;
                    tempVertex.a = 1.f;

                }

                if (curMesh->HasTextureCoords(0))
                {
                    tempVertex.u0 = curMesh->mTextureCoords[0][vertexI].x;
                    tempVertex.v0 = curMesh->mTextureCoords[0][vertexI].y;
                    tempVertex.u1 = curMesh->mTextureCoords[1][vertexI].x;
                    tempVertex.v1 = curMesh->mTextureCoords[1][vertexI].y;
                }

                tempVertex.boneId0 = -1;
                tempVertex.boneId1 = -1;
                tempVertex.boneId2 = -1;
                tempVertex.boneId3 = -1;

                tempVertex.boneWeight0 = 0.f;
                tempVertex.boneWeight1 = 0.f;
                tempVertex.boneWeight2 = 0.f;
                tempVertex.boneWeight3 = 0.f;

                meshInfo.pVertices[vertexI] = tempVertex;
            }

            meshInfo.numberOfIndices = meshInfo.numberOfTriangles * 3;
            meshInfo.pIndices = new unsigned int[meshInfo.numberOfIndices];

            // Copy the triangle ("index") values to the index (element) array
            unsigned int elementIndex = 0;
            unsigned int curNumIndicies = 0;
            for (unsigned int triIndex = 0; triIndex < meshInfo.numberOfTriangles;
                triIndex++, elementIndex += curNumIndicies)
            {
                aiFace face = curMesh->mFaces[triIndex];

                for (int j = 0; j < face.mNumIndices; j++)
                {
                    meshInfo.pIndices[elementIndex + j] = face.mIndices[j];
                }
                curNumIndicies = face.mNumIndices;
            }
            
            if (curMesh->HasBones())
            { 

                for (int boneIndex = 0; boneIndex < curMesh->mNumBones; ++boneIndex)
                {
                    int boneID = -1;
                    std::string boneName = curMesh->mBones[boneIndex]->mName.C_Str();
                    if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
                    {
                        BoneInfo newBoneInfo;
                        newBoneInfo.id = m_BoneCounter;
                        newBoneInfo.offset = ConvertMatrixToGLMFormat(curMesh->mBones[boneIndex]->mOffsetMatrix);
                        m_BoneInfoMap[boneName] = newBoneInfo;
                        boneID = m_BoneCounter;
                        m_BoneCounter++;
                    }
                    else
                    {
                        boneID = m_BoneInfoMap[boneName].id;
                    }
                    assert(boneID != -1);
                    auto weights = curMesh->mBones[boneIndex]->mWeights;
                    int numWeights = curMesh->mBones[boneIndex]->mNumWeights;

                    for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
                    {
                        int vertexId = weights[weightIndex].mVertexId;
                        float weight = weights[weightIndex].mWeight;
                        assert(vertexId <= meshInfo.numberOfVertices);
                        if (meshInfo.pVertices[vertexId].boneId0 == -1)
                        {
                            meshInfo.pVertices[vertexId].boneId0 = boneID;
                                
                            meshInfo.pVertices[vertexId].boneWeight0 = weight;
                        }else if (meshInfo.pVertices[vertexId].boneId1 == -1)
                        {
                            meshInfo.pVertices[vertexId].boneId1 = boneID;

                            meshInfo.pVertices[vertexId].boneWeight1 = weight;
                        }else if (meshInfo.pVertices[vertexId].boneId2 == -1)
                        {
                            meshInfo.pVertices[vertexId].boneId2 = boneID;

                            meshInfo.pVertices[vertexId].boneWeight2 = weight;
                        }else if (meshInfo.pVertices[vertexId].boneId3 == -1)
                        {
                            meshInfo.pVertices[vertexId].boneId3 = boneID;

                            meshInfo.pVertices[vertexId].boneWeight3 = weight;
                        }
                        
                    }
                }
            }

            glGenVertexArrays(1, &(meshInfo.VAO_ID));
            // "Bind" this buffer:
            // - aka "make this the 'current' VAO buffer
            glBindVertexArray(meshInfo.VAO_ID);

            // Now ANY state that is related to vertex or index buffer
            //	and vertex attribute layout, is stored in the 'state' 
            //	of the VAO... 


            // NOTE: OpenGL error checks have been omitted for brevity
        //	glGenBuffers(1, &vertex_buffer);
            glGenBuffers(1, &(meshInfo.VertexBufferID));

            //	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, meshInfo.VertexBufferID);
            // sVert vertices[3]
            glBufferData(GL_ARRAY_BUFFER,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B)* meshInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
                (GLvoid*)meshInfo.pVertices,							// pVertices,			//vertices, 
                GL_STATIC_DRAW);
            //glBufferData( GL_ARRAY_BUFFER, 
            //			  sizeof(sVertex_XYZW_RGBA) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
            //			  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
            //			  GL_STATIC_DRAW );
            //glBufferData( GL_ARRAY_BUFFER, 
            //			  sizeof(sVertex_XYZ_RGB) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
            //			  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
            //			  GL_STATIC_DRAW );


            // Copy the index buffer into the video card, too
            // Create an index buffer.
            glGenBuffers(1, &(meshInfo.IndexBufferID));

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo.IndexBufferID);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
                sizeof(unsigned int)* meshInfo.numberOfIndices,
                (GLvoid*)meshInfo.pIndices,
                GL_STATIC_DRAW);

            // ****************************************************************
            // Set the vertex attributes.


            //struct sVertex_XYZW_RGBA
            //{
            //    float x, y, z, w;   // Same as vec4 vPosition
            //    float r, g, b, a;   // Same as vec4 vColour
            //};

            // Set the vertex attributes for this shader
            GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	    // program
            glEnableVertexAttribArray(vpos_location);	    // vPosition
            glVertexAttribPointer(vpos_location, 4,		// vPosition
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,      
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, x));

            GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");	// program;
            glEnableVertexAttribArray(vcol_location);	    // vColour
            glVertexAttribPointer(vcol_location, 4,		// vColour
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,   
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, r));

            // And all these, too:
            //in vec4 vNormal;				// Vertex normal X,Y,Z (W ignored)
            GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
            glEnableVertexAttribArray(vNormal_location);	    // vNormal
            glVertexAttribPointer(vNormal_location, 4,		    // vNormal
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, nx));


            //in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
            GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUVx2");	// program;
            glEnableVertexAttribArray(vUVx2_location);	        // vUVx2
            glVertexAttribPointer(vUVx2_location, 4,		    // vUVx2
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, u0));


            //in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
            GLint vTangent_location = glGetAttribLocation(shaderProgramID, "vTangent");	// program;
            glEnableVertexAttribArray(vTangent_location);	    // vTangent
            glVertexAttribPointer(vTangent_location, 4,		    // vTangent
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, tx));


            //in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
            GLint vBiNormal_location = glGetAttribLocation(shaderProgramID, "vBiNormal");	// program;
            glEnableVertexAttribArray(vBiNormal_location);	        // vBiNormal
            glVertexAttribPointer(vBiNormal_location, 4,		    // vBiNormal
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bx));

            //in vec4 vBoneIds;				// For bone animation
            GLint vBoneId_Location = glGetAttribLocation(shaderProgramID, "vBoneIds");	// program;
            glEnableVertexAttribArray(vBoneId_Location);	        // vBoneIds
            glVertexAttribPointer(vBoneId_Location, 4,		    // vBoneIds
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, boneId0));

            //in vec4 vBoneIds;				// For bone animation
            GLint vBoneWeights_Location = glGetAttribLocation(shaderProgramID, "vBoneWeights");	// program;
            glEnableVertexAttribArray(vBoneWeights_Location);	        // vBiNormal
            glVertexAttribPointer(vBoneWeights_Location, 4,		    // vBiNormal
                GL_FLOAT, GL_FALSE,
                sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
                (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, boneWeight0));


            // ****************************************************************

            // Now that all the parts are set up, set the VAO to zero
            glBindVertexArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glDisableVertexAttribArray(vpos_location);
            glDisableVertexAttribArray(vcol_location);
            // And the newer ones:
            glDisableVertexAttribArray(vNormal_location);	    // vNormal
            glDisableVertexAttribArray(vUVx2_location);	        // vUVx2
            glDisableVertexAttribArray(vTangent_location);	    // vTangent
            glDisableVertexAttribArray(vBiNormal_location);	        // vBiNormal
            glDisableVertexAttribArray(vBoneId_Location);	        // vBones
            glDisableVertexAttribArray(vBoneWeights_Location);	        // vBoneWeights


            //// Store the draw information into the map
            //this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;


            //return true;

            this->meshes.push_back(meshInfo);

        }//end mesh loop
    }//mesh if

    

    //TODO animation
}

//void asMesh::setUpMesh()
//{
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(asVertex), &vertices[0], GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
//        &indices[0], GL_STATIC_DRAW);
//
//    // vertex positions
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(asVertex), (void*)0);
//    // vertex normals
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(asVertex), (void*)offsetof(asVertex, Normal));
//    // vertex texture coords
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(asVertex), (void*)offsetof(asVertex, TexCoords));
//
//    glBindVertexArray(0);
//}
//
//std::vector<asTexture> asModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
//{
//    std::vector<asTexture> textures;
//    /*
//    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//    {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        Texture texture;
//        texture.id = TextureFromFile(str.C_Str(), directory);
//        texture.type = typeName;
//        texture.path = std::string(str.C_Str());
//        textures.push_back(texture);
//    }
//   ;*/
//    return textures;
//}
//
//void asModel::processNode(aiNode* node, const aiScene* scene)
//{
//    // process all the node's meshes (if any)
//    for (unsigned int i = 0; i < node->mNumMeshes; i++)
//    {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes.push_back(processMesh(mesh, scene));
//    }
//    // then do the same for each of its children
//    for (unsigned int i = 0; i < node->mNumChildren; i++)
//    {
//        processNode(node->mChildren[i], scene);
//    }
//}
//
//asMesh asModel::processMesh(aiMesh* mesh, const aiScene* scene)
//{
//    std::vector<asVertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<asTexture> textures;
//
//    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//    {
//        asVertex vertex;
//        // process vertex positions, normals and texture coordinates
//        glm::vec3 vector;
//        vector.x = mesh->mVertices[i].x;
//        vector.y = mesh->mVertices[i].y;
//        vector.z = mesh->mVertices[i].z;
//        vertex.Position = vector;
//        vector.x = mesh->mNormals[i].x;
//        vector.y = mesh->mNormals[i].y;
//        vector.z = mesh->mNormals[i].z;
//        vertex.Normal = vector;
//        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//        {
//            glm::vec2 vec;
//            vec.x = mesh->mTextureCoords[0][i].x;
//            vec.y = mesh->mTextureCoords[0][i].y;
//            vertex.TexCoords = vec;
//        }
//        else
//            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//        vertices.push_back(vertex);
//    }
//    // process indices
//    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//    {
//        aiFace face = mesh->mFaces[i];
//        for (unsigned int j = 0; j < face.mNumIndices; j++)
//            indices.push_back(face.mIndices[j]);
//    }
//    // process material
//    if (mesh->mMaterialIndex >= 0)
//    {
//        if (mesh->mMaterialIndex >= 0)
//        {
//            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//            std::vector<asTexture> diffuseMaps = loadMaterialTextures(material,
//                aiTextureType_DIFFUSE, "texture_diffuse");
//            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//            std::vector<asTexture> specularMaps = loadMaterialTextures(material,
//                aiTextureType_SPECULAR, "texture_specular");
//            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//        }
//    }
//
//    return asMesh(vertices, indices, textures);
//}