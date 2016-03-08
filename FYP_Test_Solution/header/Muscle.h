#pragma once
class Muscle
{
public:
	Muscle();
	~Muscle();
	int ForceLength(int ContractileLength);
	int ForceVelocity(int ContractileVelocity);
	int ForceContractile();
	int ForceSerial(int value);
	int ForcePassive(int value);

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
	//btGeneric6DofSpringConstraint spring;
};

