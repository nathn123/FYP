#pragma once
#include "State.h"
#include "Physics.h"
class GameState :	public State
{
public:

	void enter();
	void exit();

	void pause();
	void resume();

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);


	static GameState* getState() { return &mGameState; };
private:

	GameState();
	void GoToIntro();
	void BuildSkeleton();
	void HideSettings();
	float ParseText(CEGUI::String ToParse);
	Ogre::Root* mRoot;
	Ogre::SceneManager* mScenMgr;
	Ogre::Viewport* mView;
	Physics* mPhysics;
	GUIManager* mGui;
	CEGUI::Window* mGUIRoot;
	InputManager* mInput;
	Camera* mCam;
	bool mExit;
	static GameState mGameState;
};

