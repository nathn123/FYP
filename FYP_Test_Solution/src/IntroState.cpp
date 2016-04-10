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
	if (mRoot != StateManager::getSingletonPtr()->GetRoot())
		return;
	mScenMgr = mRoot->getSceneManager("Scene");
	if (mScenMgr->hasCamera("IntroCam"))
		mCam = new Camera("IntroCam", mScenMgr, mRoot->getAutoCreatedWindow(), mScenMgr->getCamera("IntroCam"));
	else
		mCam = new Camera("IntroCam",mScenMgr,mRoot->getAutoCreatedWindow());
	mCam->getOgreCam()->getViewport()->setBackgroundColour(Ogre::ColourValue::Red);
	mGui = GUIManager::getSingleton();
	mExit = false;
	auto state = GameState::getState();
	// set up the GUI
	mGuiRoot = mGui->LoadGUIsheet("IntroMenuLayout.layout");
	mGuiRoot->getChild("Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::Exit, this));
	mGuiRoot->getChild("SwitchState")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::GoToGame, this));
}
void IntroState::exit()
{
	mCam->~Camera();
	//mScenMgr->clearScene();
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
	//auto key = mInput->getKeyboard();
	//auto mouse = mInput->getMouse()->getMouseState();
	//if (key->isKeyDown(OIS::KC_SPACE))
	//	changeState(GameState::getState());
	//if (key->isKeyDown(OIS::KC_ESCAPE))
	//	mExit = true;
	return true;
}
void IntroState::Exit()
{
	mExit = true;
}
void IntroState::GoToGame()
{
	changeState(GameState::getState());
}
bool IntroState::frameEnded(const Ogre::FrameEvent& evt)
{
	if (mExit)
		return false;
	return true;
}
