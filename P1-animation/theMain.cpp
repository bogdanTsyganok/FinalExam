#include "AssimpAnimation/cAssimpModel.h"

#include "GLCommon.h"
//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "globalThings.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>      // "string stream" just like iostream and fstream
#include <vector>       // "smart array"
#include <fstream>      // C++ file I-O library (look like any other stream)
#include "Animation/AnimationSystem.h"
#include "cBoneManager.h"
#include "cPhyiscsDebugDrawer.h"
#include "cInstanceManager.h"
#include "cGunManager.h"
#include "cEnemyManager.h"

#include "AssimpAnimation/animationController.h"

// Are in globalThings.h
//#include "cMesh.h"
//#include "cVAOManager.h"
//#include "cShaderManager.h"
//
//#include "cLightManager.h"
//#include "cLightHelper.h"

// Function signature for DrawObject()

float triggertimer = 0.f;
bool shotFired = false;
int holeCounter = 0;

cInstanceManager instanceManager;

AnimationController animationController;

cEnemyManager enemyManager(&animationController);

void DrawDebugObjects(
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager);

float distanceToCamera(glm::vec3 pos)
{
    return glm::distance(::g_pFlyCamera->getEye(), pos);
}

btCollisionWorld::ClosestRayResultCallback RaycastTest(glm::vec3 from, glm::vec3 to);
cGunManager* gunManager;

int main(void)
{
    GLFWwindow* pWindow;

//    GLuint vertex_buffer = 0;     // ** NOW in VAO Manager **

//    GLuint vertex_shader;     // Now in the "Shader Manager"
//    GLuint fragment_shader;   // Now in the "Shader Manager"
    GLuint program = 0;     // 0 means "no shader program"
    GLuint debugProgram = 0;
    GLint mvp_location = -1;        // Because glGetAttribLocation() returns -1 on error
//    GLint vpos_location = -1;       // Because glGetAttribLocation() returns -1 on error
//    GLint vcol_location = -1;       // Because glGetAttribLocation() returns -1 on error

    glfwSetErrorCallback(GLFW_error_callback);

    if ( ! glfwInit() )
    {
        return -1;
        //exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    pWindow = glfwCreateWindow(1200, 640, "OpenGL is great!", NULL, NULL);

    if (!pWindow )
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(pWindow, GLFW_key_callback);
    // NEW: 
    glfwSetCursorEnterCallback( pWindow, GLFW_cursor_enter_callback );
    glfwSetCursorPosCallback( pWindow, GLFW_cursor_position_callback );
    glfwSetScrollCallback( pWindow, GLFW_scroll_callback );
    glfwSetMouseButtonCallback( pWindow, GLFW_mouse_button_callback );
    glfwSetWindowSizeCallback( pWindow, GLFW_window_size_callback );

    glfwMakeContextCurrent(pWindow);
    gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);


    // void glGetIntegerv(GLenum pname, GLint * data);
    // See how many active uniform variables (registers) I can have
    GLint max_uniform_location = 0;
    GLint* p_max_uniform_location = NULL;
    p_max_uniform_location = &max_uniform_location;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

    // You'll actually see something like this:
//    GLint max_uniform_location = 0;
//    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max_uniform_location);

    std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

    // Create global things
    ::g_StartUp(pWindow);

    ::g_pFlyCamera->setEye( glm::vec3(0.0f, 0.0f, 0.0f) ); //102.0f, 225.0f
//    ::g_pFlyCamera->setEye( glm::vec3(0.0f, 5000.0f, -3000.0f) );


    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";
        
    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
        ::g_currentProgram = program;
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }

    cShaderManager::cShader vertDebugShader;
    vertDebugShader.fileName = "assets/shaders/vertShader_debug.glsl";

    cShaderManager::cShader fragDebugShader;
    fragDebugShader.fileName = "assets/shaders/fragShader_debug.glsl";

    if (::g_pShaderManager->createProgramFromFile("ShaderDebug", vertDebugShader, fragDebugShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        debugProgram = ::g_pShaderManager->getIDFromFriendlyName("ShaderDebug");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }


    // Select the shader program we want to use
    // (Note we only have one shader program at this point)


    // ************************************************
    // Now, I'm going to "load up" all the uniform locations
    // (This was to show how a map could be used)
    cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

    int theUniformIDLoc = -1;
    theUniformIDLoc = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = theUniformIDLoc;
    
    // Or...
    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matView"] = glGetUniformLocation(program, "matView");
    pShaderProc->mapUniformName_to_UniformLocation["matProjection"] = glGetUniformLocation(program, "matProjection");
    pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"] = glGetUniformLocation(program, "matModelInverseTranspose");

    pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
    // .. and so on...
    pShaderProc->mapUniformName_to_UniformLocation["theLights[0].position"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
    // ... and so on..
    // ************************************************

    //GLint mvp_location = -1;
    mvp_location = glGetUniformLocation(program, "MVP");

    // Get "uniform locations" (aka the registers these are in)
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    ::g_matModel_location = matModel_Location;
//    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");
    ::g_inverseMatModel_location = matModelInverseTranspose_Location;

    ::g_pTheLights->theLights[0].position = glm::vec4(100.0f, 10000.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].atten.y = 0.000001f;
    ::g_pTheLights->theLights[0].atten.z = 0.0000001f;
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//    
    ::g_pTheLights->theLights[0].param1.x = 2.0f;    // directional
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(0.9922f, 0.9843f, 0.9275f, 1.0f);
    ::g_pTheLights->theLights[0].param2.x = 1.f;
    //::g_pTheLights->theLights[0].diffuse *= 0.0001f;
    //::g_pTheLights->TurnOnLight(0);  // Or this!
    //gTheLights.theLights[0].param2.x = 1.0f;
    //::g_pTheLights->TurnOnLight(0);  // Or this!
    // Sunlight: https://encycolorpedia.com/fdfbd3#:~:text=The%20hexadecimal%20color%20code%20%23fdfbd3,%25%20saturation%20and%2091%25%20lightness.
    //The hexadecimal color code #fdfbd3 is a very light shade of yellow. In the RGB color model #fdfbd3 is comprised of 99.22% red, 98.43% green and 82.75% blue. 


    // Get the uniform locations of the light shader values
    ::g_pTheLights->SetUpUniformLocations(program);

    
    // Set up the debug sphere object
    ::g_pDebugSphere = new cMesh();
    ::g_pDebugSphere->meshName = "Sphere_xyz_n_rgba.ply";
    ::g_pDebugSphere->bIsWireframe = true;
    ::g_pDebugSphere->bUseObjectDebugColour = true;
    ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pDebugSphere->bDontLight = true;
    ::g_pDebugSphere->scale = glm::vec3(1.0f);
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // Give this a friendly name
    ::g_pDebugSphere->friendlyName = "Debug Sphere";
    // Then I could make a small function that searches for that name


    


    ::g_pVAOManager->setFilePath("assets/models/");

    std::vector<std::string> vecModelsToLoad;
    //vecModelsToLoad.push_back("Sphere_xyz_n_rgba.ply");
    //vecModelsToLoad.push_back("Lamps.ply");
    vecModelsToLoad.push_back("carpet.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("Sphere_xyz_n_rgba.ply");
    vecModelsToLoad.push_back("Cylinder.ply");
    vecModelsToLoad.push_back("cube.ply");
    vecModelsToLoad.push_back("revolver.ply");
    vecModelsToLoad.push_back("quad2.ply");
    vecModelsToLoad.push_back("quad1000.ply");
    vecModelsToLoad.push_back("quad1000.ply");
    vecModelsToLoad.push_back("revolver.ply");

    vecModelsToLoad.push_back("Isosphere_Smooth_Normals.ply");
    vecModelsToLoad.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
    vecModelsToLoad.push_back("4-tier-cone.ply");
    vecModelsToLoad.push_back("cone.ply");




    unsigned int totalVerticesLoaded = 0;
    unsigned int totalTrianglesLoaded = 0;
    for (std::vector<std::string>::iterator itModel = vecModelsToLoad.begin(); itModel != vecModelsToLoad.end(); itModel++)
    {
        sModelDrawInfo theModel;
        std::string modelName = *itModel;
        std::cout << "Loading " << modelName << "...";
        if ( !::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program) )
        {
            std::cout << "didn't work because: " << std::endl;
            std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
        }
        else
        {
            std::cout << "OK." << std::endl;
            std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
            totalTrianglesLoaded += theModel.numberOfTriangles;
            totalVerticesLoaded += theModel.numberOfVertices;
        }

//        ::g_pVAOManager->LoadPLYModelFromFile(modelName, theModel);
//        ::g_pVAOManager->GenerateSphericalTextureCoords(theModel, false, 1.0f, false);
//        ::g_pVAOManager->LoadModelIntoVAO(theModel, program);

    }//for (std::vector<std::string>::iterator itModel


    std::cout << "Done loading models." << std::endl;
    std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
    std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;

    ::physicsWorld->SetGravity(glm::vec3(0.f, 0.f, 0.f));

    std::vector<sMeshRbConstruct*> spheres;
    for (int i = 0; i < 10; i++)
    {
        cMesh* pSphere = new cMesh();
        pSphere->meshName = "Sphere_xyz_n_rgba.ply";
        pSphere->positionXYZ = glm::vec3(-40.f + 20.f * i, 1.f, 95.f); 
        pSphere->scale = glm::vec3(5.f);
        pSphere->textureRatios[0] = pSphere->textureRatios[1] = pSphere->textureRatios[2] = pSphere->textureRatios[3] = 0.f;
        ::g_vec_pMeshes.push_back(pSphere);

        gdpPhysics::cRigidBodyDesc sphereRBDesc;
        sphereRBDesc.mass = 1.f;
        sphereRBDesc.position = glm::vec3(5.f + 20.f * i, 8.f, 95.f);
        gdpPhysics::iRigidBody* sphereRB = ::physicsFactory->CreateRigidBody(sphereRBDesc, new gdpPhysics::SphereShape(5.f));
        glm::vec3 randVec = glm::vec3(rand() - rand() * 2, 0.f, rand() - rand() * 2);
        randVec = glm::normalize(randVec) * 500.f;
        sphereRB->ApplyImpulse(randVec);
        ::physicsWorld->AddBody(sphereRB);

        sMeshRbConstruct* rbSphereConstruct = new sMeshRbConstruct();
        rbSphereConstruct->_mesh = pSphere;
        rbSphereConstruct->_rb = sphereRB;
        g_physicsManager.addConstruct(rbSphereConstruct);
        spheres.push_back(rbSphereConstruct);
        
    }

    gdpPhysics::cRigidBodyDesc planeRBDesc;
    planeRBDesc.position = glm::vec3(0.f, -32.f, 0.f);
    planeRBDesc.rotation = glm::quat(1.0f, 0.f, 0.f, 0.f);
    planeRBDesc.mass = 0.f;
    planeRBDesc.isStatic = true;
    gdpPhysics::iRigidBody* planeRB = ::physicsFactory->CreateRigidBody(planeRBDesc, new gdpPhysics::PlaneShape(0.f, glm::vec3(0.f, 1.f, 0.f)));
    ::physicsWorld->AddBody(planeRB);

    cMesh* pCover = new cMesh();
    pCover->meshName = "quad1000.ply";
    pCover->positionXYZ = glm::vec3(0.f, -35.f, 115.f);
    pCover->orientationXYZ.x = glm::radians(90.f);
    pCover->textureRatios[0] = 1.f;
    pCover->textureNames[0] = "grass.bmp";
    pCover->bDontLight = true;
    //pCover->textureRatios[3] = 0.f;
    pCover->friendlyID = 42069;
    ::g_vec_pMeshes.push_back(pCover);
    
    for (int i = 0; i < 4; i++)
    {
        cMesh* pWall = new cMesh();
        float positionX, positionZ;
        switch (i)
        {
        case 0:
            positionX = 0.f;
            positionZ = 500.f;
            pWall->orientationXYZ.z = 0.f;
            pWall->orientationXYZ.x = glm::radians(-90.f);
            break;
        case 1:
            positionX = 500.f;
            positionZ = 0.;
            pWall->orientationXYZ.z = glm::radians(90.f);
            pWall->orientationXYZ.x = 0.f;
            break;
        case 2:
            positionX = 0.f;
            positionZ = -500.f;
            pWall->orientationXYZ.z = 0.f;
            pWall->orientationXYZ.x = glm::radians(90.f);
            break;
        case 3:
            positionX = -500.f;
            positionZ = 0.f;
            pWall->orientationXYZ.z = glm::radians(-90.f);
            pWall->orientationXYZ.x = 0.f;
            break;
        }
        pWall->meshName = "quad2.ply";
        pWall->positionXYZ = glm::vec3(positionX, 70.f, positionZ);
        pWall->textureRatios[1] = 1.f;
        pWall->textureNames[0] = "brick.bmp";
        //pCover->textureNames[3] = "noise.bmp";
        //pCover->textureRatios[3] = 0.f;
        pWall->scale = glm::vec3(500.f);
        pWall->scale.x = 1000.f;
        if ((i % 2) == 1)
        {
            pWall->orientationXYZ.y = glm::radians(90.f);
        }
        pWall->bDontLight = true;
        ::g_vec_pMeshes.push_back(pWall);

        gdpPhysics::cRigidBodyDesc planeRBDesc;
        planeRBDesc.position = pWall->positionXYZ;
        planeRBDesc.rotation = glm::quat(pWall->orientationXYZ);
        planeRBDesc.mass = 0.f;
        planeRBDesc.isStatic = true;
        gdpPhysics::iRigidBody* planeRB = ::physicsFactory->CreateRigidBody(planeRBDesc, new gdpPhysics::PlaneShape(0.f, glm::vec3(0.f, 1.f, 0.f)));
        ::physicsWorld->AddBody(planeRB);
    }

    cMesh* pGun = new cMesh();
    pGun->meshName = "revolver.ply";
    pGun->positionXYZ = glm::vec3(-2.32387, -3.30633, 15.3391);
    pGun->scale = glm::vec3(0.01f);
    pGun->bDontLight = true;
    pGun->textureNames[0] = "gunTexture.bmp";
    pGun->textureRatios[0] = 1.f;
    pGun->friendlyName = "gun";
    ::g_vec_pMeshes.push_back(pGun);


    int maxHoles = 10;
    gunManager = new cGunManager(pGun, &instanceManager, maxHoles, &enemyManager);
    std::vector<asModel*> knights;
    for (int i = 0; i < 4; i++)
    {
        asModel* Knight1 = new asModel("assets/models/Knight_Golden_Male.fbx", program);
        Knight1->mPosition = glm::vec3(-30.f + 20.f * i, -30.f, 50.f );
        Knight1->mScale = glm::vec3(.1f, .1f, .1f);
        Knight1->mOrientation = glm::vec3(0.f, 3.2f, 0.f);
        Knight1->bUseWholeObjectDebug = false;
        Knight1->wholeObjectDebugColour = glm::vec4(0.7f, 0.2f, 0.f, 1.f);

        sAssimpRbConstruct* knightConstruct = new sAssimpRbConstruct();

        {

            gdpPhysics::cRigidBodyDesc knightRBDesc;
            knightRBDesc.mass = 1.f;
            knightRBDesc.position = glm::vec3(-80.f + 40.f * i, -27.f, 235.f );//102.0f, 225.0f
            knightRBDesc.rotation = glm::vec3(0.f, 0.f, 0.f);
            gdpPhysics::iRigidBody* knightRB = ::physicsFactory->CreateRigidBody(knightRBDesc, new gdpPhysics::BoxShape(glm::vec3(5.f, 15.f, 5.f)));
            ::physicsWorld->AddBody(knightRB);
            knightConstruct->_rb = knightRB;
            knightRB->SetUserPointer(Knight1);
        }

        knightConstruct->_model = Knight1;

        ::g_physicsManager.addConstruct(knightConstruct);
        sEnemy enemStruct;
        enemStruct.construct = knightConstruct;
        enemStruct.dieCmd = "die" + std::to_string(i);
        enemStruct.attackCmd = "attack" + std::to_string(i);
        enemStruct.state = "alive";
        enemyManager.AddEnemy(enemStruct);

        animationController.loadAnimation("assets/models/Knight_Golden_Male.fbx", "idle", Knight1, 8);
        animationController.loadAnimation("assets/models/Knight_Golden_Male.fbx", "punch", Knight1, 5);
        animationController.loadAnimation("assets/models/Knight_Golden_Male.fbx", "dying", Knight1, 0);

        sAnimationControllerInfo knightAnimInfo;
        knightAnimInfo.animationName = "idle";
        knightAnimInfo.type = sAnimationControllerInfo::eStateType::command;
        knightAnimInfo.stringArray.push_back(enemStruct.attackCmd);
        knightAnimInfo.floatData[0] = 1.f;
        AnimatedState* idleState = animationController.createState(knightAnimInfo);
        knightAnimInfo.animationName = "punch";
        knightAnimInfo.stringArray[0] = enemStruct.dieCmd;
        knightAnimInfo.floatData[0] = 1.f;
        AnimatedState* attackState = animationController.createState(knightAnimInfo);
        knightAnimInfo.animationName = "dying";
        knightAnimInfo.stringArray[0] = "wow";
        knightAnimInfo.floatData[0] = 0.f;
        AnimatedState* dyingState = animationController.createState(knightAnimInfo);

        idleState->setNextState(attackState);
        attackState->setNextState(dyingState);
        //frazzledState->setNextState(idleState);
        animationController.addState(Knight1->GetId(), idleState);
        knights.push_back(Knight1);
    }

    cMesh* instancedHoles = new cMesh();
    instancedHoles->meshName = "quad2.ply";
    instancedHoles->bDontLight = true;
    instancedHoles->textureNames[0] = "bullethole.bmp";
    instancedHoles->textureRatios[0] = 1.f;
    instancedHoles->textureNames[2] = "bullethole.bmp";
    /*instancedHoles->orientationXYZ = instancedHoles->positionXYZ = glm::vec3(0.f, 0.f, 0.f);
    instancedHoles->scale = glm::vec3(10.f);
    instancedHoles->friendlyName = "holes";*/
    //g_vec_pMeshes.push_back(instancedHoles);
    instanceManager.AddMesh(instancedHoles, "bulletHoles", maxHoles);

    for (int i = 0; i < maxHoles; i++)
    {
        glm::vec3 pos = glm::vec3(20.f - (i * 15.f), 0.f, 0.f);
        glm::vec3 rot = glm::vec3(.0f);
        glm::vec3 sca = glm::vec3(1.f);
        instanceManager.SetPosition("bulletHoles", i, pos);
        instanceManager.SetRotation("bulletHoles", i, rot);
        instanceManager.SetScale("bulletHoles", i, sca);
    }

    GLint glMaxCombinedTextureImageUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &glMaxCombinedTextureImageUnits);
    std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << glMaxCombinedTextureImageUnits << std::endl;

    GLint glMaxVertexTextureImageUnits = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &glMaxVertexTextureImageUnits);
    std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << glMaxVertexTextureImageUnits << std::endl;

    GLint glMaxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTextureSize);
    std::cout << "GL_MAX_TEXTURE_SIZE = " << glMaxTextureSize << std::endl;
   
    
    // Load the textures
    ::g_pTextureManager->SetBasePath("assets/textures");


    ::g_pTextureManager->Create2DTextureFromBMPFile("grass.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("bullethole.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("gunTexture.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("brick.bmp", true);

    // Add a skybox texture
    std::string errorTextString;

    ::g_pTextureManager->SetBasePath("assets/textures/cubemaps");
    //if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Indoors01",
    //    "SpaceBox_right1_posX.bmp",     /* posX_fileName */
    //    "SpaceBox_left2_negX.bmp",      /*negX_fileName */
    //    "SpaceBox_top3_posY.bmp",       /*posY_fileName*/
    //    "SpaceBox_bottom4_negY.bmp",    /*negY_fileName*/
    //    "SpaceBox_front5_posZ.bmp",     /*posZ_fileName*/
    //    "SpaceBox_back6_negZ.bmp",      /*negZ_fileName*/
    //    true, errorTextString))
    //{
    //    std::cout << "Didn't load because: " << errorTextString << std::endl;
    //}
    //else
    //{
    //    std::cout << "Loaded the sunny day cube texture OK" << std::endl;
    //}    
  
    std::cout << "I've loaded these textures:" << std::endl;
    std::vector<std::string> vecTexturesLoaded;
    ::g_pTextureManager->GetLoadedTextureList(vecTexturesLoaded);

    for (std::vector<std::string>::iterator itTexName = vecTexturesLoaded.begin();
        itTexName != vecTexturesLoaded.end(); itTexName++)
    {
        std::cout << "\t" << *itTexName << std::endl;
    }

//    ::g_vec_pMeshes.push_back(pDebugSphere);     // Debug sphere is #5

    const double MAX_DELTA_TIME = 0.1;  // 100 ms
    double previousTime = glfwGetTime();

    float offX = 0.f, offY = 0.f;


    cPhysicsDebugDrawer* debugDrawer = new cPhysicsDebugDrawer();
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawAabb);
    debugDrawer->setUpLocations(debugProgram);
    ::physicsWorld->RegisterDebugDrawer(debugDrawer);

    glm::mat4 gunMatProjection;
    glm::mat4 gunMatView;
    glm::mat4 gunMatModel;
    {
        int width, height;

        glfwGetFramebufferSize(pWindow, &width, &height);
        float ratio = width / (float)height;
        
        gunMatProjection = glm::perspective(
            ::g_pFlyCamera->FOV,
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)
        gunMatView = glm::lookAt(::g_pFlyCamera->getEye(),   // "eye"
                            ::g_pFlyCamera->getAtInWorldSpace(),    // "at"
                            ::g_pFlyCamera->getUpVector());
    }
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    while (!glfwWindowShouldClose(pWindow))
    {
        float ratio;
        int width, height;
        glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
        glm::mat4 matProjection;        // used to be "p";
        glm::mat4 matView;              // used to be "v";

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
        previousTime = currentTime;



        glUseProgram(program);

        g_physicsManager.updateConstructs();

        glfwGetFramebufferSize(pWindow, &width, &height);
        ratio = width / (float)height;

        // Turn on the depth buffer
        glEnable(GL_DEPTH);         // Turns on the depth buffer
        glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

        glViewport(0, 0, width, height);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glClearColor(0.3f, 0.3f, 0.8f, 1.f);

        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************

        // Update the title text
        glfwSetWindowTitle(pWindow, ::g_TitleText.c_str() );

        // Copy the light information into the shader to draw the scene
        ::g_pTheLights->CopyLightInfoToShader();

        // Place the "debug sphere" at the same location as the selected light (again)
        ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
        // HACK: Debug sphere is 5th item added
//        ::g_vecMeshes[5].positionXYZ = gTheLights.theLights[0].position;

        matProjection = glm::perspective(
            ::g_pFlyCamera->FOV,       
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

        //matProjection = glm::perspective(
        //    0.6f,       // in degrees
        //    ratio,
        //    10.0f,       // Near plane (as large as possible)
        //    1'000'000.0f);   // Far plane (as small as possible)

        ::g_pFlyCamera->Update(deltaTime);


        animationController.update(deltaTime, program);
        enemyManager.Update(deltaTime, ::g_pFlyCamera->getEye());

        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();
        glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();


        matView = glm::mat4(1.0f);
        matView = glm::lookAt( cameraEye,   // "eye"
                               cameraAt,    // "at"
                               cameraUp );

//        // HACK: Black hole discard example
//        ::g_blackHoleSize_for_discard_example += 1.0f;
//        GLint blackHoleSize_LocID = glGetUniformLocation(program, "blackHoleSize");
//        glUniform1f(blackHoleSize_LocID, ::g_blackHoleSize_for_discard_example);
//        std::cout << ::g_blackHoleSize_for_discard_example << std::endl;


//        glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(matView));

        cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

        glUniformMatrix4fv( pShaderProc->getUniformID_From_Name("matView"),
                            1, GL_FALSE, glm::value_ptr(matView));


        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));


        // *********************************************************************
        //    ____  _            ____             
        //   / ___|| | ___   _  | __ )  _____  __ 
        //   \___ \| |/ / | | | |  _ \ / _ \ \/ / 
        //    ___) |   <| |_| | | |_) | (_) >  <  
        //   |____/|_|\_\\__, | |____/ \___/_/\_\ 
        //               |___/                    
        //
       // GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
       // glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);
       // GLint bUseHeightMap_LodID = glGetUniformLocation(program, "bUseHeightMap");
       // glUniform1f(bUseHeightMap_LodID, (GLfloat)GL_FALSE);
       // 
       // glDisable( GL_DEPTH_TEST );
       // // Move the "skybox object" with the camera
       // pSkyBox->positionXYZ = ::g_pFlyCamera->getEye();
       ///* DrawObject(
       //     pSkyBox, glm::mat4(1.0f),
       //     matModel_Location, matModelInverseTranspose_Location,
       //     program, ::g_pVAOManager);*/

       // glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
       // //
       // glEnable( GL_DEPTH_TEST );
        // *********************************************************************
        if (triggerPulled)
        {
            triggertimer += deltaTime;
            if (!shotFired)
            {
                gunManager->Fire();
                shotFired = true;
            }
            if (triggertimer > 0.3f)
            {
                triggerPulled = false;
                shotFired = false;
                triggertimer = 0.f;
            }
        }
        glUseProgram(program);
    


        // Set up the discard texture, etc.

        {

            bool swapp = true;
            while (swapp)
            {
                swapp = false;
                for (int i = 0; i < ::g_vec_pMeshes.size() - 1; i++)
                {
                    if (distanceToCamera(::g_vec_pMeshes[i]->positionXYZ) < distanceToCamera(::g_vec_pMeshes[i+1]->positionXYZ))
                    {
                        cMesh* temp = ::g_vec_pMeshes[i];
                        ::g_vec_pMeshes[i] = ::g_vec_pMeshes[i + 1];
                        ::g_vec_pMeshes[i + 1] = temp;
                        swapp = true;
                    }
                }
            }
        }

        // **********************************************************************
        // Draw the "scene" of all objects.
        // i.e. go through the vector and draw each one...
        // **********************************************************************
        for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

            if (::g_vec_pMeshes[index]->friendlyName == "gun")
                continue;

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            {
                GLint bUseHeightMap_LodID = glGetUniformLocation(program, "bUseHeightMap");
                glUniform1f(bUseHeightMap_LodID, (GLfloat)GL_FALSE);

                if (pCurrentMesh->friendlyName == "water")
                {

                    // I'm picking texture unit 31 since it's not in use.
                    GLuint heightMapTextureUnit = 31;			// Texture unit go from 0 to 79
                    glActiveTexture(heightMapTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

                    GLuint heightMapTextureID = ::g_pTextureManager->getTextureIDFromName("noise.bmp");
                    glBindTexture(GL_TEXTURE_2D, heightMapTextureID);

                    GLint heightMapTexture_LocID = glGetUniformLocation(program, "heightMapTexture");
                    glUniform1i(heightMapTexture_LocID, heightMapTextureUnit);


                    GLint heightMapScale_LocID = glGetUniformLocation(program, "heightMapScale");
                    glUniform1f(heightMapScale_LocID, 0.8f);

                    GLint heightMapUVOffsetRotation_LocID = glGetUniformLocation(program, "heightMapUVOffsetRotation");
                    // Move to a global variable so we can move them with the keyboard
                    //glm::vec2 heightMapUVOffset = glm::vec2(0.0f, 0.0f);
                    glUniform3f(heightMapUVOffsetRotation_LocID,
                        ::g_heightMapUVOffsetRotation.x, ::g_heightMapUVOffsetRotation.y, ::g_heightMapUVOffsetRotation.z);

                    // Turn height map function on (will be turned off at next object - line 778)
                    glUniform1f(bUseHeightMap_LodID, (GLfloat)GL_TRUE);


                    GLint heightMapOffset_ID = glGetUniformLocation(program, "heightMapOffset");
                    glUniform2f(heightMapOffset_ID, offX, offY);
                    offX += 0.05f * deltaTime;
                    offY += 0.05f * deltaTime;
//                    std::cout << "heightMapTextureID = " << heightMapTextureID << std::endl;
//                    std::cout << "heightMapTextureUnit = " << heightMapTextureUnit << std::endl;
//                    std::cout << "heightMapTextureUnit + GL_TEXTURE0 = " << heightMapTextureUnit + GL_TEXTURE0 << std::endl;
//
//                    // returns a single value indicating the active multitexture unit. The initial value is GL_TEXTURE0.
//                    GLint glActiveTexture = 0;
//                    glGetIntegerv(GL_ACTIVE_TEXTURE, &glActiveTexture);
//                    std::cout << "GL_ACTIVE_TEXTURE = " << glActiveTexture << std::endl;
//                    std::cout << "GL_ACTIVE_TEXTURE - GL_TEXTURE0 = " << glActiveTexture - GL_TEXTURE0 << std::endl;
//
//                    // returns a single value, the name of the texture currently bound to the target GL_TEXTURE_2D. The initial value is 0.
//                    GLint glTextureBindings = 0;
//                    glGetIntegerv(GL_TEXTURE_BINDING_2D, &glTextureBindings);
//                    std::cout << "GL_TEXTURE_BINDING_2D = " << glTextureBindings << std::endl;

                }//if (pCurrentMesh->friendlyName == "FlatTerrain")
            }
// ***************************************************

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);


        }//for (unsigned int index
        // Scene is drawn
        // **********************************************************************

        for (sMeshRbConstruct* pSphere : spheres)
        {
            glm::vec3 pos;
            pSphere->_rb->GetPosition(pos);
            if (pos.y > 100.f)
            {
                glm::vec3 force(rand() - rand() * 2, -2.f, rand() - rand() * 2);
                pSphere->_rb->ApplyForce(glm::normalize(force) * 50.f);
            }
        }

        for (asModel* knight : knights)
        {
            animationController.loadMatricies(knight->GetId(), program);
            knight->Draw(glm::mat4(1.0f),
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);
        }
        DrawObject(instanceManager.GetMesh("bulletHoles"), matModel, matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);


        glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
            1, GL_FALSE, glm::value_ptr(gunMatView));


        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(gunMatProjection));

        DrawObject(pGun,
            matModel,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            ::g_pVAOManager);

        //glm::vec3 at = g_pFlyCamera->getAtInWorldSpace();
        //RaycastTest(::g_pFlyCamera->eye, at + ((at - ::g_pFlyCamera->eye)* 1000.f));

        //glUniform1f(bUseHeightMap_LodID, (GLfloat)GL_FALSE);
        //DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);

        //{
        //    glm::vec3 from = ::g_pFlyCamera->eye;
        //    glm::vec3 at = g_pFlyCamera->getAtInWorldSpace();
        //    glm::vec3 to = from + ((at - ::g_pFlyCamera->eye) * 1000.f);
        //    btVector3 btFrom(from.x, from.y, from.z);
        //    btVector3 btTo(to.x, to.y, to.z);
        //    glm::vec3 offset = glm::normalize(glm::cross(g_pFlyCamera->getAt(), g_pFlyCamera->getUp())) * 2.f + glm::normalize(g_pFlyCamera->getAt()) * 10.f - glm::normalize(g_pFlyCamera->getUp()) * 1.f;
        //    btVector3 btOffset(offset.x, offset.y, offset.z);
        //    btDiscreteDynamicsWorld* worldPointer = (btDiscreteDynamicsWorld*)::physicsWorld->GetDynamicsWorld();
        //    worldPointer->getDebugDrawer()->drawLine(btFrom + btOffset, btTo, btVector3(1.f, 0.f, 0.f));//+ btOffset
        //}


        debugDrawer->setMatrices(matView, matProjection);
        ::physicsWorld->TimeStep(deltaTime);

        // "Present" what we've drawn.
        glfwSwapBuffers(pWindow);        // Show what we've drawn

        // Process any events that have happened
        glfwPollEvents();

        // Handle OUR keyboard, mouse stuff
        handleAsyncKeyboard(pWindow, deltaTime);
        handleAsyncMouse(pWindow, deltaTime);

    }//while (!glfwWindowShouldClose(window))

    // All done, so delete things...
    ::g_ShutDown(pWindow);
    delete gunManager;
    /*delete dieAnimation;
    delete monsterAnimation;
    delete heroKnockdownAnim;
    delete heroRemoveAnim;
    delete animationController;*/
    
    

    glfwDestroyWindow(pWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


btCollisionWorld::ClosestRayResultCallback RaycastTest(glm::vec3 from, glm::vec3 to)
{
    btVector3 btFrom(from.x, from.y, from.z);
    btVector3 btTo(to.x, to.y, to.z);
    btCollisionWorld::ClosestRayResultCallback callback(btFrom, btTo);
    btDiscreteDynamicsWorld* worldPointer = (btDiscreteDynamicsWorld*)::physicsWorld->GetDynamicsWorld();
    worldPointer->rayTest(btFrom, btTo, callback);
    if (callback.hasHit())
    {
        btVector3 p = btFrom.lerp(btTo, callback.m_closestHitFraction);
        worldPointer->getDebugDrawer()->drawSphere(p, 0.1, btVector3(0.f, 1.f, 0.f));
        worldPointer->getDebugDrawer()->drawLine(p, p + (callback.m_hitNormalWorld * 100.f), btVector3(0.f, 1.f, 0.f));

    }
    return callback;
}


void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        triggerPulled = true;

    return;
}