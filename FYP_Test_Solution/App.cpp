#include "App.h"
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include <OgreEntity.h>
#include "Physics_FrameListener.h"
#include "Camera_FrameListener.h"

App::App() : mRoot(0), mWindow(0), mSceneMgr(0), mCamera(0),
mPluginsCfg(Ogre::StringUtil::BLANK),
mResourcesCfg(Ogre::StringUtil::BLANK)
{

}
App::~App()
{
	// remove window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

bool App::Go()
{


	//configure




	// create the SceneManager
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");


	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	// set up inputmanager singleton
	mInputManager = InputManager::getSingletonPtr();
	mInputManager->initialise(mWindow);

	//set initial mouse clipping size
	windowResized(mWindow);
	//register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	Create_Scene();
	mRoot->addFrameListener(this); // set this as framelistener
	mRoot->startRendering();
	return true;
}

void App::Create_Scene()
{


	Camera_FrameListener* camframe = new Camera_FrameListener(mSceneMgr,mWindow, "Maincam");
	Physics_FrameListener* physicsframe = new Physics_FrameListener(mSceneMgr);
	mRoot->addFrameListener(camframe);
	mRoot->addFrameListener(physicsframe);



}

void App::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);
	mInputManager->setWindowSize(width, height);
}


void App::windowClosed(Ogre::RenderWindow* rw)
{
	if (rw == mWindow) // only close for window that created OIS
	{
		if (mInputManager)
		{
			mInputManager->removeAllListeners();
			mInputManager->~InputManager();
		}
	}
}


bool App::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mWindow->isClosed())
		return false;

	// Need to capture/update each device
	mInputManager->capture();
	auto key = mInputManager->getKeyboard();
	if (key->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
}
