#pragma once
#include<vector>
#include<Ogre.h>
#include<OgreBulletCollisions.h>
#include"..\header\Bone.h"

class Skeleton
{
public:
	Skeleton();
	~Skeleton();
private:
	std::vector<Bone*> mBones;
	Ogre::SceneNode mNode;
	OgreBulletCollisions::CompoundCollisionShape* mCollider;
};

