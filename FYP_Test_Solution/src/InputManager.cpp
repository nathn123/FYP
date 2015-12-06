#include "..\header\InputManager.h"

InputManager* InputManager::mInputmanager;

InputManager::InputManager() : mMouse(0), mKeyboard(0), mInputSystem(0)
{
}


InputManager::~InputManager()
{
	if (mInputmanager)
	{
		if (mMouse)
		{
			mInputSystem->destroyInputObject(mMouse);
			mMouse = 0;
		}
		if (mKeyboard)
		{
			mInputSystem->destroyInputObject(mKeyboard);
			mKeyboard = 0;
		}
		mInputSystem->destroyInputSystem(mInputSystem);
		mInputSystem = 0;

		mKeyListeners.clear();
		mMouseListeners.clear();
	}
}

void InputManager::initialise(Ogre::RenderWindow* rw)
{
	if (!mInputSystem)
	{
		OIS::ParamList paramList;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		rw->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << (unsigned int)windowHnd;
		paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		mInputSystem = OIS::InputManager::createInputSystem(paramList);

		if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0)
		{
			mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
			mKeyboard->setEventCallback(this);
		}
		if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0)
		{
			mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));
			mMouse->setEventCallback(this);

			// window size
			unsigned int width, height, depth;
			int left, top;
			rw->getMetrics(width, height, depth, left, top);

			this->setWindowSize(width, height);
		}
	}
}

void InputManager::capture()
{
	if (mMouse)
		mMouse->capture();
	if (mKeyboard)
		mKeyboard->capture();
}

void InputManager::addKeyListener(OIS::KeyListener* keyListener, const std::string& instanceName)
{
	if (mKeyboard)
	{
		if (mKeyListeners.find(instanceName) == mKeyListeners.end())
			mKeyListeners[instanceName] = keyListener;
	}
}

void InputManager::addMouseListener(OIS::MouseListener* mouseListener, const std::string& instanceName)
{
	if (mMouse)
	{
		if (mMouseListeners.find(instanceName) == mMouseListeners.end())
			mMouseListeners[instanceName] = mouseListener;
	}
}

void InputManager::removeKeyListener(const std::string& instanceName)
{
	if (mKeyListeners.find(instanceName) != mKeyListeners.end())
		mKeyListeners.erase(mKeyListeners.find(instanceName));
}

void InputManager::removeMouseListener(const std::string& instanceName)
{
	if (mMouseListeners.find(instanceName) != mMouseListeners.end())
		mMouseListeners.erase(mMouseListeners.find(instanceName));
}

void InputManager::removeAllListeners()
{
	mKeyListeners.clear();
	mMouseListeners.clear();
}
void InputManager::removeAllKeyListeners()
{
	mKeyListeners.clear();
}

void InputManager::removeAllMouseListeners()
{
	mMouseListeners.clear();
}

void InputManager::setWindowSize(int width, int height)
{
	const OIS::MouseState& mouseState = mMouse->getMouseState();
	mouseState.width = width;
	mouseState.height = height;
}

OIS::Mouse* InputManager::getMouse()
{
	return mMouse;
}

OIS::Keyboard* InputManager::getKeyboard()
{
	return mKeyboard;
}

bool InputManager::keyPressed(const OIS::KeyEvent &e)
{
	for (auto key : mKeyListeners)
		if (key.second->keyPressed(e))
			break;
		return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &e)
{
	for (auto key: mKeyListeners)
		if (key.second->keyPressed(e))
			break;
	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent &e)
{
	for (auto mouse:mMouseListeners)
		if (mouse.second->mouseMoved(e))
			break;
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	for (auto mouse: mMouseListeners)
		if (mouse.second->mousePressed(e,id))
			break;
	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	for (auto mouse: mMouseListeners)
		if (mouse.second->mouseReleased(e,id))
			break;
	return true;
}

InputManager* InputManager::getSingletonPtr()
{
	if (!mInputmanager)
		mInputmanager = new InputManager();

	return mInputmanager;
}