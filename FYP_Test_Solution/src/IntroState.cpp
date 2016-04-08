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
	auto state = GameState::getState();
	// set up the GUI
	mGuiRoot = mGui->LoadGUIsheet("IntroMenuLayout.layout");
	mGuiRoot->getChild("Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::Exit, this));
	mGuiRoot->getChild("SwitchState")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::GoToGame, this));
	
	
		//GameState::getState
		//
	
	CEGUI::ComboDropList* ArmCombobox = static_cast<CEGUI::ComboDropList*>(mGuiRoot->getChild("Settings Box/Arm Type/Arm ComboBox"));
	CEGUI::ListboxTextItem* arm1 = new CEGUI::ListboxTextItem("Test 1 ", 1);
	auto check = arm1->getSelectionBrushImage();
	arm1->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	CEGUI::ListboxTextItem* arm2 = new CEGUI::ListboxTextItem("Test 2 ", 2);
	arm1->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	ArmCombobox->addItem(arm1);
	ArmCombobox->addItem(arm2);
	//CEGUI::ComboDropList* LegCombobox = static_cast<CEGUI::ComboDropList*>(GUIROOT->getChildElement("Leg Combobox"));
	//CEGUI::ComboDropList* TorsoCombobox = static_cast<CEGUI::ComboDropList*>(GUIROOT->getChildElement("Torso Combobox"));
	//CEGUI::ComboDropList* NeckCombobox = static_cast<CEGUI::ComboDropList*>(GUIROOT->getChildElement("Neck Combobox"));
	//CEGUI::ComboDropList* TailCombobox = static_cast<CEGUI::ComboDropList*>(GUIROOT->getChildElement("Tail Combobox"));
	//mGuiRoot = CEGUI::WindowManager::getSingletonPtr()->createWindow("DefaultWindow", "IntroStateRoot");
	//mGuiObjects.push_back(mGui->createWindow("TaharezLook/Button", "IntroStateRoot/QuitButton"));
	//mGuiObjects.back()->setText("Quit");
	//mGuiObjects.back()->setSize(CEGUI::USize(CEGUI::UDim(0.10f,0), CEGUI::UDim(0.10f,0)));
	//mGuiRoot->addChild(mGuiObjects.back());
	//mGui->SetGUIsheet(mGuiRoot);
	//mGuiObjects.back()->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(mExit = true));
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
