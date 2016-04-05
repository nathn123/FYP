#include "..\header\Muscle.h"
#include "btBulletDynamicsCommon.h"
#include <math.h>

#include <math.h>


Muscle::Muscle(btRigidBody* BodyA, btRigidBody* BodyC, btVector3& AttachmentPointA, btVector3& AttachmentPointC,
	btGeneric6DofConstraint* Tendon, btSliderConstraint* muscle, btRigidBody* BodyB)
{
	mParallelLength = abs(BodyB->getCenterOfMassPosition().distance( AttachmentPointC));//muscle/ slider dist 
	mSerialLength = abs(BodyB->getCenterOfMassPosition().distance( AttachmentPointA));//tendon dist

	mLength = mParallelLength + mSerialLength;// total length
	mOptimumLength = mParallelLength;
	mContractileVelocity = 0;
	mForceMax = 22;
	mVelocityMax =  (-12 * mOptimumLength * (-1*mSerialLength))/1000; // bring this to a lower scale
	mForceCont = 0;
	mForceParallel = 0;
	mForceSerial = 0;
	mTotalForce = 0;
	mBodyA = BodyA;
	mBodyB = BodyB;
	mBodyC = BodyC;
	mAttachmentPointA = AttachmentPointA;
	mAttachmentPointC = AttachmentPointC;
	mTendon = Tendon;
	mMuscle = muscle;


}
Muscle::~Muscle()
{

}
float Muscle::ForceLength(float ContractileLength)
{
	auto c = log(0.05f);
	float w = 0.4f * mOptimumLength;
	auto testval = exp(pow(c*(mParallelLength - mOptimumLength) / pow(mOptimumLength, w), 3));
	return testval;
}
float Muscle::ForceVelocity(float ContractileVelocity)
{
	float VCE = 0;
	if (ContractileVelocity < 0)
	{
		VCE = mVelocityMax - ContractileVelocity / mVelocityMax + 5;
	}
	else if (ContractileVelocity >= 0)
	{
		
		VCE = -mVelocityMax + (-mVelocityMax - 1)*mVelocityMax - ContractileVelocity / 7.56f * 5 * ContractileVelocity - mVelocityMax;
	}

	return VCE;
}
float Muscle::ForceContractile()
{
	auto FLEN = ForceLength(mParallelLength);
	auto FVel = ForceVelocity(mContractileVelocity);
	return mActivationState * mForceMax*  FLEN * FVel;
}
float Muscle::ForceSerial()
{
	float tendon_strain = (mSerialLength - mPrevSerialLength) / mSerialLength;
	// non linear spring
	if (tendon_strain <= 0)
		return 0;
	return pow((tendon_strain / (0.04f * mSerialLength)), 2);
}
float Muscle::ForcePassive()
{
	float muscle_strain = (mParallelLength - mPrevParallelLength) / mParallelLength;
	// non linear spring
	if (muscle_strain <= 0)
		return 0;
	return pow((muscle_strain / (0.04f * mParallelLength)), 2);
}
void Muscle::ActivationState(float a)
{
	mActivationState = 0.01f * (a - mActivationState);
	if (mActivationState < 0.f)
		mActivationState = 0.f;
	if (mActivationState > 1.f)
		mActivationState = 1.f;
}
void Muscle::Update(float dt)
{
	// calculate the contractile length
	
	mParallelLength = abs(mBodyC->getCenterOfMassPosition().distance(mBodyB->getCenterOfMassPosition()));
	
	mSerialLength = abs(mBodyA->getCenterOfMassPosition().distance(mBodyB->getCenterOfMassPosition()));
	// calculate the contractile velocity
	//velocity = displacement/time
	//displacement = curr - prev
	mContractileVelocity = (mParallelLength - mPrevParallelLength) / dt; 
	// now we need to apply the force of the muscle into the slider
	// force can only be applied to rigid bodies so we gotta get hacky xD
	auto FCON = ForceContractile();
	auto FSER = ForceSerial();
	auto FPAS = ForcePassive();
	mTotalForce = FCON + FSER + FPAS;
	btVector3 Muscleaxisinworld = mBodyC->getWorldTransform().getBasis() * mMuscle->getFrameOffsetA().getBasis().getColumn(2);
	mBodyC->applyForce(Muscleaxisinworld * mTotalForce, mAttachmentPointC);
	Muscleaxisinworld = mBodyB->getWorldTransform().getBasis() * mMuscle->getFrameOffsetB().getBasis().getColumn(2);
	//force applied to this 
	mBodyB->applyForce(Muscleaxisinworld * mTotalForce, mAttachmentPointA);
	mPrevParallelLength = mParallelLength;
	mPrevSerialLength = mSerialLength;

}