#include "..\header\Muscle.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics\ConstraintSolver\btGeneric6DofSpring2Constraint.h"
#include <math.h>


Muscle::Muscle()
{
	mLength = 0;
	mParallelLength = 0;
	mSerialLength = 0;
	mContVel = 0;
	mForceMax = 0;
	mForceCont = 0;
	mForceParallel = 0;
	mForceSerial = 0;
	mTotalForce = 0;

}


Muscle::~Muscle()
{
}
int Muscle::ForceLength(int ContractileLength)
{
	auto c = log(0.05f);
	auto w = 0.5;
	return exp(pow(c*(mParallelLength - mOptimumLength) / pow(mOptimumLength, w), 3));
}
int Muscle::ForceVelocity(int ContractileVelocity)
{
	int VCE;
	if (ContractileVelocity < 0)
	{
		VCE = mVelocityMax - ContractileVelocity / mVelocityMax + 5;
	}
	else if (ContractileVelocity >= 0)
	{
		
		VCE = -mVelocityMax + (-mVelocityMax - 1)*mVelocityMax - ContractileVelocity / 7.56 * 5 * ContractileVelocity - mVelocityMax;
	}

	return VCE;
}
int Muscle::ForceContractile()
{
	return mActivationState * mForceMax* ForceLength(ContractileVel)*ForceVelocity(ContractileVel);
}
int Muscle::ForceSerial(int value)
{
	// non linear spring
	if (value <= 0)
		return 0;
	return pow((value / refval), 2);
}
int Muscle::ForcePassive(int value)
{
	// non linear spring
	if (value <= 0)
		return 0;
	return pow((value / refval), 2);
}

