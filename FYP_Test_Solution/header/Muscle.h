#pragma once
#include "Bone.h"
#include "BulletDynamics\ConstraintSolver\btGeneric6DofSpring2Constraint.h"
#include "Bullet\BulletDynamics\ConstraintSolver\btSliderConstraint.h"
class Muscle
{
public:
	Muscle(Bone* BoneA, Bone* BoneB, btDynamicsWorld* world);
	~Muscle();
	int ForceLength(int ContractileLength);
	int ForceVelocity(int ContractileVelocity);
	int ForceContractile();
	int ForceSerial();
	int ForcePassive();
	void ActivationState(float a);
	void Update(float dt);

private:

	int mLength;
	float mParallelLength;
	float mPrevParallelLength;
	int mOptimumLength;
	int mSerialLength;
	float mPrevSerialLength;
	float mVelocityMax;
	float mContractileVelocity;
	int mForceMax;
	int mForceCont;
	int mForceParallel;
	int mForceSerial;
	int mTotalForce;
	float mActivationState;
	btGeneric6DofSpring2Constraint* mTendon;
	btSliderConstraint* mMuscle;
	btRigidBody* mBodyA, *mBodyB, *mBodyC;
};

