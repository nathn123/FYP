#pragma once
#include "Bone.h"
class Muscle
{
public:
	Muscle(Bone* BoneA, Bone* BoneB, bool Topside);
	~Muscle();
	int ForceLength(int ContractileLength);
	int ForceVelocity(int ContractileVelocity);
	int ForceContractile();
	int ForceSerial(int value);
	int ForcePassive(int value);
	void Constraints();

private:

	int mLength;
	int mParallelLength;
	int mOptimumLength;
	int mSerialLength;
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

