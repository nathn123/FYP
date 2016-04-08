#pragma once
#include "State.h"
class IntroState : public State
{
public:
	
	void enter();
	void exit();

	void pause();
	void resume();

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static IntroState* getState() { return &mIntrostate; };
private:

	IntroState();
	void Exit();
	void GoToGame();

	Ogre::Root* mRoot;
	Ogre::SceneManager* mScenMgr;
	Ogre::Viewport* mView;
	GUIManager* mGui;
	CEGUI::Window* mGuiRoot;
	std::vector<CEGUI::Window*> mGuiObjects;
	InputManager* mInput;
	Camera* mCam;
	bool mExit;
	static IntroState mIntrostate;
};

