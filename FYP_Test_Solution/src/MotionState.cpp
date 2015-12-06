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
	mInitialPosition = btTransform(Utils::OgreBTQuat(mNode->_getDerivedOrientation()),
								Utils::OgreBTVector(mNode->_getDerivedPosition()));
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
	mNode->setOrientation(Utils::BTOgreQuat(worldTrans.getRotation()));
	mNode->setPosition(Utils::BTOgreVector(worldTrans.getOrigin()));
}
