#include "MotionState.h"
#include "Utils.h"


MotionState::MotionState(const btTransform &initialPosition, Ogre::SceneNode* node)
{
	mNode = node;
	mInitialPosition = initialPosition;
}
MotionState::MotionState(Ogre::SceneNode* node)
{
	mNode = node;
	mInitialPosition = Utils::OgreNodeBtTransform(mNode);
}
MotionState::~MotionState()
{
}
void MotionState::setNode(Ogre::SceneNode* node)
{
	mNode = node;
}
void MotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans = mInitialPosition;
}
void MotionState::setWorldTransform(const btTransform& worldTrans)
{
	if (mNode == nullptr)
		return; // do nothing if no node attached
	mNode->setPosition(mNode->convertWorldToLocalPosition(Utils::BTOgreVector(worldTrans.getOrigin())));
	mNode->setOrientation(mNode->convertWorldToLocalOrientation(Utils::BTOgreQuat(worldTrans.getRotation())));
}
