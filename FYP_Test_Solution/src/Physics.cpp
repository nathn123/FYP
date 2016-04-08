#include "..\header\Physics.h"
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>
#include "Skeleton.h"
#include "Utils.h"
#include "Bone_Builder.h"
#include "Bone.h"

#define _USE_MATH_DEFINES
#include <math.h>

Physics::Physics(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
	solver = new btSequentialImpulseConstraintSolver;
	mWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	mWorld->setGravity(btVector3(0, -9.81f, 0));
	mDrawer = new OgreDebugDrawer(mSceneMgr);
	mDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
	mWorld->setDebugDrawer(mDrawer);

	// add ground
	//viusal representation ogre
	// Lights 
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	Ogre::Light* L = mSceneMgr->createLight("MainLight");
	L->setType(Ogre::Light::LT_DIRECTIONAL);
	L->setDirection(-0.5, -0.5, 0);
	L->setDiffuseColour(Ogre::ColourValue::White);
	L->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
	// create terrain
	Ogre::Entity* FloorEnt;
	Ogre::Plane FloorP(Ogre::Vector3::UNIT_Y,0);
	
	Ogre::MeshManager::getSingleton().createPlane("FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, FloorP, 1500, 1500, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);
	FloorEnt = mSceneMgr->createEntity("FloorPlane");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(FloorEnt);
	FloorEnt->setCastShadows(false);
	FloorEnt->setMaterialName("Dirt.jpg");
	

	//physics representation
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	mWorld->addRigidBody(groundRigidBody, Utils::COL_WALL, Utils::COL_SKELETON);
	
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
bool Physics::BuildCharacter(Skeleton_Builder::TorsoType Torso, Skeleton_Builder::ArmType Arm, Skeleton_Builder::LegType Leg, Skeleton_Builder::NeckType Neck, Skeleton_Builder::TailType Tail, float height, float width, float neckincline, float tailincline, Ogre::Vector3 Position)
{
	Skeleton* skel = new Skeleton();
	mBuilder->SetDimensions(neckincline,tailincline,height,width);
	mBuilder->SetBodyType(Leg,Arm,Torso,Neck,Tail);
	return mBuilder->BuildSkeleton(*skel, Position);
}
bool Physics::TestBone()
{
	Bone* test = new Bone();
	mBoneBuilder->SetDimensions(10, 20);
	mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 100, 0)));
	return mBoneBuilder->BuildBone(*test,Bone::BoneType::LowerArm);
}
bool Physics::TestLimb()
{
	Bone* Upper = new Bone();
	Bone* Lower = new Bone();
	Muscle_Builder* Musclebuilder = new Muscle_Builder(mSceneMgr, mWorld);
	mBoneBuilder->SetDimensions(10, 20);
	mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 100, 0)));
	mBoneBuilder->BuildBone(*Upper, Bone::BoneType::LUpperLeg);
	Upper->GetRigidBody()->setMassProps(0, btVector3(0, 0, 0));
	mBoneBuilder->ClearData();
	mBoneBuilder->SetDimensions(10, 20);
	mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *Upper->GetNode()->createChildSceneNode(Ogre::Vector3(0, -50, 0)));
	mBoneBuilder->BuildBone(*Lower, Bone::BoneType::LowerLeg);
	btTransform localA, localB;
	Ogre::Vector3 offset;
	offset = Ogre::Vector3(0, 25, 0);
	auto vec = Lower->GetNode()->_getDerivedPosition();
	vec += offset;

	Skeleton_Builder::SetJointTransform(localA, localB, Utils::OgreBTVector(Lower->GetNode()->_getDerivedPosition() + offset), Upper->GetNode()->_getDerivedPosition(), Lower->GetNode()->_getDerivedPosition(), btVector3(0,btScalar(M_PI_2), 0));
	btHingeConstraint* LshoulderConst = new btHingeConstraint(*Upper->GetRigidBody(), *Lower->GetRigidBody(), localA, localB,false);

	mWorld->addConstraint(LshoulderConst);
	Musclebuilder->CreateMuscle(Upper, Lower, mMuscles);
	auto test = mMuscles;
	return false;
}
void Physics::TestUpdate()
{
	mWorld->stepSimulation(0.001f);
}
void Physics::Update(Ogre::Real frametime, bool step)
{
	auto test = mWorld->getDebugDrawer()->getDebugMode();
	if (step)
		mWorld->stepSimulation(0.001f);

	mWorld->debugDrawWorld();
}