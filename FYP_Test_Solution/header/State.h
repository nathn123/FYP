#pragma once
#include "Camera.h"
#include "StateManager.h"
class State
{
public:
	virtual void enter() = 0;
	virtual void exit() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual bool frameStarted(const Ogre::FrameEvent& evt) = 0;
	virtual bool frameEnded(const Ogre::FrameEvent& evt) = 0;

	void changeState(State* state) { StateManager::getSingletonPtr()->changeState(state); }
	void pushState(State* state) { StateManager::getSingletonPtr()->pushState(state); }
	void popState() { StateManager::getSingletonPtr()->popState(); }
protected:
	State();
};