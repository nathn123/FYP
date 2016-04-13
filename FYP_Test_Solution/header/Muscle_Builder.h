#pragma once
#include "Muscle.h"
#include "btBulletDynamicsCommon.h"
#include "Utils.h"
#include "Bone.h"
#include "KinematicMotionState.h"
#include "BulletDynamics\ConstraintSolver\btGeneric6DofConstraint.h"
class Muscle_Builder
{
public:
	Muscle_Builder(Ogre::SceneManager* scenemanger, btDynamicsWorld* world);
	~Muscle_Builder();
	bool CreateMuscle(Bone* boneA, Bone* boneB, std::vector<Muscle*>& Muscle_Out);
	void SetMuscleTransform(btTransform& TransformA, btTransform& TransformB, btVector3& JointPosWorld, btTransform& pointA, btTransform& pointB, btVector3& Axis);
	bool FindAttachmentPoints(Bone* boneA, Bone* boneB, std::vector<int>& attachments);
private:
	Ogre::SceneManager* mSceneMgr;
	btDynamicsWorld* mWorld;
	


};