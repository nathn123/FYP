#pragma once
#include "BulletDynamics\ConstraintSolver\btGeneric6DofSpring2Constraint.h"
#include "Bullet\BulletDynamics\ConstraintSolver\btSliderConstraint.h"
#include "BulletDynamics\ConstraintSolver\btFixedConstraint.h"

 class Muscle
{
public:
	BT_DECLARE_ALIGNED_ALLOCATOR();
	Muscle(btRigidBody* BodyA, btRigidBody* BodyC, btVector3& AttachmentPointA, btVector3& AttachmentPointC,
		btFixedConstraint* Tendon, btSliderConstraint* muscle, btRigidBody* BodyB);
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
	btFixedConstraint* mTendon;
	btSliderConstraint* mMuscle;
	btRigidBody* mBodyA, *mBodyB, *mBodyC;
	btVector3 mAttachmentPointA, mAttachmentPointC;
};

