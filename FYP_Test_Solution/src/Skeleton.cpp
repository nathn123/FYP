#include "..\header\Skeleton.h"
#include "Utils.h"


Skeleton::Skeleton(std::vector<Bone*> Bones, std::vector<Muscle*> muscles, std::vector<btTypedConstraint*> Constraints, Ogre::SceneNode* Node, btDynamicsWorld* World)
{
	mBones = Bones;
	mNode = Node;
	mMuscles = muscles;
	for (auto constraint: Constraints)
		World->addConstraint(constraint);
}
Skeleton::Skeleton()
{

}
Skeleton::~Skeleton()
{
}