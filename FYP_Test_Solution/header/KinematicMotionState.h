#pragma once
//code take from http://www.bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates#Kinematic_Bodies
#include "Ogre.h"
#include "Bullet\LinearMath\btMotionState.h"
class MyKinematicMotionState : public btMotionState {
public:
	MyKinematicMotionState(const btTransform &initialpos) { mPos1 = initialpos; }
	virtual ~MyKinematicMotionState() { }
	virtual void getWorldTransform(btTransform &worldTrans) const { worldTrans = mPos1; }
	void setKinematicPos(btTransform &currentPos) { mPos1 = currentPos; }
	virtual void setWorldTransform(const btTransform &worldTrans) { }

protected:
	btTransform mPos1;
};

