#include "cPhyiscsDebugDrawer.h"
#include <glm/gtc/type_ptr.hpp>


float vertices[6];

void cPhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{

    vertices[0] = from.x();
    vertices[1] = from.y();
    vertices[2] = from.z();
    vertices[3] = to.x();
    vertices[4] = to.y();
    vertices[5] = to.z();
    glUseProgram(this->mShaderProgram);
    glPushMatrix();
    {
        glBindVertexArray(this->mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->mBufferLoc);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(this->mVposLoc);
        glVertexAttribPointer(this->mVposLoc, 3, GL_FLOAT, GL_FALSE,
            3 * sizeof(float), (void*)0);
        glUniform4f(this->mVcolLoc, color.x(), color.y(), color.z(), 1.0f);


        glPointSize(2.0f);
        glDrawArrays(GL_POINTS, 0, 2);
        glDrawArrays(GL_LINES, 0, 2);
    
        glDisableVertexAttribArray(this->mVposLoc);
        glBindVertexArray(0);
    }
    glPopMatrix();
    //if (::g_pDebugCylinder == NULL)
    //{
    //    ::g_pDebugCylinder = new cMesh();
    //    ::g_pDebugCylinder->meshName = "DebugCylinder.ply";
    //    ::g_pDebugCylinder->bIsWireframe = true;
    //    ::g_pDebugCylinder->bUseObjectDebugColour = true;
    //    ::g_pDebugCylinder->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //    ::g_pDebugCylinder->bDontLight = true;
    //    ::g_pDebugCylinder->scale = glm::vec3(1.0f);
    //    ::g_pDebugCylinder->positionXYZ = glm::vec3(0.f, -100000.f, 0.f);
    //    // Give this a friendly name
    //    ::g_pDebugCylinder->friendlyName = "DebugCylinder";
    //}
    //glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    //glm::vec3 glFrom;
    //glFrom.x = from.x();
    //glFrom.y = from.y();
    //glFrom.z = from.z();
    //glm::vec3 glTo;
    //glTo.x = to.x();
    //glTo.y = to.y();
    //glTo.z = to.z();
    //sMatrixData matData;
    //matData.rotationMatrix = glm::lookAt(glFrom, glTo, up);
    //::g_pDebugCylinder->positionXYZ = (glTo + glFrom) / 2.f;
    //matData.translateMatrix = glm::translate(glm::mat4(1.0f),
    //    ::g_pDebugCylinder->positionXYZ);
    //matData.scaleMatrix = glm::scale(glm::mat4(1.0f),
    //    glm::vec3(1.f,
    //        (float)((glTo + glFrom).length()),
    //        1.f));

    //DrawObject(::g_pDebugCylinder, glm::mat4(1.f), ::g_matModel_location, ::g_inverseMatModel_location, ::g_currentProgram, ::g_pVAOManager, matData);

}

int cPhysicsDebugDrawer::getDebugMode() const
{
    return this->mCurrentMode;
}

void cPhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
}

void cPhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void cPhysicsDebugDrawer::setDebugMode(int debugMode)
{
    this->mCurrentMode = debugMode;    
}

void cPhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void cPhysicsDebugDrawer::setMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix)
{
    glUseProgram(this->mShaderProgram); // Use shader program bound
    glUniformMatrix4fv(this->mMatProjLoc, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv(this->mMatViewLoc, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
}

void cPhysicsDebugDrawer::setUpLocations(GLuint program)
{
    this->mShaderProgram = program;
    this->mVcolLoc = glGetUniformLocation(program, "vVertexColor");
    this->mVposLoc = 0;
    this->mMatProjLoc = glGetUniformLocation(program, "matProjection");
    this->mMatViewLoc = glGetUniformLocation(program, "matView");
    glUseProgram(this->mShaderProgram);
    glGenVertexArrays(1, &(this->mVAO));
    glBindVertexArray(this->mVAO);
    glGenBuffers(1, &(this->mBufferLoc));
    glBindVertexArray(0);
    glUseProgram(0);
}
