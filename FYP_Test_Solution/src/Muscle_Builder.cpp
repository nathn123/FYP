#include "Muscle_Builder.h"

Muscle_Builder::Muscle_Builder(Ogre::SceneManager* scenemanger, btDynamicsWorld* world)
{
	mSceneMgr = scenemanger;
	mWorld = world;
}
std::vector<Muscle*> Muscle_Builder::CreateMuscle(Bone* boneA, Bone* boneB)
{
	std::vector<Muscle*> mMuscles;
	Muscle* T = new Muscle(boneA, boneB, mWorld);
	Muscle* B = new Muscle(boneB, boneA, mWorld);
	mMuscles.push_back(T);
	mMuscles.push_back(B);
	return mMuscles;
}