#include "../header/Physics_FrameListener.h"


Physics_FrameListener::Physics_FrameListener(Ogre::SceneManager* sceneMgr)
{
	mPhysics = new Physics(sceneMgr);
	mInput = InputManager::getSingletonPtr();
	mToggleTime = 1.0f;
}


Physics_FrameListener::~Physics_FrameListener()
{
	
}

bool Physics_FrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	auto key = mInput->getKeyboard();
	mPhysics->Update(evt.timeSinceLastFrame);

	if (key->isKeyDown(OIS::KC_P) && mToggleTime<=0)
	{
		//mPhysics->CreateTestBox();
		mToggleTime = 10.0f;
	}
	if (key->isKeyDown(OIS::KC_O) && mToggleTime <= 0)
	{
		//mPhysics->CreateTestHead();
		mToggleTime = 10.0f;
	}
	mToggleTime -= evt.timeSinceLastFrame;
	return true;
}