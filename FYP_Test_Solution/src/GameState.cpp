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
	if (mRoot != StateManager::getSingletonPtr()->GetRoot())
		return;
	mScenMgr = mRoot->getSceneManager("Scene");
	if (mScenMgr->hasCamera("GameCam"))
		mCam = new Camera("GameCam", mScenMgr, mRoot->getAutoCreatedWindow(), mScenMgr->getCamera("GameCam"));
	else
		mCam = new Camera("GameCam", mScenMgr, mRoot->getAutoCreatedWindow());
	mCam->getOgreCam()->getViewport()->setBackgroundColour(Ogre::ColourValue::Black);
	mGui = GUIManager::getSingleton();
	mPhysics = new Physics(mScenMgr);



	mExit = false;
	if (!CEGUI::FontManager::getSingleton().isDefined("DejaVuSans-10"))
		CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
	// set up the GUI
	auto font = &CEGUI::FontManager::getSingleton().get("DejaVuSans-10");
	mGUIRoot = mGui->LoadGUIsheet("GameLayout.layout");
	mGUIRoot->getChild("Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState::GoToIntro, this));
	mGUIRoot->getChild("BuildSkeleton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState::BuildSkeleton, this));
	CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*> (mGUIRoot->getChild("skeleton Setting/Checkbox"));
	checkbox->setSelected(true);
	mGUIRoot->getChild("skeleton Setting/Checkbox")->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&GameState::HideSettings, this));
	// add the drop box options
	CEGUI::Combobox* ArmList = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Arm Combobox"));
	ArmList->setReadOnly(true);
	ArmList->setSelection(0, 10);
	ArmList->activate();
	
	CEGUI::ListboxTextItem* ArmN = new CEGUI::ListboxTextItem("No Arms", 0);
	ArmN->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	ArmN->setFont(font);
	ArmList->addItem(ArmN);
	auto ArmS = new CEGUI::ListboxTextItem("Short Arms", 1);
	ArmS->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	ArmList->addItem(ArmS);
	auto ArmL = new CEGUI::ListboxTextItem("Long Arms", 2);
	ArmL->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	ArmList->addItem(ArmL);


	CEGUI::Combobox* LegList = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Leg Combobox"));
	auto LegU = new CEGUI::ListboxTextItem("Uninverted Legs", 0);
	LegU->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	LegU->setFont(font);
	LegList->addItem(LegU);
	auto LegI = new CEGUI::ListboxTextItem("Inverted Legs", 1);
	LegI->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	LegI->setFont(font);
	LegList->addItem(LegI);

	CEGUI::Combobox* TorsoList = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Torso Combobox"));
	auto TorsoU = new CEGUI::ListboxTextItem("Upright", 0);
	TorsoU->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	TorsoU->setFont(font);
	TorsoList->addItem(TorsoU);
	auto TorsoH = new CEGUI::ListboxTextItem("Horizontal", 1);
	TorsoH->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	TorsoH->setFont(font);
	TorsoList->addItem(TorsoH);

	CEGUI::Combobox* NeckList = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Neck Combobox"));
	auto NeckL = new CEGUI::ListboxTextItem("Long Neck", 0);
	NeckL->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	NeckL->setFont(font);
	NeckList->addItem(NeckL);
	auto NeckS = new CEGUI::ListboxTextItem("Short Neck", 1);
	NeckS->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	NeckS->setFont(font);
	NeckList->addItem(NeckS);

	CEGUI::Combobox* TailList = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Tail Combobox"));
	auto TailN = new CEGUI::ListboxTextItem("No Tail", 0);
	TailN->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	TailN->setFont(font);
	TailList->addItem(TailN);
	auto TailS = new CEGUI::ListboxTextItem("Short Tail", 1);
	TailS->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	TailS->setFont(font);
	TailList->addItem(TailS);
	auto TailL = new CEGUI::ListboxTextItem("Long Tail", 2);
	TailL->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
	TailL->setFont(font);
	TailList->addItem(TailL);

	//ArmList->setVisible(true);
	//LegList->setVisible(true);
	//TorsoList->setVisible(true);
	//NeckList->setVisible(true);
	//TailList->setVisible(true);

}
void GameState::exit()
{
	
	mCam->~Camera();
	//mScenMgr->clearScene();
	//mScenMgr->destroyAllCameras();
	delete mPhysics;
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
	float movespeed = 0.05f;
	float rotspeed = 0.001f;
	if (key->isKeyDown(OIS::KC_UP))
	{
		// rotate up
		mCam->Rotateupdate(Ogre::Vector3::UNIT_X, Ogre::Radian(rotspeed));
	}
	if (key->isKeyDown(OIS::KC_DOWN))
	{
		mCam->Rotateupdate(Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Radian(rotspeed));
	}
	if (key->isKeyDown(OIS::KC_LEFT))
	{
		mCam->Rotateupdate(Ogre::Vector3::UNIT_Y, Ogre::Radian(rotspeed));
	}
	if (key->isKeyDown(OIS::KC_RIGHT))
	{
		mCam->Rotateupdate(Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Radian(rotspeed));
	}

	if (key->isKeyDown(OIS::KC_A))
	{
		// pan up
		mCam->Transupdate(Ogre::Vector3::UNIT_X * movespeed);
	}
	if (key->isKeyDown(OIS::KC_D))
	{
		//pan down
		mCam->Transupdate(Ogre::Vector3::NEGATIVE_UNIT_X * movespeed);
	}
	if (key->isKeyDown(OIS::KC_W))
	{
		//pan left
		mCam->Transupdate(Ogre::Vector3::NEGATIVE_UNIT_Z * movespeed);
	}
	if (key->isKeyDown(OIS::KC_S))
	{
		//pan right
		mCam->Transupdate(Ogre::Vector3::UNIT_Z * movespeed);
	}
	if (key->isKeyDown(OIS::KC_Q))
	{
		mCam->Transupdate(Ogre::Vector3::UNIT_Y * movespeed);
	}
	if (key->isKeyDown(OIS::KC_E))
	{
		mCam->Transupdate(Ogre::Vector3::NEGATIVE_UNIT_Y * movespeed);
	}
	if (key->isKeyDown(OIS::KC_R))
		mCam->instantUpdate(Ogre::Vector3(90, 100, 0), Ogre::Vector3(0, 10, 0));

	return true;
}
bool GameState::frameEnded(const Ogre::FrameEvent& evt)
{
	if (mExit)
		return false;
	mPhysics->Update(evt.timeSinceLastFrame, true);
	return true;
}
void GameState::GoToIntro()
{
	changeState(IntroState::getState());
}
void GameState::BuildSkeleton()
{
	CEGUI::Editbox* heightbox = static_cast<CEGUI::Editbox*>(mGUIRoot->getChild("Settings Box/Skeleton Height/HeightEdit"));
	CEGUI::Editbox* widthbox = static_cast<CEGUI::Editbox*>(mGUIRoot->getChild("Settings Box/Skeleton Width/WidthEditBox"));
	CEGUI::Editbox* neckbox = static_cast<CEGUI::Editbox*>(mGUIRoot->getChild("Settings Box/Neck Incline/NeckEditbox"));
	CEGUI::Editbox* tailbox = static_cast<CEGUI::Editbox*>(mGUIRoot->getChild("Settings Box/Tail Incline/TailEditbox"));
	CEGUI::Editbox* Xbox = static_cast<CEGUI::Editbox*>(mGUIRoot->getChild("Settings Box/Spawn Position/X Editbox"));
	CEGUI::Editbox* Ybox = static_cast<CEGUI::Editbox*>(mGUIRoot->getChild("Settings Box/Spawn Position/Y Editbox"));
	CEGUI::Editbox* Zbox = static_cast<CEGUI::Editbox*>(mGUIRoot->getChild("Settings Box/Spawn Position/Z Editbox"));
	Skeleton_Builder::TorsoType Torso;
	Skeleton_Builder::ArmType Arm;
	Skeleton_Builder::LegType Leg;
	Skeleton_Builder::NeckType Neck;
	Skeleton_Builder::TailType Tail;
	auto legitem = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Leg Combobox"))->getSelectedItem();
	auto torsoitem = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Torso Combobox"))->getSelectedItem();
	auto armitem = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Arm Combobox"))->getSelectedItem();
	auto tailitem = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Tail Combobox"))->getSelectedItem();
	auto neckitem = static_cast<CEGUI::Combobox*>(mGUIRoot->getChild("Settings Box/Neck Combobox"))->getSelectedItem();

	if (legitem->getID() == 0)
		Leg = Skeleton_Builder::LegType::Uninverted;
	else
		Leg = Skeleton_Builder::LegType::Inverted;

	if (torsoitem->getID() == 0)
		Torso = Skeleton_Builder::TorsoType::Upright;
	else
		Torso = Skeleton_Builder::TorsoType::Horizontal;

	if (armitem->getID() == 0)
		Arm = Skeleton_Builder::ArmType::NoArms;
	else if (armitem->getID() == 1)
		Arm = Skeleton_Builder::ArmType::ShortArms;
	else
		Arm = Skeleton_Builder::ArmType::LongArms;

	if (tailitem->getID() == 0)
		Tail = Skeleton_Builder::TailType::NoTail;
	else if (tailitem->getID() == 1)
		Tail = Skeleton_Builder::TailType::ShortTail;
	else
		Tail = Skeleton_Builder::TailType::LongTail;

	if (neckitem->getID() == 1)
		Neck = Skeleton_Builder::NeckType::ShortNeck;
	else if (neckitem->getID() == 0)
		Neck = Skeleton_Builder::NeckType::LongNeck;

	auto Position = Ogre::Vector3(std::stof(Xbox->getText().c_str()), std::stof(Ybox->getText().c_str()), std::stof(Zbox->getText().c_str()));
	mPhysics->BuildCharacter(Torso, Arm, Leg,Neck,Tail,
		std::stof(heightbox->getText().c_str()),
		std::stof(widthbox->getText().c_str()),
		std::stof(neckbox->getText().c_str()),
		std::stof(tailbox->getText().c_str()),
		static_cast<CEGUI::ToggleButton*>(mGUIRoot->getChild("Settings Box/Hide Arm/Checkbox"))->isSelected(),
		static_cast<CEGUI::ToggleButton*>(mGUIRoot->getChild("Settings Box/Hide Leg/Checkbox"))->isSelected(),
		static_cast<CEGUI::ToggleButton*>(mGUIRoot->getChild("Settings Box/Hide Neck/Checkbox"))->isSelected(),
		static_cast<CEGUI::ToggleButton*>(mGUIRoot->getChild("Settings Box/Has Muscles/Checkbox"))->isSelected(),
		static_cast<CEGUI::ToggleButton*>(mGUIRoot->getChild("Settings Box/Is Fixed/Checkbox"))->isSelected(),
		Position);
}
void GameState::HideSettings()
{
	auto test = mGUIRoot->getChild("Settings Box")->isVisible();
	auto visabiliy = !mGUIRoot->getChild("Settings Box")->isVisible();
	mGUIRoot->getChild("Settings Box")->setVisible(visabiliy);
}
float GameState::ParseText(CEGUI::String ToParse)
{
	
	float returnval = std::stof(ToParse.c_str());;

	
	return returnval;
}