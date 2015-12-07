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
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif
	// construct ogre root
	mRoot = new Ogre::Root(mPluginsCfg);
	// set up resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// go through all sections and settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	//configure
	//show the config dialog and initialise system
	if (!(/*mRoot->restoreConfig()||*/ mRoot->showConfigDialog()))
	{
		return false;
	}
	mWindow = mRoot->initialise(true, "Final Year Project - Nathan O'Connor");
	// set default mipmap level
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	//initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
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
	// set up physics
	Physics_FrameListener* mPhysics = new Physics_FrameListener(mSceneMgr);
	mRoot->addFrameListener(mPhysics);
	mRoot->addFrameListener(this); // set this as framelistener
	mRoot->startRendering();
	return true;
}

void App::Create_Scene()
{
	// Lights 
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	Ogre::Light* L = mSceneMgr->createLight("MainLight");
	L->setType(Ogre::Light::LT_DIRECTIONAL);
	L->setDirection(-0.5, -0.5, 0);
	//Camera
	mCamera = mSceneMgr->createCamera("MainCam");
	mCamera->setPosition(Ogre::Vector3(0, 300, 80));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);
	//auto camerama = new OgreBites::SdkCameraMan(mCamera);
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(1, 0, 0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	// create terrain
	Ogre::Entity* FloorEnt;
	Ogre::Plane FloorP;
	FloorP.normal = Ogre::Vector3(0, 1, 0); FloorP.d = 0;
	Ogre::MeshManager::getSingleton().createPlane("FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, FloorP, 20000, 20000, 20, 20, true, 1, 9000, 900, Ogre::Vector3::UNIT_Z);
	FloorEnt = mSceneMgr->createEntity("Floor", "FloorPlane");
	FloorEnt->setMaterialName("Examples/BumpyMetal");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(FloorEnt);


	Physics_FrameListener* physicsframe = new Physics_FrameListener(mSceneMgr);
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
