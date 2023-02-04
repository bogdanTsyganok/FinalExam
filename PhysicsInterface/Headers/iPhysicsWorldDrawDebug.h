#pragma once


#include <LinearMath/btIDebugDraw.h>

namespace gdpPhysics 
{
	//implement drawLine and getDebugMode at least
	class iPhysicsWorldDrawDebug : public btIDebugDraw
	{
		//necessary
		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) = 0;

		virtual void reportErrorWarning(const char* warningString) = 0;

		virtual void draw3dText(const btVector3& location, const char* textString) = 0;

		virtual void setDebugMode(int debugMode) = 0;
		//necessasry
		virtual int getDebugMode() const = 0;

		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) = 0;
	};
	//#define USE_SOFT_BODY_WORLD
}