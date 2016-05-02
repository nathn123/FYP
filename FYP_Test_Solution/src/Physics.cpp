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
	// create terrain
	//physics representation
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	mWorld->addRigidBody(groundRigidBody/*, Utils::COL_WALL, Utils::COL_SKELETON*/);
	
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
bool Physics::BuildCharacter(Skeleton_Builder::TorsoType Torso, Skeleton_Builder::ArmType Arm, Skeleton_Builder::LegType Leg, Skeleton_Builder::NeckType Neck, Skeleton_Builder::TailType Tail,
							float height, float width, float depth, float neckincline, float tailincline, bool hideArms,bool hideLegs,bool hideNeck, bool hasMuscle, bool Fixed, Ogre::Vector3 Position)
{
	Skeleton* skel = new Skeleton();
	mBuilder->SetDimensions(neckincline,tailincline,height,width,depth);
	mBuilder->SetHidden(hideArms, hideLegs, hideNeck);
	mBuilder->SetBodyType(Leg,Arm,Torso,Neck,Tail,hasMuscle,Fixed);
	return mBuilder->BuildSkeleton(*skel, Position);
}
bool Physics::TestBone()
{
	Bone* test = new Bone();
	mBoneBuilder->SetDimensions(10, 20, 15);
	mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 100, 0)));
	return mBoneBuilder->BuildBone(*test,Bone::BoneType::LowerArm);
}
bool Physics::TestLimb(char constrainttype)
{
	Muscle_Builder* Musclebuilder = new Muscle_Builder(mSceneMgr, mWorld);
	Skeleton_Builder* SkeletonBuilder = new Skeleton_Builder(mSceneMgr, mWorld);
	if (constrainttype != 'M' && constrainttype != 'Z')
	{
		Bone* Upper = new Bone();
		Bone* Lower = new Bone();

		float width = 10.f;
		float height = 20.f;
		float depth = 10.f;
		Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Radian(Ogre::Real(M_PI_2)), Ogre::Vector3::UNIT_X);
		btQuaternion btquat;
		btquat.setRotation(btVector3(0, 0, 1), btScalar(M_PI_2));
		mBoneBuilder->SetDimensions(width, height, depth);
		mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 200, 0)));
		mBoneBuilder->BuildBone(*Upper, Bone::BoneType::LUpperLeg);

		mBoneBuilder->ClearData();
		mBoneBuilder->SetDimensions(width, height, depth);
		mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 180, 0)));
		mBoneBuilder->BuildBone(*Lower, Bone::BoneType::LowerLeg);

		Upper->GetRigidBody()->setMassProps(0, btVector3(0, 0, 0));

		btTransform localA, localB, testA, testB, JointTransform;
		SkeletonBuilder->SetJointTransform(testA, testB, btVector3(0, 190, 0), Upper, Lower, btVector3(0, btScalar(-M_PI_2), 0));
		//GREEN IS Y AXIS
		//BLUE IS Z AXIS
		//RED IS X AXIS
		if (constrainttype == 'H')
		{
			btHingeConstraint* TestConstraint = new btHingeConstraint(*Upper->GetRigidBody(), *Lower->GetRigidBody(), testA, testB);
			//for hinge constraints we rotate the joint along Y by PI/2 
			// legs set limits as 0 PI/2
			// we need to ac
			TestConstraint->setLimit(0, btScalar(M_PI_2));
			mWorld->addConstraint(TestConstraint,true);
			Musclebuilder->CreateMuscle(Upper, Lower, mMuscles);
		}
		else if (constrainttype == 'S')
		{
			btSliderConstraint* TestConstraint = new btSliderConstraint(*Upper->GetRigidBody(), *Lower->GetRigidBody(), testA, testB, false);
			TestConstraint->setLowerAngLimit(0);
			TestConstraint->setUpperAngLimit(0);
			//TestConstraint->setLowerLinLimit(150.f);
			// depending on the Linear reference i.e the bool at the end
			// if true it uses direction from A as the direction to be multiplied by scalar / false it uses the directon from B as the scalar
			//now what the limit actually controls , is the distance to be traveled from original position to the constraint position
			// if the scaler is positon, this move is towards / if negative then move away from constraint
			TestConstraint->setUpperLinLimit(150.f);
			mWorld->addConstraint(TestConstraint);
		}
		else if (constrainttype == 'C')
		{
			btConeTwistConstraint* TestConstraint = new btConeTwistConstraint(*Upper->GetRigidBody(), *Lower->GetRigidBody(), testA, testB);
			// hip to leg contraint along X axis -M_PI_4*5
			// torso to arm constraint along X axis by M_PI_4
			// shoulder to neck we need M_PI_2 ALONG Z axis with limits at M_PI_4, M_PI_4, 0
			//torso to torso / neck to neck constraint along -Y axis by M_PI_2 with limits (M_PI_4/4, M_PI_4/4, M_PI_4/4)
			TestConstraint->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));
			mWorld->addConstraint(TestConstraint);
		}
		mTestBody = Lower->GetRigidBody();
		mTestBody->setDeactivationTime(FLT_MAX);
	}
	else if (constrainttype == 'M')
	{
		// simulated muscles with constraint, no forces generated yet.
		// what we need are three bodies A and B connected by HINGE JOINT
		// the above method does not produce the required force in the correct direction
		// next attempt is to have Fixed constraints attached to the attachment points 
		btFixedConstraint* FixedA,*FixedB;
		
		float width = 10.f;
		float height = 20.f;
		float depth = 10.f;
		float muscledist = 0.0f;
		float tendondist = 0.0f;
		float maxContract = 0.95f;
		float TendonSlack = 0.0f;
		Bone* Upper = new Bone();
		Bone* Lower = new Bone();
		btRigidBody *PointMassA, *PointMassB;
		btCollisionShape* shape = new btSphereShape(0.1f);
		btRigidBody* PointMassM = new btRigidBody(0.1f, new MotionState(mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(width / 2 + 1, 183, 0))), shape);
		PointMassA = new btRigidBody(0.1f, new MotionState(mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(width / 2 +1, 200, 0))), shape);
		PointMassB = new btRigidBody(0.1f, new MotionState(mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(width / 2 + 1, 180, 0))), shape);

		mTestBody = PointMassM;
		mBoneBuilder->SetDimensions(width, height, depth);
		mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 200, 0)));
		mBoneBuilder->BuildBone(*Upper, Bone::BoneType::LUpperLeg);
		mBoneBuilder->SetDimensions(width, height, depth);
		mBoneBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 180, 0)));
		mBoneBuilder->BuildBone(*Lower, Bone::BoneType::LowerLeg);
		Upper->GetRigidBody()->setMassProps(0, btVector3(0, 0, 0));
		btTransform localA, localB, testA, testB, JointTransform;
		// now set up the joints
		SkeletonBuilder->SetJointTransform(testA, testB, btVector3(0, 190, 0), Upper, Lower, btVector3(0, 0, btScalar(-M_PI_2)));
		btHingeConstraint* TestConstraint = new btHingeConstraint(*Upper->GetRigidBody(), *Lower->GetRigidBody(), testA, testB);
		/*for hinge constraints we rotate the joint along Y by PI/2 */
		// legs set limits as 0 PI/2
		TestConstraint->setLimit(btScalar(M_PI_2), btScalar(M_PI_2));
		mWorld->addConstraint(TestConstraint, true);
		btTransform A, PA, B, BA, FixedTrans;
		btQuaternion fixedQ;
		fixedQ.setEulerZYX(0,M_PI_2,0);
		FixedTrans.setRotation(fixedQ);
		FixedTrans.setOrigin(btVector3(width / 4, 200, 0));
		A = Upper->GetRigidBody()->getWorldTransform().inverse() * FixedTrans;
		PA = PointMassA->getWorldTransform().inverse() * FixedTrans;
		FixedTrans.setOrigin(btVector3(width / 4, 180, 0));
		B = Lower->GetRigidBody()->getWorldTransform().inverse() * FixedTrans;
		BA = PointMassB->getWorldTransform().inverse() * FixedTrans;

		FixedA = new btFixedConstraint(*Upper->GetRigidBody(), *PointMassA, A, PA);
		FixedB = new btFixedConstraint(*Lower->GetRigidBody(), *PointMassB, B, PA);
		FixedA->setEnabled(true);
		FixedB->setEnabled(true);
		// now the sliders
		btQuaternion btquat;
		btTransform JointTrans;
		btquat.setEulerZYX(-M_PI_2, 0, 0);
		JointTrans.setIdentity();
		JointTrans.setRotation(btquat);
		JointTrans.setOrigin(btVector3(width, 190, 0));
		auto TransformA = Upper->GetRigidBody()->getWorldTransform().inverse() * JointTrans;
		auto TransformB = PointMassM->getWorldTransform().inverse() * JointTrans;
		// first muscle / rework from actual muscle class, so we want muscle attached to the immovable bone and the pointmass
		muscledist = fabsf(PointMassM->getWorldTransform().getOrigin().distance(PointMassA->getWorldTransform().getOrigin()));
		btSliderConstraint* MuscleTestConstraint = new btSliderConstraint(*PointMassA, *PointMassM, TransformA, TransformB, false);
		MuscleTestConstraint->setLowerAngLimit(0);
		MuscleTestConstraint->setUpperAngLimit(0);
		MuscleTestConstraint->setUpperLinLimit(muscledist * maxContract);
		mWorld->addConstraint(MuscleTestConstraint);

		TransformA = PointMassM->getWorldTransform().inverse() * JointTrans;
		TransformB = Lower->GetRigidBody()->getWorldTransform().inverse() * JointTrans;
		tendondist = fabsf(PointMassM->getWorldTransform().getOrigin().distance(PointMassB->getWorldTransform().getOrigin()));
		btSliderConstraint* TendonTestConstraint = new btSliderConstraint(*PointMassM, *PointMassB, TransformA, TransformB, false);
		TendonTestConstraint->setLowerAngLimit(0);
		TendonTestConstraint->setUpperAngLimit(0);
		TendonTestConstraint->setUpperLinLimit(tendondist * TendonSlack);
		TendonTestConstraint->setLowerLinLimit(tendondist);
		mWorld->addConstraint(TendonTestConstraint);
		mWorld->addRigidBody(PointMassM);
		mWorld->addRigidBody(PointMassA);
		mWorld->addRigidBody(PointMassB);
	

	}
	else if (constrainttype == 'Z')
	{
		Skeleton skel = Skeleton();
		SkeletonBuilder->SetBodyType(Skeleton_Builder::LegType::Uninverted,Skeleton_Builder::ArmType::LongArms,Skeleton_Builder::TorsoType::Upright,Skeleton_Builder::NeckType::LongNeck,Skeleton_Builder::TailType::LongTail,false,false);
		SkeletonBuilder->SetHidden(false, false, false);
		SkeletonBuilder->SetDimensions(-35, -35, 100, 25, 15);
		SkeletonBuilder->BuildSkeleton(skel, Ogre::Vector3(0, 100, 0));
	}
	

	//Musclebuilder->CreateMuscle(Upper, Lower, mMuscles);
	return false;
}
void Physics::TestUpdate()
{
	mWorld->stepSimulation(0.001f);
}
void Physics::Update(Ogre::Real frametime, bool step)
{
	if (step)
		mWorld->stepSimulation(frametime);


	mWorld->debugDrawWorld();
}