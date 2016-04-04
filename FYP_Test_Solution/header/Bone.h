#pragma once
#include<vector>
#include<OGRE/Ogre.h>
#include<Bullet/btBulletCollisionCommon.h>
#include<Bullet/btBulletDynamicsCommon.h>
#include"MotionState.h"
#include"Muscle.h"




class Bone
{


public:
	enum BoneType
	{
		Neck,
		Tail,
		Torso,
		Hip,
		Shoulder,
		HipShoulder,
		LUpperArm,
		RUpperArm,
		LowerArm,
		LUpperLeg,
		RUpperLeg,
		LowerLeg,
		Foot
	};

	Bone(Ogre::ManualObject* newbone,BoneType type, Ogre::Vector3 RelPos, Ogre::Quaternion RelRot,std::vector<Ogre::Vector3> MuscleAttachments, Ogre::SceneNode* parent,btDynamicsWorld* World);
	Bone();
	~Bone();
	btCollisionShape* GetCollider();
	btRigidBody* GetRigidBody();
	Ogre::SceneNode* GetNode();
	BoneType GetType();
	Ogre::Vector3 Bone::GetAttachments(int index);
private:
	
	Ogre::SceneNode* mNode;
	BoneType mType;
	btCollisionShape* mCollider;
	btRigidBody* mRigidBody;
	MotionState* mMotionState;
	// locations of muscle attachment points
	std::vector<Ogre::Vector3> mMuscleAttachments;
};

