#include "..\header\Skeleton.h"


Skeleton::Skeleton(std::vector<Bone*> Bones, std::vector<Muscle*> muscles, std::vector<btTypedConstraint*> Constraints, Ogre::SceneNode* Node, btDynamicsWorld* World)
{
	mBones = Bones;
	mNode = Node;
	mMuscles = muscles;
	for (int i = 0; i < 2 /*Constraints.size()*/; ++i)
		World->addConstraint(Constraints[i]);
}
Skeleton::Skeleton()
{

}

Skeleton::~Skeleton()
{
}
