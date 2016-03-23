#include "../header/Camera_FrameListener.h"


Camera_FrameListener::Camera_FrameListener(Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* rw, Ogre::String name)
{
	mCam = new Camera(name,sceneMgr,rw);
	mInput = InputManager::getSingletonPtr();
	movespeed = 0.01f;
	mCam->instantUpdate(Ogre::Vector3(90, 100, 0), Ogre::Vector3(0, 10, 0));
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
		auto dir = mLastTrackedPos - mCam->getCameraPosition();
		rotQuat.FromAngleAxis(Ogre::Degree(movespeed), mCam->getCameraPosition().UNIT_X);
		dir = rotQuat * dir;
		dir.normalise();
		// rotate up
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + dir, mLastTrackedPos);
	}
	if (key->isKeyDown(OIS::KC_DOWN))
	{
		auto dir = mLastTrackedPos - mCam->getCameraPosition();
		rotQuat.FromAngleAxis(Ogre::Degree(-movespeed), mCam->getCameraPosition().UNIT_X);
		dir = rotQuat * dir;
		//rotate down
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + dir, mLastTrackedPos);
	}
	if (key->isKeyDown(OIS::KC_LEFT))
	{
		auto dir = mLastTrackedPos - mCam->getCameraPosition();
		rotQuat.FromAngleAxis(Ogre::Degree(movespeed), mCam->getCameraPosition().UNIT_Y);
		dir = rotQuat * dir;
		//rotate left
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + dir, mLastTrackedPos);
	}
	if (key->isKeyDown(OIS::KC_RIGHT))
	{
		auto dir = mLastTrackedPos - mCam->getCameraPosition();
		rotQuat.FromAngleAxis(Ogre::Degree(-movespeed), mCam->getCameraPosition().UNIT_Y);
		dir = rotQuat * dir;
		//rotate right
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + dir, mLastTrackedPos);
	}
	// when panning we disconnect from the node
	if (key->isKeyDown(OIS::KC_W) || key->isKeyDown(OIS::KC_S) || key->isKeyDown(OIS::KC_A) || key->isKeyDown(OIS::KC_D))
		mTracking = nullptr;

	if (key->isKeyDown(OIS::KC_W))
	{
		// pan up
		mLastTrackedPos += mCam->getCameraPosition().UNIT_Y;
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + mCam->getCameraPosition().UNIT_Y, mLastTrackedPos + mCam->getCameraPosition().UNIT_Y);
	}
	if (key->isKeyDown(OIS::KC_S))
	{
		//pan down
		mLastTrackedPos += mCam->getCameraPosition().NEGATIVE_UNIT_Y;
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + mCam->getCameraPosition().NEGATIVE_UNIT_Y, mLastTrackedPos + mCam->getCameraPosition().NEGATIVE_UNIT_Y);
	}
	if (key->isKeyDown(OIS::KC_A))
	{
		//pan left
		mLastTrackedPos += mCam->getCameraPosition().UNIT_X;
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + mCam->getCameraPosition().UNIT_X, mLastTrackedPos + mCam->getCameraPosition().UNIT_X);
	}
	if (key->isKeyDown(OIS::KC_D))
	{
		//pan right
		mLastTrackedPos += mCam->getCameraPosition().NEGATIVE_UNIT_X;
		mCam->update(evt.timeSinceLastFrame, mCam->getCameraPosition() + mCam->getCameraPosition().NEGATIVE_UNIT_X, mLastTrackedPos + mCam->getCameraPosition().NEGATIVE_UNIT_X);
	}
	if (mouse.buttonDown(OIS::MB_Left))
	{
		//new target
		mCam->TestRayCast(Ogre::Vector2(mouse.width, mouse.height), mTracking);
	}
	if (key->isKeyDown(OIS::KC_R))
		mCam->instantUpdate(Ogre::Vector3(90, 100, 0), Ogre::Vector3(0, 10, 0));


	return true;
}