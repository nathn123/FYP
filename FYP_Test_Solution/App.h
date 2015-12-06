#pragma once
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OGRE\OgreRoot.h>
#include "InputManager.h"
#include <OgreWindowEventUtilities.h>
class App : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
	App();
	virtual ~App();
	bool Go();
	void Create_Scene();
protected:
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
	Ogre::Root* mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	InputManager* mInputManager;
};
