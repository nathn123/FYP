#include "Muscle_Builder.h"
#define _USE_MATH_DEFINES
#include <math.h>

Muscle_Builder::Muscle_Builder(Ogre::SceneManager* scenemanger, btDynamicsWorld* world)
{
	mSceneMgr = scenemanger;
	mWorld = world;
}
void Muscle_Builder::SetMuscleTransform(btTransform& TransformA, btTransform& TransformB, btVector3& JointPosWorld, btTransform& pointA, btTransform& pointB, btVector3& Axis)
{
	btQuaternion btquat;
	btTransform JointTrans;
	btquat.setEulerZYX(Axis.z(), Axis.y(), Axis.x());
	JointTrans.setIdentity();
	JointTrans.setRotation(btquat);
	JointTrans.setOrigin(JointPosWorld);
	TransformA = pointA.inverse() * JointTrans;
	TransformB = pointB.inverse() * JointTrans;
}
bool Muscle_Builder::FindAttachmentPoints(Bone* boneA, Bone* boneB, std::vector<int>& attachments)
{
	attachments.clear();
	// we first find attachment A
	if (boneA->GetType() == Bone::BoneType::Hip)
	{
		if (boneB->GetType() == Bone::BoneType::Torso)
		{
			attachments.clear();
			attachments.push_back(0);
			attachments.push_back(0);
			attachments.push_back(1);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::LUpperLeg)
		{
			attachments.clear();
			// front back 
			attachments.push_back(1);
			attachments.push_back(2);
			attachments.push_back(2);
			attachments.push_back(3);
			// left right
			attachments.push_back(6);
			attachments.push_back(0);
			attachments.push_back(7);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::RUpperLeg)
		{
			attachments.clear();
			// front back 
			attachments.push_back(3);
			attachments.push_back(2);
			attachments.push_back(4);
			attachments.push_back(3);
			// left right
			attachments.push_back(8);
			attachments.push_back(0);
			attachments.push_back(9);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::Tail)
		{
			attachments.push_back(10);
			attachments.push_back(0);
			attachments.push_back(11);
			attachments.push_back(1);
			return true;
		}
	}
	else if (boneA->GetType() == Bone::BoneType::Shoulder)
	{
		if (boneB->GetType() == Bone::BoneType::Torso)
		{
			attachments.push_back(0);
			attachments.push_back(0);
			attachments.push_back(1);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::LUpperArm)
		{
			//front and back
			attachments.push_back(2);
			attachments.push_back(2);
			attachments.push_back(3);
			attachments.push_back(3);
			//up and bot
			attachments.push_back(6);
			attachments.push_back(0);
			attachments.push_back(7);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::RUpperArm)
		{
			//front and back
			attachments.push_back(4);
			attachments.push_back(2);
			attachments.push_back(5);
			attachments.push_back(3);
			//up and bot
			attachments.push_back(8);
			attachments.push_back(0);
			attachments.push_back(9);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::Neck)
		{
			//up and down 
			attachments.push_back(0);
			attachments.push_back(2);
			attachments.push_back(1);
			attachments.push_back(3);

			//left and righr neck
			attachments.push_back(6);
			attachments.push_back(0);
			attachments.push_back(8);
			attachments.push_back(1);
			return true;
		}
	}
	else if (boneA->GetType() == Bone::BoneType::HipShoulder)
	{
		if (boneB->GetType() == Bone::BoneType::LUpperArm)
		{
			//front and back
			attachments.push_back(8);
			attachments.push_back(0);
			attachments.push_back(9);
			attachments.push_back(1);
			//up and bot
			attachments.push_back(12);
			attachments.push_back(2);
			attachments.push_back(13);
			attachments.push_back(3);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::RUpperArm)
		{
			//front and back
			attachments.push_back(10);
			attachments.push_back(0);
			attachments.push_back(11);
			attachments.push_back(1);
			//up and bot
			attachments.push_back(14);
			attachments.push_back(2);
			attachments.push_back(15);
			attachments.push_back(3);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::Neck)
		{
			//up and down 
			attachments.push_back(0);
			attachments.push_back(2);
			attachments.push_back(1);
			attachments.push_back(3);
			//left and righr neck
			attachments.push_back(6);
			attachments.push_back(0);
			attachments.push_back(8);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::Tail)
		{
			// NEEED TO FILL IN
			//up and down
			//attachments.push_back();
			//attachments.push_back();
			//attachments.push_back();
			//attachments.push_back();
			////left and right neck
			//attachments.push_back();
			//attachments.push_back();
			//attachments.push_back();
			//attachments.push_back();
		}
		else if (boneB->GetType() == Bone::BoneType::LUpperLeg)
		{
			attachments.clear();
			// front back 
			attachments.push_back(0);
			attachments.push_back(2);
			attachments.push_back(1);
			attachments.push_back(3);
			// left right
			attachments.push_back(4);
			attachments.push_back(0);
			attachments.push_back(6);
			attachments.push_back(1);
			return true;
		}
		else if (boneB->GetType() == Bone::BoneType::RUpperLeg)
		{
			attachments.clear();
			// front back 
			attachments.push_back(2);
			attachments.push_back(2);
			attachments.push_back(3);
			attachments.push_back(3);
			// left right
			attachments.push_back(6);
			attachments.push_back(0);
			attachments.push_back(7);
			attachments.push_back(1);
			return true;
		}
	}
	else if (boneA->GetType() == Bone::BoneType::RUpperLeg)
	{
		if (boneB->GetType() == Bone::BoneType::LowerLeg)
		{
			attachments.clear();
			// front back 
			attachments.push_back(2);
			attachments.push_back(0);
			attachments.push_back(3);
			attachments.push_back(1);
			return true;
		}

	}
	else if (boneA->GetType() == Bone::BoneType::RUpperArm)
	{
		if (boneB->GetType() == Bone::BoneType::LowerArm)
		{
			attachments.clear();
			// front back 
			attachments.push_back(2);
			attachments.push_back(0);
			attachments.push_back(3);
			attachments.push_back(1);
			return true;
		}
	}
	else if (boneA->GetType() == Bone::BoneType::LUpperLeg)
	{
		if (boneB->GetType() == Bone::BoneType::LowerLeg)
		{
			attachments.clear();
			// front back 
			attachments.push_back(2);
			attachments.push_back(0);
			attachments.push_back(3);
			attachments.push_back(1);
			return true;
		}
	}
	else if (boneA->GetType() == Bone::BoneType::LUpperArm)
	{
		if (boneB->GetType() == Bone::BoneType::LowerArm)
		{
			attachments.clear();
			// front back 
			attachments.push_back(2);
			attachments.push_back(0);
			attachments.push_back(3);
			attachments.push_back(1);
			return true;
		}
	}
	return false;
}
bool Muscle_Builder::CreateMuscle(Bone* boneA, Bone* boneB, std::vector<Muscle*>& Muscle_Out)
{
	std::vector<int> attachmentpoints;
	if (!FindAttachmentPoints(boneA, boneB, attachmentpoints))
		return false;
	std::vector<Muscle*> mMuscles;
	for (unsigned int i = 0; i <  attachmentpoints.size(); i+=2)
	{
		// model the muscle as 3 points 
		// point A and C attachment points on connected to a bone
		btVector3 WorldAttachmentA, WorldAttachmentC, LocalAttachmentA, LocalAttachmentC, pointmasspositon;
		btRigidBody* BodyA, *BodyC;
		// calculate a muscle to tendon ratio
		// possibly changable
			BodyA = boneA->GetRigidBody();
			BodyC = boneB->GetRigidBody();
			WorldAttachmentA = Utils::OgreBTVector(boneA->GetNode()->_getDerivedPosition() + boneA->GetAttachments(attachmentpoints[i]));
			WorldAttachmentC = Utils::OgreBTVector(boneB->GetNode()->_getDerivedPosition() + boneB->GetAttachments(attachmentpoints[i + 1]));
			LocalAttachmentA = Utils::OgreBTVector(boneA->GetAttachments(attachmentpoints[i]));
			LocalAttachmentC = Utils::OgreBTVector(boneB->GetAttachments(attachmentpoints[i + 1]));

		// A and B connected by Slider
		// B and C connected via Slider, to simulate contraction

		// calculate WORLD position of pointmass
		pointmasspositon = WorldAttachmentA + (WorldAttachmentC - WorldAttachmentA).normalized() *( (WorldAttachmentC.distance(WorldAttachmentA) * 0.15f));
		
		auto pointmassnode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Utils::BTOgreVector(pointmasspositon));
		MotionState* state = new MotionState(pointmassnode);
		btCollisionShape* shape = new btSphereShape(0.01f);
		
		auto BodyB = new btRigidBody(0.01f, state, shape);
		

		btTransform transformA = btTransform();
		btTransform transformB = btTransform();
		btTransform AttachTransA = btTransform(BodyA->getWorldTransform().getBasis(), WorldAttachmentA);
		btTransform AttachTransB = btTransform(BodyB->getWorldTransform());

		SetMuscleTransform(transformA, transformB, pointmasspositon, AttachTransA, AttachTransB, btVector3(M_PI_4, 0, 0));

		auto Tendon = new btSliderConstraint(*BodyA, *BodyB, transformA, transformB, false);
		// set constraint limits to allow 10% motion in either direction
		// due to the slider point being the pointmass point
		// the upper limit is positive because the attachment point should be over the pointmass

		auto tendondist = fabsf(pointmasspositon.distance(WorldAttachmentA));
		// find the initial 10%
		auto tendonPerc = tendondist * 0.10;
		Tendon->setLowerLinLimit(-tendonPerc);
		Tendon->setUpperLinLimit(0);

		// NOW SET UP THE MUSCLE
		btTransform transformC = btTransform();
		btTransform transformB2 = btTransform();
		btTransform AttachTransC = btTransform(BodyA->getWorldTransform().getBasis(), WorldAttachmentC);

		SetMuscleTransform(transformC, transformB2, pointmasspositon, AttachTransC, AttachTransB, btVector3(M_PI_4, 0, 0));
		auto Musclepart = new btSliderConstraint(*BodyC, *BodyB, transformC, transformB2, false);
		


		auto muscledist = pointmasspositon.distance(WorldAttachmentC);
		auto musclePerc = muscledist * 0.10f; 

		Musclepart->setUpperLinLimit(0); // 
		Musclepart->setLowerLinLimit(musclePerc);
		mWorld->addRigidBody(BodyB);

		mWorld->addConstraint(Tendon);
		mWorld->addConstraint(Musclepart);
		mMuscles.push_back(new Muscle(BodyA, BodyC, LocalAttachmentA, LocalAttachmentC, Tendon, Musclepart, BodyB, state));
	}
	Muscle_Out =  mMuscles;
	return true;
}