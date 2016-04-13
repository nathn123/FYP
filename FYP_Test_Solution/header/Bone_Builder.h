#pragma once
#include <Bullet\btBulletDynamicsCommon.h>
#include <OGRE\Ogre.h>
#include "Bone.h"
class Bone_Builder
{
public:


	Bone_Builder(Ogre::SceneManager* scenemanager, btDynamicsWorld* world);
	~Bone_Builder();
	void SetDimensions(float width, float height, float depth);
	void SetRelativePosition(Ogre::Vector3 pos, Ogre::Quaternion RelRot, Ogre::SceneNode& mNode);
	bool BuildBone(Bone& newBone, Bone::BoneType bone);
	void SetAttachmentPoints(Bone::BoneType bone);
	void ClearData();


private:
	float mWidth, mHeight, mLength;
	Ogre::Vector3 mPosition;
	std::vector<Ogre::Vector3> mAttachments;
	Ogre::Quaternion mRotation;
	Ogre::SceneNode* mNode;
	Ogre::SceneManager* mSceneMgr;
	btDynamicsWorld* mWorld;

};

