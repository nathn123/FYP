#include "..\header\TrackableObject.h"


TrackableObject::TrackableObject()
{
}


TrackableObject::~TrackableObject()
{
}

Ogre::SceneNode*  TrackableObject::getSightNode()
{
	return mSightNode;
}
Ogre::SceneNode* TrackableObject::getCameraNode()
{
	return mCameraNode;
}
Ogre::Vector3 TrackableObject::getWorldPosition()
{
	return mCharNode->_getDerivedPosition();
}