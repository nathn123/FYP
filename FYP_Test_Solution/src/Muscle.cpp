#include "..\header\Muscle.h"
#include "btBulletDynamicsCommon.h"
#include <math.h>

#include <math.h>


Muscle::Muscle(Bone* BoneA, Bone* BoneB,bool Topside)
{
	
	mContVel = 0;
	mForceMax = 0;
	mForceCont = 0;
	mForceParallel = 0;
	mForceSerial = 0;
	mTotalForce = 0;
	// model the muscle as 3 points 
	// point A and C connected to a bone
	// or should we make A and C the bone 
	// make more sense 
	// 
	// A and B connected by 6DOfConst
	// B and C connected via Slider, to simulate contraction
	btDefaultMotionState* state = new btDefaultMotionState();
	btCollisionShape* shape = new btSphereShape(0.1f);
	mBodyA = BoneA->GetRigidBody();
	mBodyB = new btRigidBody(0, state, shape);
	mBodyC = BoneB->GetRigidBody();
	// what we need now is to compute the tendon dist B-A
	// and the muscle distance B-C
	// should it be computered or predefined
	// not sure where to start for computed soooooooooo
	// if predefined needs to be considerations at too 
	// will the muscle be on the correct side of the leg for the appropriate motion
	auto identity = new btMatrix3x3();
	btVector3* transA;
	btVector3* transB;
	btVector3* transB2;
	btVector3* transC;
	
	if (Topside)
	{
		transA = new btVector3(-0.1f, 0.1f, 0);
		transB = new btVector3(0.1f, -0.1f, 0);
		transB2 = new btVector3(0.5f, -0.1f, 0);
		transC = new btVector3(-0.5f, 0.1f, 0);
	}
	else
	{
		transA = new btVector3(-0.1f, -0.1f, 0);
		transB = new btVector3(0.1f, 0.1f, 0);
		transB2 = new btVector3(0.5f, 0.1f, 0);
		transC = new btVector3(-0.5f, -0.1f, 0);
	}
	btTransform* transformA = new btTransform(identity->getIdentity(), *transA);
	btTransform* transformB = new btTransform(identity->getIdentity(), *transB);
	btTransform* transformC = new btTransform(identity->getIdentity(), *transC);
	btTransform* transformB2 = new btTransform(identity->getIdentity(), *transB2);
	mTendon = new btGeneric6DofSpring2Constraint(*mBodyA, *mBodyB, *transformA, *transformB);
	mMuscle = new btSliderConstraint(*mBodyC, *mBodyB, *transformC, *transformB2, true);
	
	

	
	mParallelLength = abs(mBodyC->getCenterOfMassPosition().distance(mBodyB->getCenterOfMassPosition()));;//muscle/ slider dist 
	mSerialLength = abs(mBodyA->getCenterOfMassPosition().distance(mBodyB->getCenterOfMassPosition()));;//tendon dist
	mLength = mParallelLength + mSerialLength;// total length
}


Muscle::~Muscle()
{
	


}
int Muscle::ForceLength(int ContractileLength)
{
	auto c = log(0.05f);
	auto w = 0.4 * mOptimumLength;
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
	return mActivationState * mForceMax* ForceLength(mParallelLength)*ForceVelocity(mContVel);
}
int Muscle::ForceSerial()
{
	float tendon_strain = (mCurSerialLength - mSerialLength) / mSerialLength;
	// non linear spring
	if (tendon_strain <= 0)
		return 0;
	return pow((tendon_strain / (0.04 * mSerialLength)), 2);
}
int Muscle::ForcePassive()
{
	int muscle_strain = (mCurParallelLength - mParallelLength) / mParallelLength;
	// non linear spring
	if (muscle_strain <= 0)
		return 0;
	return pow((muscle_strain / (0.04 * mParallelLength)), 2);
}
void Muscle::Constraints()
{

}

