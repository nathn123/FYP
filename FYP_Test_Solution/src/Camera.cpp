#include "..\header\Camera.h"


Camera::Camera(Ogre::String name, Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* rw, Ogre::Camera* camera)
{
	mName = name;
	mSceneMgr = sceneMgr;

	// camera node structure
	mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(mName);
	mTargetNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(mName + "_target");
	mCameraNode->setAutoTracking(true, mTargetNode);
	mCameraNode->setFixedYawAxis(true);
	
	if (camera == 0)
	{
		mCamera = mSceneMgr->createCamera(mName);
		mOwnCamera = true;
	}
	else
	{
		mCamera = camera;
		mCamera->setPosition(0, 0, 0);
		mOwnCamera = false;
	}
	mCameraNode->attachObject(mCamera);
	mCameraNode->setPosition(0, 100, 0);
	mVP = rw->addViewport(mCamera);
	mVP->setBackgroundColour(Ogre::ColourValue::Blue);
	mCamera->setAutoAspectRatio(true);
	mTightness = 0.01f;
}
Camera::~Camera()
{
	mCameraNode->detachAllObjects();
	if (mOwnCamera)
		delete mCamera;
	mSceneMgr->destroySceneNode(mName);
	mSceneMgr->destroySceneNode(mName + "_target");
}
void Camera::setTightness(Ogre::Real tightness)
{
	mTightness = tightness;
}
Ogre::Real Camera::getTightness()
{
	return mTightness;
}
Ogre::Vector3 Camera::getCameraPosition()
{
	return mCameraNode->getPosition();
}
bool Camera::TestRayCast(Ogre::Vector2 mousepos, Ogre::SceneNode* newNode)
{
	auto ray = mCamera->getCameraToViewportRay(mousepos.x, mousepos.y);
	auto raytest = mSceneMgr->createRayQuery(ray);
	raytest->setSortByDistance(true);
	if (raytest->execute().size() <= 0)
		return false;
	newNode =  raytest->getLastResults()[0].movable->getParentSceneNode();
	return true;
}
void Camera::instantUpdate(Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition)
{
	mCameraNode->setPosition(cameraPosition);
	mTargetNode->setPosition(targetPosition);
}
void Camera::update(Ogre::Real elapsedTime, Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition)
{
	// handle camera movement
	Ogre::Vector3 displacement;

	displacement = (cameraPosition - mCameraNode->getPosition()) * mTightness;
	mCameraNode->translate(displacement);
	displacement = (targetPosition - mTargetNode->getPosition()) * mTightness;
	mTargetNode->translate(displacement);
}
