#include "..\header\IntroState.h"
#include "..\header\GameState.h"

IntroState IntroState::mIntrostate;
IntroState::IntroState()
{
}
void IntroState::enter()
{
	mInput = InputManager::getSingletonPtr();
	mRoot = Ogre::Root::getSingletonPtr();
	if (!mRoot->hasSceneManager("IntroSceneMgr"))
		mScenMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "IntroSceneMgr");
	else
		mScenMgr = mRoot->getSceneManager("IntroSceneMgr");
	mCam = new Camera("IntroCam",mScenMgr,mRoot->getAutoCreatedWindow());
	mCam->getOgreCam()->getViewport()->setBackgroundColour(Ogre::ColourValue::Red);
	mGui = GUIManager::getSingleton();
	mExit = false;

	// set up the GUI
}
void IntroState::exit()
{
	mCam->~Camera();
	mScenMgr->clearScene();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}
void IntroState::pause()
{
}
void IntroState::resume()
{
}
bool IntroState::frameStarted(const Ogre::FrameEvent& evt)
{
	mInput->capture();
	auto key = mInput->getKeyboard();
	auto mouse = mInput->getMouse()->getMouseState();
	if (key->isKeyDown(OIS::KC_SPACE))
		changeState(GameState::getState());
	if (key->isKeyDown(OIS::KC_ESCAPE))
		mExit = true;
	return true;
}
bool IntroState::frameEnded(const Ogre::FrameEvent& evt)
{
	if (mExit)
		return false;
	return true;
}
