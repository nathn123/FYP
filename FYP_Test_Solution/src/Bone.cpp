#include"Bone.h"
#include"Utils.h"




Bone::Bone(Ogre::ManualObject* newbone,Bone::BoneType type, Ogre::Vector3 RelPos,Ogre::Quaternion RelRot,std::vector<Ogre::Vector3> MuscleAttachments, Ogre::SceneNode* parent,btDynamicsWorld* World)
{
	mNode = parent->createChildSceneNode(RelPos, RelRot);
	mType = type;
	mNode->attachObject(newbone);
	mCollider = new btBoxShape(Utils::OgreBTVector(newbone->getBoundingBox().getHalfSize()));
	mMotionState = new MotionState(mNode);
	mMuscleAttachments = MuscleAttachments;
	auto mass = (newbone->getBoundingBox().getSize().x*newbone->getBoundingBox().getSize().y*newbone->getBoundingBox().getSize().z)/1000;
	btVector3 Inertia(0,0,0);
	mCollider->calculateLocalInertia(mass, Inertia);
	btRigidBody::btRigidBodyConstructionInfo boneRigidBody(mass, mMotionState, mCollider, Inertia);
	mRigidBody = new btRigidBody(boneRigidBody);
	World->addRigidBody(mRigidBody, Utils::COL_SKELETON, Utils::COL_WALL);
	
}
Bone::Bone()
{

}
Bone::~Bone()
{
}
btCollisionShape* Bone::GetCollider()
{
	return mCollider;
}
btRigidBody* Bone::GetRigidBody()
{
	return mRigidBody;
}
Ogre::SceneNode* Bone::GetNode()
{
	return mNode;
}
Ogre::Vector3 Bone::GetAttachments(int index)
{
	return mMuscleAttachments[index];
}
Bone::BoneType Bone::GetType()
{
	return mType;
}
