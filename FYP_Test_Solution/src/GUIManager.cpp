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
	mWindow = CEGUI::WindowManager::getSingletonPtr();
	mInput->addKeyListener(this, "GUI");
	mInput->addMouseListener(this, "GUI");
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
CEGUI::Window* GUIManager::LoadGUIsheet(std::string filename)
{
	
	CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(filename);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
	return guiRoot;
};
CEGUI::Window* GUIManager::createWindow(std::string type, std::string name)
{
	auto win = CEGUI::WindowManager::getSingletonPtr()->createWindow(type, name);
	return win;
}
void GUIManager::SetGUIsheet(CEGUI::Window* sheet)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}
// OIS::KeyListener
bool GUIManager::keyPressed(const OIS::KeyEvent &arg)
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)arg.key);
	context.injectChar((CEGUI::Key::Scan)arg.text);
	return true;
}
bool GUIManager::keyReleased(const OIS::KeyEvent &arg)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
	return true;
}
// OIS::MouseListener
CEGUI::MouseButton GUIManager::convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}
bool GUIManager::mouseMoved(const OIS::MouseEvent &arg)
{
	auto &sys = CEGUI::System::getSingleton().getDefaultGUIContext();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}
bool GUIManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	return true;
}
bool GUIManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

