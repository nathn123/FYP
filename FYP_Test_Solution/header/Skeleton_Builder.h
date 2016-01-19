#pragma once
#include <vector>
#include"..\header\Bone.h"
#include"..\header\Bone_Builder.h"
#include"..\header\Skeleton.h"

class Skeleton_Builder
{
	
public:
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
		None1,
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
	Skeleton_Builder(Ogre::SceneManager* scene, btDynamicsWorld* world);
	~Skeleton_Builder();
	void SetDimensions(int NeckIncline, int TailIncline, int height, int width);
	void SetBodyType(LegType leg, ArmType arm, TorsoType torso);
	bool BuildSkeleton(Skeleton& newSkel, Ogre::Vector3 pos);
	

private: // this is only used by the class
	bool BuildArm(int arm_Width, int arm_height, int torso_width);
	bool BuildNeck(int neck_width, int neck_height);
	bool BuildLeg(int leg_width, int leg_height, int torso_width);
	bool BuildTail(int tail_width, int tail_height);
	bool Upright();
	bool LongNeck();
	bool LongTail();
	bool ShortTail();
	void ClearData();
	void SetJointTransform(btTransform& TransformA, btTransform& TransformB, Bone* boneA, Bone* boneB);


	LegType mLeg;
	ArmType mArm;
	TorsoType mTorso;
	Bone* mShouldernode,*mHipNode;
	float mNeckIncline;
	float mTailIncline;
	float mHeight;
	float mWidth;
	Bone_Builder* mBuilder;
	Ogre::SceneManager* mSceneMgr;
	std::vector<Bone*> mBones;
	std::vector<btTypedConstraint*>mConstraints;
	Ogre::SceneNode* mBase;
	btDynamicsWorld* mWorld;
};

