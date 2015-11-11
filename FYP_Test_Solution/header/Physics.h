#pragma once
#include "OgreBulletDynamicsRigidBody.h"
class Physics
{
public:
	Physics();
	~Physics();
private:

	OgreBulletDynamics::DynamicsWorld *mWorld;	// OgreBullet World
	OgreBulletCollisions::DebugDrawer *debugDrawer;
	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
};

