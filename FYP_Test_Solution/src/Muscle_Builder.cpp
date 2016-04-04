#include "Muscle_Builder.h"

Muscle_Builder::Muscle_Builder(Ogre::SceneManager* scenemanger, btDynamicsWorld* world)
{
	mSceneMgr = scenemanger;
	mWorld = world;
}
void Muscle_Builder::SetMuscleTransform(btTransform& TransformA, btTransform& TransformB, btVector3& pointA, btVector3& pointB, btVector3& Axis)
{
	btVector3 JointPosWorld = Utils::OgreBTVector(Utils::BTOgreVector(pointA).midPoint(Utils::BTOgreVector(pointB)));
	TransformA.setIdentity(); TransformB.setIdentity();
	TransformA.getBasis().setEulerZYX(Axis.x(), Axis.y(), Axis.z());
	TransformB.getBasis().setEulerZYX(Axis.x(), Axis.y(), Axis.z());
	TransformA.setOrigin(pointA - JointPosWorld);
	TransformB.setOrigin(pointB - JointPosWorld);
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
	for (unsigned int i = 0; i < attachmentpoints.size(); i+=2)
	{
		// model the muscle as 3 points 
		// point A and C attachment points on connected to a bone
		btVector3 AttachmentA, AttachmentC, pointmasspositon;
		btRigidBody* BodyA, *BodyC;
		if (i % 4 == 0)
		{
			BodyA = boneA->GetRigidBody();
			BodyC = boneB->GetRigidBody();
			AttachmentA = Utils::OgreBTVector(boneA->GetAttachments(attachmentpoints[i]));
			AttachmentC = Utils::OgreBTVector(boneB->GetAttachments(attachmentpoints[i + 1]));
			// calculate WORLD position of pointmass
			pointmasspositon = Utils::OgreBTVector(boneA->GetNode()->_getDerivedPosition()) + AttachmentA + AttachmentA.normalized() * 20.f;
		}
		else
		{
			// if odd numered then reverse
			BodyC = boneA->GetRigidBody();
			BodyA = boneB->GetRigidBody();
			AttachmentC = Utils::OgreBTVector(boneA->GetAttachments(attachmentpoints[i]));
			AttachmentA = Utils::OgreBTVector(boneB->GetAttachments(attachmentpoints[i + 1]));
			// calculate WORLD position of pointmass
			pointmasspositon = Utils::OgreBTVector(boneB->GetNode()->_getDerivedPosition()) + AttachmentA + AttachmentA.normalized() * 20.f;

		}
		// A and B connected by 6DOfConst
		// B and C connected via Slider, to simulate contraction
		
		
		btDefaultMotionState* state = new btDefaultMotionState(btTransform(btQuaternion(0,0,0),pointmasspositon));
		btCollisionShape* shape = new btSphereShape(5.f);
		auto BodyB = new btRigidBody(0, state, shape);
		

		btTransform transformA = btTransform();
		btTransform transformB = btTransform();

		SetMuscleTransform(transformA, transformB, AttachmentA, pointmasspositon);

		auto Tendon = new btFixedConstraint(*BodyA, *BodyB, transformB, transformA);
		//LOCK THE CONSTRAINT
		auto test = Tendon->getDbgDrawSize();
		Tendon->setDbgDrawSize(btScalar(10.0f));
		test = Tendon->getDbgDrawSize();

		btTransform transformC = btTransform();
		btTransform transformB2 = btTransform();

		SetMuscleTransform(transformC, transformB2, AttachmentC, pointmasspositon,btVector3(0,1,0));
		auto Musclepart = new btSliderConstraint(*BodyC, *BodyB, transformC, transformB2, true);
		mWorld->addRigidBody(BodyB, Utils::COL_NOTHING, Utils::COL_NOTHING);

		mWorld->addConstraint(Tendon);
		mWorld->addConstraint(Musclepart);
		mMuscles.push_back(new Muscle(BodyA, BodyC, AttachmentA, AttachmentC, Tendon, Musclepart, BodyB));
	}
	Muscle_Out =  mMuscles;
	return true;
}