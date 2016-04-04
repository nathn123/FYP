#pragma once
#include <OGRE\Ogre.h>
#include "OgreBulletDebugDrawer.h"
#include <Bullet\btBulletDynamicsCommon.h>
#include "Skeleton_Builder.h"
class Physics
{
public:


	Physics(Ogre::SceneManager* sceneMgr);
	~Physics();
	bool BuildCourse(std::string filename); // possible xml based courses
	bool BuildCharacter();
	bool TestBone();
	bool TestLimb();
	void TestUpdate();
	//bool AddSkeleton(Skeleton* skel);
	void Update(Ogre::Real frametime, bool step);

	std::vector<Muscle*> mMuscles;

private:
	Ogre::SceneManager* mSceneMgr;
	Skeleton_Builder* mBuilder;
	
	OgreDebugDrawer* mDrawer;
	Bone_Builder*mBoneBuilder;
	btDynamicsWorld *mWorld;	// OgreBullet World
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	std::deque<btRigidBody *>         mBodies;
	std::deque<btCollisionShape *>  mShapes;
};

