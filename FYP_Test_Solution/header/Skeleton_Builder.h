#pragma once
#include <vector>
#include"..\header\Bone.h"
#include"..\header\Skeleton.h"
class Skeleton_Builder
{
	enum LegType
	{
		None,
		Uninverted,
		Inverted
	};
	enum ArmType
	{
		NoArms,
		LongArms,
		ShortArms
	};
	enum TorsoType
	{
		None,
		LongNeckShortTail,
		LongNeckLongTail,
		LongNeckNoTail,
		ShortNeckShortTail,
		ShortNeckLongTail,
		ShortNeckNoTail,
		LongNeckShortTailUpright,
		LongNeckLongTailUpright,
		LongNeckNoTailUpright,
		ShortNeckShortTailUpright,
		ShortNeckLongTailUpright,
		ShortNeckNoTailUpright,
	};
public:
	Skeleton_Builder();
	~Skeleton_Builder();
	void SetDimensions(int NeckIncline, int TailIncline);
	void SetBodyType(LegType leg, ArmType arm, TorsoType torso);
	bool BuildSkeleton(Skeleton* newSkel);

private:
	LegType mLeg;
	ArmType mArm;
	TorsoType mTorso;
	int mNeckIncline;
	int mTailIncline;
	std::vector<Bone> mBones;
};

