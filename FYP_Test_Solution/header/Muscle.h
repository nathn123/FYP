#pragma once
#include "Bone.h"
#include "BulletDynamics\ConstraintSolver\btGeneric6DofSpring2Constraint.h"
#include "Bullet\BulletDynamics\ConstraintSolver\btSliderConstraint.h"
class Muscle
{
public:
	Muscle(Bone* BoneA, Bone* BoneB, btDynamicsWorld* world);
	~Muscle();
	float ForceLength(float ContractileLength);
	float ForceVelocity(float ContractileVelocity);
	float ForceContractile();
	float ForceSerial();
	float ForcePassive();
	void ActivationState(float a);
	void Update(float dt);

private:

	float mLength;
	float mParallelLength;
	float mPrevParallelLength;
	float mOptimumLength;
	float mSerialLength;
	float mPrevSerialLength;
	float mVelocityMax;
	float mContractileVelocity;
	float mForceMax;
	float mForceCont;
	float mForceParallel;
	float mForceSerial;
	float mTotalForce;
	float mActivationState;
	btGeneric6DofSpring2Constraint* mTendon;
	btSliderConstraint* mMuscle;
	btRigidBody* mBodyA, *mBodyB, *mBodyC;
};

