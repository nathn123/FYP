#pragma once
#include <OgreRoot.h>
#include "..\header\InputManager.h"
#include "..\header\Physics.h"
class Physics_FrameListener : public Ogre::FrameListener
{
public:
	Physics_FrameListener(Ogre::SceneManager* sceneMgr);
	~Physics_FrameListener();
	bool frameEnded(const Ogre::FrameEvent& evt);

private:
	InputManager* mInput;
	Physics* mPhysics;
	float mToggleTime, muscle1,muscle2;
	bool mToggleUpdate;
};

