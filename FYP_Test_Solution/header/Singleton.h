#pragma once
#include<Ogre.h>
class Singletons
{
public:
	static Ogre::SceneManager* getInstance();
	~Singletons();


};