#pragma once
#include <OgreRoot.h>
#include "..\header\InputManager.h"
#include "..\header\Camera.h"
class Camera_FrameListener : public Ogre::FrameListener
{
public:
	Camera_FrameListener(Ogre::SceneManager* sceneMgr,Ogre::RenderWindow* rw, Ogre::String name);
	~Camera_FrameListener();
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
	InputManager* mInput;
	Ogre::SceneNode* mTracking;
	Ogre::Vector3 mLastTrackedPos;
	Camera* mCam;
	float movespeed, rotspeed;
};

