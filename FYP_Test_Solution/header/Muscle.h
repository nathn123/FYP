#pragma once
#include "Bone.h"
#include "BulletDynamics\ConstraintSolver\btGeneric6DofSpring2Constraint.h"
#include "Bullet\BulletDynamics\ConstraintSolver\btSliderConstraint.h"
class Muscle
{
public:
	Muscle(Bone* BoneA, Bone* BoneB, bool Topside);
	~Muscle();
	int ForceLength(int ContractileLength);
	int ForceVelocity(int ContractileVelocity);
	int ForceContractile();
	int ForceSerial();
	int ForcePassive();
	void Constraints();

private:

	int mLength;
	int mParallelLength;
	float mCurParallelLength;
	int mOptimumLength;
	int mSerialLength;
	float mCurSerialLength;
	float mVelocityMax;
	int mContVel;
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

