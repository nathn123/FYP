#pragma once
#include<vector>
#include<OGRE\Ogre.h>
#include"Bone.h"

class Skeleton
{
public:
	Skeleton(std::vector<Bone*> mBones, 
		std::vector<btTypedConstraint*> mConstraints,
		Ogre::SceneNode* mNode,
		btDynamicsWorld* World);
	~Skeleton();
private:
	std::vector<Bone*> mBones;
	Ogre::SceneNode* mNode;
};

