#include "..\header\TestState.h"

TestState TestState::mTest;

TestState::TestState()
{
}

void TestState::enter()
{
	mInput = InputManager::getSingletonPtr();
	mRoot = Ogre::Root::getSingletonPtr();
	if (mRoot != StateManager::getSingletonPtr()->GetRoot())
		return;
	mSceneMgr = mRoot->getSceneManager("Scene");
	if (mSceneMgr->hasCamera("TestCam"))
		mCam = new Camera("TestCam", mSceneMgr, mRoot->getAutoCreatedWindow(), mSceneMgr->getCamera("IntroCam"));
	else
		mCam = new Camera("TestCam", mSceneMgr, mRoot->getAutoCreatedWindow());
	mCam->getOgreCam()->getViewport()->setBackgroundColour(Ogre::ColourValue::Red);
	mToggleTime = 1.0f;
	mPhysics = new Physics(mSceneMgr);
	step = false;
}
void TestState::exit()
{
	mCam->~Camera();
	//mScenMgr->clearScene();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void TestState::pause()
{

}
void TestState::resume()
{

}

bool TestState::frameStarted(const Ogre::FrameEvent& evt)
{
	mInput->capture();
	auto key = mInput->getKeyboard();
	auto mouse = mInput->getMouse();
	Ogre::SceneNode* Node = new Ogre::SceneNode(mSceneMgr);
	float movespeed = 0.05f;
	float rotspeed = 0.001f;


	auto test = mPhysics->mTestBody;
	if (key->isKeyDown(OIS::KC_L))
	{
		if (!mPhysics->mTestBody->isActive())
			mPhysics->mTestBody->activate();
		mPhysics->mTestBody->applyCentralForce(btVector3(0, 0, 100));
	}
	if (key->isKeyDown(OIS::KC_K))
	{
		if (!mPhysics->mTestBody->isActive())
			mPhysics->mTestBody->activate();
		mPhysics->mTestBody->applyCentralForce(btVector3(0, 100, 0));
	}
	if (key->isKeyDown(OIS::KC_J))
	{
		if (!mPhysics->mTestBody->isActive())
			mPhysics->mTestBody->activate();
		mPhysics->mTestBody->applyCentralForce(btVector3(100, 0, 0));
	}
	if (key->isKeyDown(OIS::KC_M))
	{
		if (!mPhysics->mTestBody->isActive())
			mPhysics->mTestBody->activate();
		mPhysics->mTestBody->applyCentralForce(btVector3(-100, 0, 0));
	}
	if (key->isKeyDown(OIS::KC_COMMA))
	{
		if (!mPhysics->mTestBody->isActive())
			mPhysics->mTestBody->activate();
		mPhysics->mTestBody->applyCentralForce(btVector3(0,-100, 0));
	}
	if (key->isKeyDown(OIS::KC_PERIOD))
	{
		if (!mPhysics->mTestBody->isActive())
			mPhysics->mTestBody->activate();
		mPhysics->mTestBody->applyCentralForce(btVector3(0, 0, -100));
	}
	
	if (key->isKeyDown(OIS::KC_G))
	{
		mActivation1 += 0.01f;
		if (mPhysics->mMuscles[0] == nullptr)
			mPhysics->mMuscles[0]->ActivationState(mActivation1);
	}

	if (key->isKeyDown(OIS::KC_H))
	{
		mActivation2 += 0.01f;
		if (mPhysics->mMuscles[1] == nullptr)
			mPhysics->mMuscles[1]->ActivationState(mActivation2);
	}

	if (key->isKeyDown(OIS::KC_B))
	{
		mActivation1 -= 0.01f;
		if (mPhysics->mMuscles[0] == nullptr)
			mPhysics->mMuscles[0]->ActivationState(mActivation1);
	}

	if (key->isKeyDown(OIS::KC_N))
	{
		mActivation2 -= 0.01f;
		if (mPhysics->mMuscles[1] == nullptr)
			mPhysics->mMuscles[1]->ActivationState(mActivation2);
	}

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

	if (key->isKeyDown(OIS::KC_T) && mToggleTime <=0.f)
	{
		mPhysics->TestLimb('C');
		mToggleTime = 2.0f;
	}
	else if (key->isKeyDown(OIS::KC_Y) && mToggleTime <= 0.f)
	{
		mPhysics->TestLimb('S');
		mToggleTime = 2.0f;
	}
	else if (key->isKeyDown(OIS::KC_U) && mToggleTime <= 0.f)
	{
		mPhysics->TestLimb('H');
		mToggleTime = 2.0f;
	}
	else if (key->isKeyDown(OIS::KC_Z) && mToggleTime <= 0.f)
	{
		mPhysics->TestLimb('Z');
		mToggleTime = 2.0f;
	}
	else if (key->isKeyDown(OIS::KC_SPACE) && mToggleTime <= 0.f)
	{
		step = !step;
		mToggleTime = 2.0f;
	}
	if (mouse->getMouseState().buttonDown(OIS::MB_Left))
	{
		mCam->TestRayCast(Ogre::Vector2(mouse->getMouseState().X.abs, mouse->getMouseState().Y.abs), Node);
	}
	mToggleTime -= evt.timeSinceLastFrame;
	mPhysics->Update(evt.timeSinceLastFrame, step);
	return true;
}
bool TestState::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}