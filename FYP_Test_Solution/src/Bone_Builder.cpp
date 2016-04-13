#include "..\header\Bone_Builder.h"
#include "Bone.h"



Bone_Builder::Bone_Builder(Ogre::SceneManager* scenemanger, btDynamicsWorld* world)
{
	mSceneMgr = scenemanger;
	mWorld = world;
}
Bone_Builder::~Bone_Builder()
{
}
void Bone_Builder::SetDimensions(float width, float height, float depth)
{
	mWidth = width;
	mHeight = height;
	mLength = depth;
}
void Bone_Builder::SetRelativePosition(Ogre::Vector3 relpos, Ogre::Quaternion RelRot, Ogre::SceneNode& parent)
{
	mNode = &parent;
	mPosition = relpos;
	mRotation = RelRot;
}
bool Bone_Builder::BuildBone(Bone& newBone, Bone::BoneType bone)
{
	SetAttachmentPoints(bone);
	if (mWidth <= 0 || mHeight <= 0 || mAttachments.empty() || mPosition.isNaN() || mNode == nullptr) // ensure it has been initialised
		return false;
	
	mWidth /= 2;
	mHeight /= 2;
	mLength /= 2;
	Ogre::ManualObject* bonecube = mSceneMgr->createManualObject();
	bonecube->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

	bonecube->position(-mWidth, -mHeight, mLength);
	bonecube->position(mWidth, -mHeight, mLength);
	bonecube->position(-mWidth, mHeight, mLength);
	bonecube->position(mWidth, mHeight, mLength);
	bonecube->position(-mWidth, -mHeight, -mLength);
	bonecube->position(mWidth, -mHeight, -mLength);
	bonecube->position(-mWidth, mHeight, -mLength);
	bonecube->position(mWidth, mHeight, -mLength);




	bonecube->index(0);
	bonecube->index(1);
	bonecube->index(2);
	bonecube->index(3);
	bonecube->index(7);
	bonecube->index(1);
	bonecube->index(5);
	bonecube->index(4);
	bonecube->index(7);
	bonecube->index(6);
	bonecube->index(2);
	bonecube->index(4);
	bonecube->index(0);
	bonecube->index(1);



	bonecube->end();
	newBone = Bone(bonecube,bone,mPosition,mRotation,mAttachments,mNode,mWorld);
	this->ClearData();
	return true;
}
void Bone_Builder::SetAttachmentPoints(Bone::BoneType bone)
{
	// create muscle attachment points on opposite sides
	// in pairs [0] = left [1] = right
	if (bone == Bone::BoneType::Neck)
	{
		mAttachments.push_back(Ogre::Vector3(mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::Tail)
	{

	}
	else if (bone == Bone::BoneType::Foot)
	{
		mAttachments.push_back(Ogre::Vector3(0,mHeight/2 ,0 ));
		mAttachments.push_back(Ogre::Vector3(0,-mHeight/2 ,0));
	}
	else if (bone == Bone::BoneType::LowerArm)
	{
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::LowerLeg)
	{
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::Torso)
	{
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::LUpperArm)
	{
		mAttachments.push_back(Ogre::Vector3(mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::RUpperArm)
	{
		mAttachments.push_back(Ogre::Vector3(mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::LUpperLeg)
	{
		mAttachments.push_back(Ogre::Vector3(mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::RUpperLeg)
	{
		mAttachments.push_back(Ogre::Vector3(mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
	}
	else if (bone == Bone::BoneType::Hip)
	{
		// standard torso front back
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
		// left side front and back
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, -mLength / 2));
		// right side front and back
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, -mLength / 2));
		//left side left and right
		mAttachments.push_back(Ogre::Vector3(mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(0, mHeight/2, 0));
		//right side left and right
		mAttachments.push_back(Ogre::Vector3(0, mHeight / 2, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 2, 0, 0));

		//tail attachment
		mAttachments.push_back(Ogre::Vector3(0,0,mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, mHeight / 2, 0));
	}
	else if (bone == Bone::BoneType::Shoulder)
	{
		// standard torso front back
		mAttachments.push_back(Ogre::Vector3(0, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
		// left side front and back
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, -mLength / 2));
		// right side front and back
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, -mLength / 2));
		//left side up and bot
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, mHeight / 2, 0));
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, -mHeight / 2, 0));
		//right side up and bot
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, mHeight / 2, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, -mHeight / 2, 0));

		// neck
		// use standard torso  0 and 1
		// left and right neck 6 and 8

	}
	else if (bone == Bone::BoneType::HipShoulder)
	{
		//LEGS
		// left side front and back
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, -mLength / 2));
		// right side front and back
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, -mLength / 2));
		//left side left and right
		mAttachments.push_back(Ogre::Vector3(mWidth / 2, 0, 0));
		mAttachments.push_back(Ogre::Vector3(0, mHeight / 2, 0));
		//right side left and right
		mAttachments.push_back(Ogre::Vector3(0, mHeight / 2, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 2, 0, 0));
		//ARMS
		// left side front and back
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, 0, -mLength / 2));
		// right side front and back
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, mLength / 2));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, 0, -mLength / 2));
		//left side up and bot
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, mHeight / 2, 0));
		mAttachments.push_back(Ogre::Vector3(mWidth / 4, -mHeight / 2, 0));
		//right side up and bot
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, mHeight / 2, 0));
		mAttachments.push_back(Ogre::Vector3(-mWidth / 4, -mHeight / 2, 0));

		// neck
		// use standard torso  0 and 1
		// left and right neck 6 and 8

		//tail attachment
		mAttachments.push_back(Ogre::Vector3(0, 0, -mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, -mHeight / 2, 0));

		mAttachments.push_back(Ogre::Vector3(0, mHeight / 2, -mLength / 2));
		mAttachments.push_back(Ogre::Vector3(0, -mHeight / 2, -mLength / 2));
	}

}
void Bone_Builder::ClearData()
{
	mWidth = 0;
	mHeight = 0;
	mNode = nullptr;
	mAttachments.clear();
}