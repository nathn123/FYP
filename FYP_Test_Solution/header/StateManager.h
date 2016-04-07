#pragma once
#include <vector>
#include "InputManager.h"
#include <Ogre.h>

#include "GUIManager.h"

class State;
class StateManager : public Ogre::FrameListener, public Ogre::Singleton<StateManager>
{
public:
	
	StateManager();
	~StateManager();
	void start(State* state);
	void changeState(State* state);
	void pushState(State* state);
	void popState();

	void Initialise();
private:
	// functions
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	void setupResources();
	bool configure();
	//variables
	InputManager* mInput;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mRW;
	GUIManager* mGui;
	std::vector<State*> mStates;
};