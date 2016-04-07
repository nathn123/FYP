#pragma once
#include <Ogre.h>
#include <OIS\OIS.h>
// all code take from http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Using+OIS#InputManager_Class
class InputManager : public OIS::KeyListener, public OIS::MouseListener
{
public:
	
	virtual ~InputManager();
	void initialise(Ogre::RenderWindow* rw);
	void capture();

	void addKeyListener(OIS::KeyListener* keyListener, const Ogre::String& name);
	void removeKeyListener(const Ogre::String& name);
	void addMouseListener(OIS::MouseListener* mouseListener, const Ogre::String& name);
	void removeMouseListener(const Ogre::String& name);

	void removeAllListeners();
	void removeAllKeyListeners();
	void removeAllMouseListeners();


	void setWindowSize(int width, int height);

	OIS::Mouse* getMouse();
	OIS::Keyboard* getKeyboard();


	static InputManager* getSingletonPtr();
private:
	InputManager();
	InputManager(const InputManager&);
	InputManager & operator = (const InputManager&);
	static InputManager* mInputmanager;

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::InputManager* mInputSystem;

	std::map<std::string, OIS::KeyListener*> mKeyListeners;
	std::map<std::string, OIS::MouseListener*> mMouseListeners;
};

