#include "..\header\Physics.h"
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>

Physics::Physics(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
	solver = new btSequentialImpulseConstraintSolver;
	mWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	mWorld->setGravity(btVector3(0, -9.81, 0));

	// add ground

	// create skeleton builder
	mBuilder = new Skeleton_Builder(mSceneMgr, mWorld);
}


Physics::~Physics()
{
	delete mWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}

bool Physics::BuildCharacter()
{
	Skeleton* skel = nullptr; // initialised one bones created
	// just for testing
	mBuilder->SetDimensions(15, 15, 100, 40);
	mBuilder->SetBodyType(Skeleton_Builder::LegType::Uninverted, Skeleton_Builder::ArmType::LongArms, Skeleton_Builder::TorsoType::LongNeckLongTail);
	return mBuilder->BuildSkeleton(skel);
}

void Physics::Update(Ogre::Real frametime)
{
	mWorld->stepSimulation(frametime);
}