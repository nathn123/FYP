#include "..\header\Skeleton.h"


Skeleton::Skeleton(std::vector<Bone*> Bones, std::vector<btTypedConstraint*> Constraints, Ogre::SceneNode* Node, btDynamicsWorld* World)
{
	mBones = Bones;
	mNode = Node;
	for (auto constraint : Constraints)
		World->addConstraint(constraint);
	
}


Skeleton::~Skeleton()
{
}
