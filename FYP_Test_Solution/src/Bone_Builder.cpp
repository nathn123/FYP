#include "..\header\Bone_Builder.h"


Bone_Builder::Bone_Builder(Ogre::SceneManager* scenemanger, btDynamicsWorld* world)
{
	mSceneMgr = scenemanger;
	mWorld = world;
}


Bone_Builder::~Bone_Builder()
{
}

void Bone_Builder::SetDimensions(int width, int height)
{
	mWidth = width;
	mHeight = height;
	mLength = 30;
}

void Bone_Builder::SetRelativePosition(Ogre::Vector3 relpos, Ogre::Quaternion RelRot, Ogre::SceneNode* parent)
{
	mPosition = relpos;
	mRotation = RelRot;
	mNode = parent;
}

bool Bone_Builder::BuildBone(Bone* newBone)
{
	if (mWidth <= 0 || mHeight <= 0 || mPosition.isNaN() || mNode == nullptr) // ensure it has been initialised
		return false;
	Ogre::ManualObject* bonecube = mSceneMgr->createManualObject("bone");
	bonecube->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

	// bottom face
	bonecube->position(0, 0, 0);
	bonecube->position(mWidth, 0, 0);
	bonecube->position(mWidth, 0, mLength);
	bonecube->position(0, 0, mLength);

	// top face
	bonecube->position(0, mHeight, 0);
	bonecube->position(mWidth, mHeight, 0);
	bonecube->position(mWidth, mHeight, mLength);
	bonecube->position(0, mHeight, mLength);

	//top triangle 1
	bonecube->index(0);
	bonecube->index(1);
	bonecube->index(2);
	//top triangle 2 
	bonecube->index(2);
	bonecube->index(0);
	bonecube->index(3);

	//bottom triangle 1
	bonecube->index(4);
	bonecube->index(5);
	bonecube->index(6);
	//bottom triangle 2
	bonecube->index(6);
	bonecube->index(4);
	bonecube->index(7);

	//left triangle 1
	bonecube->index(3);
	bonecube->index(2);
	bonecube->index(7);
	//left triangle 2
	bonecube->index(7);
	bonecube->index(2);
	bonecube->index(6);

	//right triangle 1
	bonecube->index(0);
	bonecube->index(1);
	bonecube->index(4);
	//right triangle 2
	bonecube->index(4);
	bonecube->index(1);
	bonecube->index(5);

	//front triangle 1
	bonecube->index(1);
	bonecube->index(2);
	bonecube->index(5);
	//front triangle 2
	bonecube->index(5);
	bonecube->index(2);
	bonecube->index(6);

	//back triangle 1
	bonecube->index(0);
	bonecube->index(3);
	bonecube->index(4);
	//back triangle 2
	bonecube->index(4);
	bonecube->index(3);
	bonecube->index(7);

	bonecube->end();
	newBone = new Bone(bonecube,mPosition,mRotation,mNode,mWorld);
	this->ClearData();
	return true;
}

void Bone_Builder::ClearData()
{
	mWidth = 0;
	mHeight = 0;
	mNode = nullptr;
}
