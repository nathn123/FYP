#pragma once
#include <Ogre.h>
#include "Bone.h"
class Bone_Builder
{
public:
	Bone_Builder();
	~Bone_Builder();
	void SetDimensions(int width, int height, int length);
	void SetRelativePosition(Ogre::Vector3 pos, Ogre::SceneNode* mNode);
	bool BuildBone(Bone* newBone);
private:
	int mWidth, mHeight, mLength;
	Ogre::Vector3 mPosition;
	Ogre::SceneNode* mNode;

};

