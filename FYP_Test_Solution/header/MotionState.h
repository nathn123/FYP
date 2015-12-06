#pragma once
#include <Bullet\LinearMath\btMotionState.h>
#include<OGRE/Ogre.h>
class MotionState :
	public btMotionState
{
public:
	MotionState(const btTransform &initialPosition,Ogre::SceneNode* node);
	MotionState(Ogre::SceneNode* node);

	virtual ~MotionState();

	void setNode(Ogre::SceneNode* node);
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);
private:
	Ogre::SceneNode* mNode;
	btTransform mInitialPosition;
};

