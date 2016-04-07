#include "..\header\GameState.h"
#include "IntroState.h"

GameState GameState::mGameState;
GameState::GameState()
{
}
void GameState::enter()
{
	mInput = InputManager::getSingletonPtr();
	mRoot = Ogre::Root::getSingletonPtr();
	if (!mRoot->hasSceneManager("GameSceneMgr"))
		mScenMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "GameSceneMgr");
	else
		mScenMgr = mRoot->getSceneManager("GameSceneMgr");
	mCam = new Camera("GameCam", mScenMgr, mRoot->getAutoCreatedWindow());
	mCam->getOgreCam()->getViewport()->setBackgroundColour(Ogre::ColourValue::Green);
	mGui = GUIManager::getSingleton();
	mExit = false;

	// set up the GUI
}
void GameState::exit()
{
	
	mCam->~Camera();
	mScenMgr->clearScene();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}
void GameState::pause()
{
}
void GameState::resume()
{
}
bool GameState::frameStarted(const Ogre::FrameEvent& evt)
{
	mInput->capture();
	auto key = mInput->getKeyboard();
	auto mouse = mInput->getMouse()->getMouseState();
	if (key->isKeyDown(OIS::KC_Z))
		changeState(IntroState::getState());
	if (key->isKeyDown(OIS::KC_ESCAPE))
		mExit = true;
	return true;
}
bool GameState::frameEnded(const Ogre::FrameEvent& evt)
{
	if (mExit)
		return false;
	return true;
}