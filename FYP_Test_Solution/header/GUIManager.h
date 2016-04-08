#pragma once
#include "OGRE\OgreSingleton.h"
#include "InputManager.h"
#include "CEGUI\CEGUI.h"
#include "CEGUI\RendererModules\Ogre\Renderer.h"
class GUIManager : public OIS::KeyListener, public OIS::MouseListener
{
public:
	GUIManager();
	~GUIManager();

	// functions
	CEGUI::Window* LoadGUIsheet(std::string filename);
	CEGUI::Window* createWindow(std::string type, std::string name);
	void SetGUIsheet(CEGUI::Window* sheet);
	void Initialise();
	static GUIManager* getSingleton(){ return mGui; };
private:
	// OIS::KeyListener
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	// OIS::MouseListener
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	static GUIManager* mGui;
	CEGUI::OgreRenderer* mRenderer;
	InputManager* mInput;
	CEGUI::WindowManager* mWindow;

};

