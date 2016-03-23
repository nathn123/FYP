#pragma once
#include<vector>
#include<OGRE\Ogre.h>
#include"Bone.h"
#include"Muscle.h"

class Skeleton
{
public:
	Skeleton(std::vector<Bone*> mBones,
		std::vector<Muscle*> muscles,
		std::vector<btTypedConstraint*> mConstraints,
		Ogre::SceneNode* mNode,
		btDynamicsWorld* World);
	Skeleton();
	~Skeleton();
private:
	std::vector<Bone*> mBones;
	std::vector<Muscle*> mMuscles;
	Ogre::SceneNode* mNode;
};

