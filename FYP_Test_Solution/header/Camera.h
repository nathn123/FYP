#pragma once
#include<Ogre.h>
class Camera
{
public:
	Camera(Ogre::String name, Ogre::SceneManager* sceneMgr,Ogre::RenderWindow* rw, Ogre::Camera* camera = 0);
	~Camera();

	void setTightness(Ogre::Real tightness);
	Ogre::Real getTightness();
	Ogre::Vector3 getCameraPosition();
	Ogre::Camera* getOgreCam();
	bool TestRayCast(Ogre::Vector2 mousepos, Ogre::SceneNode* newNode);
	void instantUpdate(Ogre::Vector3 cameraPosition, Ogre::Vector3 targetPosition);
	void Transupdate(Ogre::Vector3 translate);
	void Rotateupdate(Ogre::Vector3 axis, Ogre::Radian turnAmount);


protected:
	Ogre::SceneNode* mTargetNode; // camera target
	Ogre::SceneNode* mCameraNode; // the camera
	Ogre::Camera* mCamera; // Ogre Camera
	Ogre::Viewport* mVP;
	Ogre::SceneManager* mSceneMgr;
	Ogre::String mName;
	bool mOwnCamera; // to check if the camera was created inside the class
	Ogre::Real mTightness; // check the movement of the camera 1 means tight movement 0 means no movement
};

