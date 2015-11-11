#include"..\header\Bone.h"


Bone::Bone(Ogre::ManualObject* newbone, Ogre::Vector3 RelPos, Ogre::SceneNode* parent)
{
	mNode = parent->createChildSceneNode(RelPos);
	mNode->attachObject(newbone);
}


Bone::~Bone()
{
}

OgreBulletCollisions::BoxCollisionShape* Bone::GetCollider()
{
	return mCollider;
}

Ogre::SceneNode* Bone::GetNode()
{
	return mNode;
}

std::vector<Ogre::Vector3> Bone::GetAttachments()
{
	return mMuscleAttachments;
}

bool Bone::AttachMuscle(int index, Muscle* newmuscle)
{
	if (mMuscle[index] != nullptr)
		return false;
	mMuscle[index] = newmuscle;
	return true;
}
