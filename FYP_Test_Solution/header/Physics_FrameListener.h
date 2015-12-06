#pragma once
#include <OgreRoot.h>
#include "..\header\InputManager.h"
#include "..\header\Physics.h"
class Physics_FrameListener : public Ogre::FrameListener
{
public:
	Physics_FrameListener(Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* rw);
	~Physics_FrameListener();
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
	InputManager* mInput;
	Physics* mPhysics;
	float mToggleTime;
};

