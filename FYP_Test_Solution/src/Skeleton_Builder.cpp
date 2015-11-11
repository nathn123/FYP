#include "..\header\Skeleton_Builder.h"


Skeleton_Builder::Skeleton_Builder()
{
}


Skeleton_Builder::~Skeleton_Builder()
{
}

void Skeleton_Builder::SetDimensions(int NeckIncline, int TailIncline)
{
	mNeckIncline = NeckIncline;
	mTailIncline = TailIncline;
}
void Skeleton_Builder::SetBodyType(LegType leg, ArmType arm, TorsoType torso)
{
	mLeg = leg;
	mArm = arm;
	mTorso = torso;
}
bool Skeleton_Builder::BuildSkeleton(Skeleton* newSkel)
{
	if (mLeg == LegType::None || mTorso == TorsoType::None) // check to ensure all attribs required
		return false;

	// create parts of skeleton separately, then join together
	
	// start with torso

	OgreBulletCollisions::BoxCollisionShape* torsoStart;
	btCollisionShape* test;
	test->


	



}

