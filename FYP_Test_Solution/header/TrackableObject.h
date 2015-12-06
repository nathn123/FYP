#pragma once
#include "Ogre.h"
#include "InputManager.h"
class TrackableObject
{
public:
	TrackableObject();
	~TrackableObject();

	Ogre::SceneNode* getSightNode();
	Ogre::SceneNode* getCameraNode();
	Ogre::Vector3 getWorldPosition();
	virtual void update(Ogre::Real elapsedTime, OIS::Keyboard* input) = 0; // updates the object
protected:

	Ogre::SceneNode* mCharNode; // main object node
	Ogre::SceneNode* mSightNode; // "sight node" - where the object is looking
	Ogre::SceneNode* mCameraNode; // Node for the chase camera
	Ogre::Entity* mEntity; // object entity
	Ogre::SceneManager* mSceneMgr;
};

