#pragma once
#include "State.h"
#include "Physics.h"
class TestState :
	public State
{
public:
	void enter();
	void exit();

	void pause();
	void resume();

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static TestState* getState(){ return &mTest; };

protected:
	TestState();
	Physics* mPhysics;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Root* mRoot;
	InputManager* mInput;
	float mActivation1, mActivation2;
	Camera* mCam;
	bool step;
	float mToggleTime;
	static TestState mTest;

};

