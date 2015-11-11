#pragma once
#include<vector>
#include<Ogre.h>
#include<OgreBulletCollisions.h>
class Bone
{
	class Muscle;
public:
	Bone(Ogre::ManualObject* newbone,Ogre::Vector3 RelPos, Ogre::SceneNode* parent);
	~Bone();
	OgreBulletCollisions::BoxCollisionShape* GetCollider();
	Ogre::SceneNode* GetNode();
	std::vector<Ogre::Vector3> GetAttachments();
	bool AttachMuscle(int index, Muscle* newmuscle);
private:
	Ogre::SceneNode* mNode;
	OgreBulletCollisions::BoxCollisionShape* mCollider;
	// locations of muscle attachment points
	std::vector<Ogre::Vector3> mMuscleAttachments;
	std::vector<Muscle*>mMuscle;
};

