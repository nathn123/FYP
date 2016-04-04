#pragma once
#include <vector>
#include"..\header\Bone_Builder.h"
#include"..\header\Skeleton.h"
#include"Muscle_Builder.h"

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
	void SetDimensions(float NeckIncline, float TailIncline, float height, float width);
	void SetBodyType(LegType leg, ArmType arm, TorsoType torso);
	bool BuildSkeleton(Skeleton& newSkel, Ogre::Vector3 pos);


	static void SetJointTransform(btTransform& TransformA, btTransform& TransformB, btVector3& JointPosWorld, Ogre::Vector3 boneA, Ogre::Vector3 boneB, btVector3& Axis);


private: // this is only used by the class
	bool BuildArm(float arm_Width, float arm_height, float torso_width);
	bool BuildNeck(float neck_width, float neck_height, float torso_height);
	bool BuildLeg(float leg_width, float leg_height, float torso_width, float torso_height);
	bool BuildTail(float tail_width, float tail_height);
	bool Upright();
	bool LongNeck();
	bool LongTail();
	bool ShortTail();
	void ClearData();


	LegType mLeg;
	ArmType mArm;
	TorsoType mTorso;
	Bone* mShouldernode,*mHipNode;
	float mNeckIncline;
	float mTailIncline;
	float mHeight;
	float mWidth;
	Bone_Builder* mBuilder;
	Muscle_Builder* mMuscleBuilder;
	Ogre::SceneManager* mSceneMgr;
	std::vector<Bone*> mBones;
	std::vector<Muscle*>mMuscles;
	std::vector<btTypedConstraint*>mConstraints;
	Ogre::SceneNode* mBase;
	btDynamicsWorld* mWorld;
};

