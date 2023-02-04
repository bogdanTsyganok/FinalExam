#include "globalThings.h"

cFlyCamera* g_pFlyCamera = NULL;



//glm::vec3 g_cameraEye = glm::vec3(0.0f, 0.0f, +4.0f);
//glm::vec3 g_cameraAt = glm::vec3(0.0f, 0.0f, 0.0f);

cVAOManager* g_pVAOManager = NULL;
cShaderManager* g_pShaderManager = NULL;
cBasicTextureManager* g_pTextureManager = NULL;

cLightManager* g_pTheLights = NULL;
// This got moved into the light manager class
//extern cLightHelper* g_pTheLightHelper;

// List of objects to draw
std::vector< cMesh* > g_vec_pMeshes;


cMesh* g_pDebugSphere = NULL;
cMesh* g_pDebugCylinder = NULL;
bool g_bShowDebugShere = true;

GLint g_currentProgram = 0;
GLint g_matModel_location = 0;
GLint g_inverseMatModel_location = 0;

unsigned int g_selectedObject = 0;
unsigned int g_selectedLight = 0;
bool triggerPulled = false;

// Used for the texture height displacement example
glm::vec3 g_heightMapUVOffsetRotation = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 CalculateMatrix(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation)
{

    glm::mat4 matModel(1.f);
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        pos);
    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
        rotation.z,//(float)glfwGetTime(),
        glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = matModel * rotateZ;
    // *****************************************************

    // *****************************************************
    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
        rotation.y,
        glm::vec3(0.0f, 1.0f, 0.0f));
    //matModel = matModel * rotateY;
    // *****************************************************

    // *****************************************************
    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        rotation.x,
        glm::vec3(1.0f, 0.0f, 0.0f));
    //matModel = matModel * rotateX;
    // *****************************************************


    // *****************************************************
    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        scale);
    // *****************************************************


    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;

    return matModel;
}


// This will be printed in the title bar
std::string g_TitleText = "";

gdpPhysics::PhysicsFactory* physicsFactory = NULL;
gdpPhysics::iPhysicsWorld* physicsWorld = NULL;
cPhysicsManager g_physicsManager;

bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh*& theObjectIFound);

glm::vec3 DirectionToRotation(glm::vec3 direction, glm::vec3 upVec)
{
    glm::vec3 eulerRotation = glm::vec3(0.f);
    glm::vec3 axis = glm::vec3(glm::normalize(glm::cross(upVec, direction)));
    GLfloat angle = glm::acos(glm::dot(upVec, direction));
    if (upVec == direction)
    {
        return eulerRotation;
    }
    GLdouble s = glm::sin(angle / 2);
    GLdouble c = glm::cos(angle / 2);
    glm::quat quatRotation(axis.x * s, axis.y * s, axis.z * s, c);

    eulerRotation = glm::eulerAngles(quatRotation);
    float temp = eulerRotation.z;
    eulerRotation.z = eulerRotation.x;
    eulerRotation.x = temp;
    eulerRotation.x = -eulerRotation.x;
    eulerRotation.y = -eulerRotation.y;
    return eulerRotation;
}

//void gdpPhysics::iPhysicsWorldDrawDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
//{
//    if (::g_pDebugCylinder == NULL)
//    {
//        ::g_pDebugCylinder = new cMesh();
//        ::g_pDebugCylinder->meshName = "DebugCylinder.ply";
//        ::g_pDebugCylinder->bIsWireframe = true;
//        ::g_pDebugCylinder->bUseObjectDebugColour = true;
//        ::g_pDebugCylinder->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//        ::g_pDebugCylinder->bDontLight = true;
//        ::g_pDebugCylinder->scale = glm::vec3(1.0f);
//        ::g_pDebugCylinder->positionXYZ = glm::vec3(0.f, -100000.f, 0.f);
//        // Give this a friendly name
//        ::g_pDebugCylinder->friendlyName = "DebugCylinder";
//    }
//    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
//    glm::vec3 glFrom;
//    glFrom.x = from.x();
//    glFrom.y = from.y();
//    glFrom.z = from.z();
//    glm::vec3 glTo;
//    glTo.x = to.x();
//    glTo.y = to.y();
//    glTo.z = to.z();
//    sMatrixData matData;
//    matData.rotationMatrix = glm::lookAt(glFrom, glTo, up);
//    ::g_pDebugCylinder->positionXYZ = (glTo + glFrom) / 2.f;
//    matData.translateMatrix = glm::translate(glm::mat4(1.0f),
//        ::g_pDebugCylinder->positionXYZ);
//    matData.scaleMatrix = glm::scale(glm::mat4(1.0f), 
//                                        glm::vec3(1.f, 
//                                            (float)((glTo + glFrom).length()), 
//                                            1.f));
//
//    DrawObject(::g_pDebugCylinder, glm::mat4(1.f), ::g_matModel_location, ::g_inverseMatModel_location, ::g_currentProgram, ::g_pVAOManager, matData);
//}
//
//int gdpPhysics::iPhysicsWorldDrawDebug::getDebugMode() const
//{
//    return 0;
//}

cMesh* g_findObjectByUniqueID(unsigned int uniqueID_toFind)
{
    for ( unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++ )
    {
        if ( ::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind )
        {
            return ::g_vec_pMeshes[index];
        }
    }
    return NULL;
}

bool g_findObjectByUniqueID( unsigned int uniqueID_toFind, cMesh &theObjectIFound )
{
    for ( unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++ )
    {
        if ( ::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind )
        {
            // Dereferences and returns a stack based copy
            theObjectIFound = *(::g_vec_pMeshes[index]);
            return true;
        }
    }
    return false;
}

bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh* &theObjectIFound)
{
    for ( unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++ )
    {
        if ( ::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind )
        {
            // Returns a pointer to the object
            theObjectIFound = ::g_vec_pMeshes[index];
            return true;
        }
    }
    return false;
}

// If not found, return NULL (0)
cMesh* g_findObjectByFriendlyName(std::string NametoFind)
{
    for ( unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++ )
    {
        cMesh* pMeshToTest = ::g_vec_pMeshes[index];

        // Is functionally the same thing as using a reference:
        //cMesh& meshToTest = g_vecMeshes[index];

        if ( pMeshToTest->friendlyName == NametoFind )
        {
            return pMeshToTest;
        }
    }
    // DIDN'T find it, return NULL
    return NULL;
}

cMesh* g_FindObjectByUniqueID(unsigned int uniqueID_toFind)
{
    for ( unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++ )
    {
        cMesh* pMeshToTest = ::g_vec_pMeshes[index];

        // Is functionally the same thing as using a reference:
        //cMesh& meshToTest = g_vecMeshes[index];

        if ( pMeshToTest->getUniqueID() == uniqueID_toFind )
        {
            return pMeshToTest;
        }
    }
    // DIDN'T find it, return NULL
    return NULL;
}


