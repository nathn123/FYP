#include "../header/Camera_FrameListener.h"


Camera_FrameListener::Camera_FrameListener(Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* rw, Ogre::String name)
{
	mCam = new Camera(name,sceneMgr,rw);
	mInput = InputManager::getSingletonPtr();
	movespeed = 0.05f;
	rotspeed = 0.001f; 
	mCam->instantUpdate(Ogre::Vector3(0, 300, 300), Ogre::Vector3(0, 100, 0));
}
Camera_FrameListener::~Camera_FrameListener()
{
	
}
bool Camera_FrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	Ogre::Ray* clickray = new Ogre::Ray();

	auto key = mInput->getKeyboard();
	auto mouse = mInput->getMouse()->getMouseState();
	// we need to know if we are tracking something and where it is
	if (mTracking != nullptr)
		mLastTrackedPos = mTracking->_getDerivedPosition();
	Ogre::Quaternion rotQuat;
	rotQuat = Ogre::Quaternion::IDENTITY;
	// when we rotate the target stay the same but we move the camera
	if (key->isKeyDown(OIS::KC_UP) )
	{
		// rotate up
		mCam->Rotateupdate(Ogre::Vector3::UNIT_X,Ogre::Radian(rotspeed));
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
	// when panning we disconnect from the node
	if (key->isKeyDown(OIS::KC_W) || key->isKeyDown(OIS::KC_S) || key->isKeyDown(OIS::KC_A) || key->isKeyDown(OIS::KC_D))
		mTracking = nullptr;

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
	if (mouse.buttonDown(OIS::MB_Left))
	{
		//new target
		mCam->TestRayCast(Ogre::Vector2(Ogre::Real(mouse.width), Ogre::Real(mouse.height)), mTracking);
	}
	if (key->isKeyDown(OIS::KC_R))
		mCam->instantUpdate(Ogre::Vector3(90, 100, 0), Ogre::Vector3(0, 10, 0));


	return true;
}