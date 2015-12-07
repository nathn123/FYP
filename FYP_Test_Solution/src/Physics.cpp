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
	//viusal representation ogre

	//physics representation
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	mWorld->addRigidBody(groundRigidBody);

	// create skeleton builder
	mBuilder = new Skeleton_Builder(mSceneMgr, mWorld);
	mBoneBuilder = new Bone_Builder(mSceneMgr, mWorld);
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
	Skeleton* skel = new Skeleton(); // initialised one bones created
	// just for testing
	mBuilder->SetDimensions(15, 15, 100, 40);
	mBuilder->SetBodyType(Skeleton_Builder::LegType::Uninverted, Skeleton_Builder::ArmType::LongArms, Skeleton_Builder::TorsoType::LongNeckLongTail);
	return mBuilder->BuildSkeleton(*skel,Ogre::Vector3(0,250,0));
}
bool Physics::TestBone()
{
	Bone* test = new Bone();
	mBoneBuilder->SetDimensions(10, 20);
	mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(20, 20, 20)));
	return mBoneBuilder->BuildBone(*test);
}

void Physics::Update(Ogre::Real frametime)
{
	mWorld->stepSimulation(frametime);
}