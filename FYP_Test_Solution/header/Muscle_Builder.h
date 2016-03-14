#pragma once
#include "Muscle.h"
class Muscle_Builder
{
public:
	Muscle_Builder(Ogre::SceneManager* scenemanger, btDynamicsWorld* world);
	~Muscle_Builder();
	bool CreateMuscle(Bone* boneA, Bone* boneB);

private:
	Ogre::SceneManager* mSceneMgr;
	btDynamicsWorld* mWorld;
	std::vector<Muscle*> mMuscles;

};