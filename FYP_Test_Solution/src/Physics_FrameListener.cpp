#include "../header/Physics_FrameListener.h"

Physics_FrameListener::Physics_FrameListener(Ogre::SceneManager* sceneMgr)
{
	mPhysics = new Physics(sceneMgr);
	mInput = InputManager::getSingletonPtr();
	mToggleTime = 1.0f;
	mToggleUpdate = true;
}


Physics_FrameListener::~Physics_FrameListener()
{
	
}

bool Physics_FrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
	auto key = mInput->getKeyboard();
	mPhysics->Update(evt.timeSinceLastFrame, mToggleUpdate);

	if (key->isKeyDown(OIS::KC_P) && mToggleTime<=0)
	{
		mPhysics->BuildCharacter();
		mToggleTime = 10.0f;
	}
	if (key->isKeyDown(OIS::KC_O) && mToggleTime <= 0)
	{
		mPhysics->TestLimb();
		mToggleTime = 10.0f;
	}
	if (key->isKeyDown(OIS::KC_Y))
		mToggleUpdate = !mToggleUpdate;
	if (key->isKeyDown(OIS::KC_SPACE) && mToggleTime <= 0)
	{
		mPhysics->TestUpdate();
		mToggleTime = 2.0f;
	}
	if (key->isKeyDown(OIS::KC_U))
	{
		muscle1 += 1.f;
		mPhysics->mMuscles[0]->ActivationState(muscle1);
	}
	if (key->isKeyDown(OIS::KC_J))
	{
		muscle1 -= 1.f;
		mPhysics->mMuscles[0]->ActivationState(muscle1);
	}
	if (key->isKeyDown(OIS::KC_I))
	{
		muscle2 += 1.f;
		mPhysics->mMuscles[1]->ActivationState(muscle2);
	}
	if (key->isKeyDown(OIS::KC_K))
	{
		muscle2 -= 1.f;
		mPhysics->mMuscles[1]->ActivationState(muscle2);
	}
	for each (auto muscle in mPhysics->mMuscles)
		muscle->Update(evt.timeSinceLastFrame);
	mToggleTime -= evt.timeSinceLastFrame;
	return true;
}