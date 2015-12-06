#pragma once
#include <OGRE\Ogre.h>
#include <Bullet\btBulletDynamicsCommon.h>
class Physics
{
public:
	Physics(Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* rw);
	~Physics();
	bool BuildCharacter();
	bool TestSkeleton();
	void Update(Ogre::Real frametime);

private:
	Ogre::SceneManager* mSceneMgr;
	btDynamicsWorld *mWorld;	// OgreBullet World
	//OgreBulletCollisions::DebugDrawer *debugDrawer;
	std::deque<btRigidBody *>         mBodies;
	std::deque<btCollisionShape *>  mShapes;
};

