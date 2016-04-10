#include "..\header\StateManager.h"
#include "State.h"

template<> StateManager* Ogre::Singleton<StateManager>::msSingleton = 0;

StateManager::StateManager() : mRoot(0), mInput(0)
{
}
StateManager::~StateManager()
{
	while (!mStates.empty())
	{
		mStates.back()->exit();
		mStates.pop_back();
	}
	if (mInput)
		delete mInput;
	if (mRoot)
		delete mRoot;
}
Ogre::Root* StateManager::GetRoot()
{
	return mRoot;
}
void StateManager::start(State* state)
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
	if (mRoot != Ogre::Root::getSingletonPtr())
		return;
	if (!configure())
		return;
	setupResources();
	mRoot->addFrameListener(this);
	mGui = new GUIManager();
	mInput = InputManager::getSingletonPtr();
	mInput->initialise(mRW);
	mGui->Initialise();
	//mInput->addKeyListener(mInput, "default");
	//mInput->addMouseListener(mInput, "default");
	changeState(state);
	mRoot->startRendering();
}
void StateManager::changeState(State* state)
{
	
	if (!mStates.empty())
	{
		mStates.back()->exit();
		mStates.pop_back();
	}
	mStates.push_back(state);
	mStates.back()->enter();
}
void StateManager::pushState(State* state)
{
	//pause the current state
	if (!mStates.empty())
		mStates.back()->pause();
	mStates.push_back(state);
	mStates.back()->enter();
}
void StateManager::popState()
{
	// resume the previous state
	if (!mStates.empty())
	{
		mStates.back()->exit();
		mStates.pop_back();
	}
	if (!mStates.empty())
		mStates.back()->resume();

}
void StateManager::setupResources()
{
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
	// set default mipmap level
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	//initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	// create the SceneManager
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager","Scene");
	CreateScene();
}
void StateManager::CreateScene()
{
	
	// Lights 
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	Ogre::Light* L = mSceneMgr->createLight("MainLight");
	L->setType(Ogre::Light::LT_DIRECTIONAL);
	L->setDirection(-0.5, -0.5, 0);
	//Camera

	// create terrain
	Ogre::Entity* FloorEnt;
	Ogre::Plane FloorP;
	FloorP.normal = Ogre::Vector3(0, 1, 0); FloorP.d = 0;
	Ogre::MeshManager::getSingleton().createPlane("FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, FloorP, 20000, 20000, 20, 20, true, 1, 9000, 900, Ogre::Vector3::UNIT_Z);
	FloorEnt = mSceneMgr->createEntity("Floor", "FloorPlane");
	FloorEnt->setMaterialName("Examples/BumpyMetal");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(FloorEnt);
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

}
bool StateManager::configure()
{
	if (!mRoot->restoreConfig())
		if (!mRoot->showConfigDialog())
			return false;
	mRW = mRoot->initialise(true, "Final Year Project - Nathan O'Connor");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	return true;
}
bool StateManager::frameStarted(const Ogre::FrameEvent& evt)
{
	return mStates.back()->frameStarted(evt);
}
bool StateManager::frameEnded(const Ogre::FrameEvent& evt)
{
	return mStates.back()->frameEnded(evt);
}
