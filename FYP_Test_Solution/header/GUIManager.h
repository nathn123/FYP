#pragma once
#include "OGRE\OgreSingleton.h"
#include "InputManager.h"
#include "CEGUI\CEGUI.h"
#include "CEGUI\RendererModules\Ogre\Renderer.h"
class GUIManager
{
public:
	GUIManager();
	~GUIManager();

	void Initialise();
	static GUIManager* getSingleton(){ return mGui; };
private:
	// functions
	void LoadGUIsheet(std::string filename);
	CEGUI::Window* createWindow(std::string type, std::string name);
	void SetGUIsheet(CEGUI::Window* sheet);
	// variables
	static GUIManager* mGui;
	CEGUI::OgreRenderer* mRenderer;
	InputManager* mInput;
	CEGUI::WindowManager* mWindow;

};

