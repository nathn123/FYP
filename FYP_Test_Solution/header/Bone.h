#pragma once
#include<vector>
#include<OGRE/Ogre.h>
#include<Bullet/btBulletCollisionCommon.h>
#include<Bullet/btBulletDynamicsCommon.h>
#include"MotionState.h"
class Bone
{
	class Muscle;
public:
	Bone(Ogre::ManualObject* newbone, Ogre::Vector3 RelPos, Ogre::Quaternion RelRot, Ogre::SceneNode* parent,btDynamicsWorld* World);
	Bone();
	~Bone();
	btCollisionShape* GetCollider();
	btRigidBody* GetRigidBody();
	Ogre::SceneNode* GetNode();
	std::vector<Ogre::Vector3> GetAttachments();
	bool AttachMuscle(int index, Muscle* newmuscle);
private:
	
	Ogre::SceneNode* mNode;
	btCollisionShape* mCollider;
	btRigidBody* mRigidBody;
	MotionState* mMotionState;
	// locations of muscle attachment points
	std::vector<Ogre::Vector3> mMuscleAttachments;
	std::vector<Muscle*>mMuscle;
};

