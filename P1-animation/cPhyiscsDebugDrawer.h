#pragma once


#include "../PhysicsInterface/Headers/iPhysicsWorldDrawDebug.h"
#include "globalThings.h"

class cPhysicsDebugDrawer : public gdpPhysics::iPhysicsWorldDrawDebug
{
	int mCurrentMode = DebugDrawModes::DBG_NoDebug;
	GLuint mShaderProgram = 0, mBufferLoc = 0, mVAO = 0;
	GLint mMatModelLoc = -1, mMatViewLoc = -1, mMatProjLoc = -1;
	GLint mVposLoc = -1, mVcolLoc = -1;
public:

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	virtual int getDebugMode() const;

	virtual void reportErrorWarning(const char* warningString);

	virtual void draw3dText(const btVector3& location, const char* textString);

	virtual void setDebugMode(int debugMode);

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	void setMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix);

	void setUpLocations(GLuint program);
};