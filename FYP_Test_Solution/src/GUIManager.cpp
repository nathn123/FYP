#include "..\header\GUIManager.h"

GUIManager* GUIManager::mGui;
GUIManager::GUIManager()
{
	mInput = InputManager::getSingletonPtr();
}


GUIManager::~GUIManager()
{
}

void GUIManager::Initialise()
{
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	mWindow = &CEGUI::WindowManager::getSingleton();
	// default resource groups
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	// setting initial style
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
}
void GUIManager::LoadGUIsheet(std::string filename)
{
	CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(filename);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
};
CEGUI::Window* GUIManager::createWindow(std::string type, std::string name)
{
	return mWindow->createWindow(type, name);
}
void GUIManager::SetGUIsheet(CEGUI::Window* sheet)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

