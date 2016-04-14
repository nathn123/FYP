#include "..\header\Skeleton_Builder.h"
#include "Utils.h"
#include "Bone_Builder.h"
#define _USE_MATH_DEFINES
#include <math.h>

Skeleton_Builder::Skeleton_Builder(Ogre::SceneManager* scene,btDynamicsWorld* world)
{
	mSceneMgr = scene;
	mWorld = world;
	mBuilder = new Bone_Builder(mSceneMgr, mWorld);
	mMuscleBuilder = new Muscle_Builder(mSceneMgr, mWorld);
}
Skeleton_Builder::~Skeleton_Builder()
{
	
}
void Skeleton_Builder::SetHidden(bool arm, bool leg, bool neck)
{
	mHidearms = arm;
	mHidelegs = leg;
	mHideneck = neck;
}
void Skeleton_Builder::SetDimensions(float NeckIncline, float TailIncline, float height, float width,float depth)
{
	mNeckIncline = NeckIncline;
	mTailIncline = TailIncline;
	mDepth = depth;
	mHeight = height;
	mWidth = width;
}
void Skeleton_Builder::SetBodyType(LegType leg, ArmType arm, TorsoType torso, NeckType neck, TailType tail, bool hasmuscle, bool Fixed)
{
	mHasMuscle = hasmuscle;
	mFixed = Fixed;
	mLeg = leg;
	mArm = arm;
	mTorso = torso;
	mNeck = neck;
	mTail = tail;
}
bool Skeleton_Builder::BuildSkeleton(Skeleton& newSkel, Ogre::Vector3 pos)
{
	if (mLeg == LegType::None || mHeight <= 10) // check to ensure all attribs required
		return false;
	
	float neck_height;
	float torso_height;
	float Leg_height;
	float tail_height;
	//create base node for skeleton
	mBase = mSceneMgr->getRootSceneNode()->createChildSceneNode(pos);
	// set block area heights
	// if longneck  neck/legs ratio = 2:1
	// if shortneck neck/legs ratio = 1:2
	// if upright torso 40% 
	// else torso 25% 
	
	
	if (IsUpright())
	{
		auto onepercent = mHeight / 100;
		Leg_height = onepercent * 40;
		if (IsLongNeck())
		{
			neck_height = onepercent * 40;
			torso_height = onepercent * 20;
		}
		else
		{
			neck_height = onepercent * 20;
			torso_height = onepercent * 40;
		}
	}
	else
	{
		auto onepercent = mHeight / 100;
		torso_height = onepercent * 25;
		if (IsLongNeck())
		{
			neck_height = onepercent * 50;
			Leg_height = onepercent * 25;
		}
		else
		{
			neck_height = onepercent * 25;
			Leg_height = onepercent * 50;
		}
	}
	if (IsLongTail())
	{
		auto onepercent = mHeight / 100;
		tail_height = onepercent * 50;
	}
	else
	{
		auto onepercent = mHeight / 100;
		tail_height = onepercent * 25;
	}

	//set block area widths
	float neck_width;
	float torso_width;
	float Leg_width;
	float Arm_width;

	auto onepercentW = mWidth / 100;
	// arms take 35% of width i.e 17.5% each side if needed
	torso_width = onepercentW * 65;
	Leg_width = onepercentW * 25; // ensures gaps between legs like realistic bipeds
	Arm_width = onepercentW * 17.5f;
	neck_width = onepercentW * 35;

	//build the torso here
	Bone* CentreTorso = new Bone();
	if (IsUpright())
	{
		
		Bone* TopTorso = new Bone();
		Bone* BotTorso = new Bone();
		//build 3 torso pieces
		torso_height /= 3;
		//build centre piece
		mBuilder->SetDimensions(torso_width, torso_height, mDepth);
		mBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY,*mBase);
		if (!mBuilder->BuildBone(*CentreTorso,Bone::BoneType::Torso))
			return false;
		//build bot piece
		mBuilder->SetDimensions(torso_width, torso_height, mDepth);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -(torso_height), 0), Ogre::Quaternion::IDENTITY, *CentreTorso->GetNode());
		if (!mBuilder->BuildBone(*BotTorso,Bone::BoneType::Hip))
			return false;
		//build top piece
		mBuilder->SetDimensions(torso_width, torso_height, mDepth);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, (torso_height), 0), Ogre::Quaternion::IDENTITY, *CentreTorso->GetNode());
		if (!mBuilder->BuildBone(*TopTorso, Bone::BoneType::Shoulder))
			return false;
		mShouldernode = TopTorso;
		mHipNode = BotTorso;

		mBones.push_back(CentreTorso);
		mBones.push_back(TopTorso);
		mBones.push_back(BotTorso);
		//add muscles
		mMuscleBuilder->CreateMuscle(CentreTorso, TopTorso, mMuscles);
		mMuscleBuilder->CreateMuscle(CentreTorso, BotTorso, mMuscles);

		btTransform TransA;
		btTransform TransB;
		btVector3 JointPos; // world position of transform
		/*add constraints*/
		// JOINT POS IS THE POSITION ON THE EDGE OF THE BONE ATTACH I.E BONE POS + EDGE OF BONE IN DIRECTION TO NEXT
		JointPos = Utils::OgreBTVector(CentreTorso->GetNode()->_getDerivedPosition() + (torso_height / 2));

		SetJointTransform(TransA, TransB, JointPos, CentreTorso, TopTorso, btVector3(0, btScalar(-M_PI_2), 0));
		btConeTwistConstraint* TopSpine = new btConeTwistConstraint(*CentreTorso->GetRigidBody(), *TopTorso->GetRigidBody(), TransA, TransB);
		TopSpine->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

		JointPos = Utils::OgreBTVector(CentreTorso->GetNode()->_getDerivedPosition() - (torso_height / 2));

		SetJointTransform(TransA, TransB, JointPos, CentreTorso, BotTorso, btVector3(0, btScalar(-M_PI_2), 0));
		btConeTwistConstraint* BotSpine = new btConeTwistConstraint(*CentreTorso->GetRigidBody(), *BotTorso->GetRigidBody(), TransA, TransB);
		BotSpine->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

		mConstraints.push_back(TopSpine);
		mConstraints.push_back(BotSpine);
		
	}
	else
	{
		//Bone* CentreTorso = new Bone();

		//build centre piece
		mBuilder->SetDimensions(torso_width, torso_height, mDepth);
		mBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, *mBase);
		if (!mBuilder->BuildBone(*CentreTorso,Bone::BoneType::HipShoulder))
			return false;
		mBones.push_back(CentreTorso);
		
		mShouldernode = CentreTorso;
		mHipNode = CentreTorso;
	}
	if (mFixed)
		CentreTorso->GetRigidBody()->setMassProps(0, btVector3(0, 0, 0));

	//if (!BuildArm(Arm_width, Leg_height, torso_width) || !BuildNeck(neck_width, neck_height, torso_height) || !BuildLeg(Leg_width, Leg_height, torso_width, torso_height))
	//	return false;
	if (!mHidearms)
		if (!BuildArm(Arm_width, torso_height, torso_width))
			return false;
	if (!mHideneck)
		if (!BuildNeck(neck_width, neck_height, torso_height))
			return false;
	if (!mHidelegs)
		if (!BuildLeg(Leg_width, Leg_height, torso_width, torso_height))
		return false;

	if (IsShortTail() || IsLongTail())
		if(!BuildTail(neck_width,tail_height, torso_height))
			return false;

		newSkel = Skeleton(mBones,mMuscles, mConstraints, mBase, mWorld);

		ClearData();
		return true;
}
bool Skeleton_Builder::IsUpright()
{
	if (mTorso == TorsoType::Upright)
		return true;
	return false;
}
bool Skeleton_Builder::IsLongNeck()
{
	if (mTorso == NeckType::LongNeck)
		return true;
	return false;
}
bool Skeleton_Builder::IsLongTail()
{
	if (mTail == TailType::LongTail)
		return true;
	return false;
}
bool Skeleton_Builder::IsShortTail()
{
	if (mTail == TailType::ShortTail)
		return true;
	return false;
}
void Skeleton_Builder::ClearData()
{
	mLeg = LegType::None;
	mHeight = -1;
	mBones.clear();
	mConstraints.clear();
}
bool Skeleton_Builder::BuildArm(float arm_Width, float arm_height, float torso_width)
{
	// 3 blocks no hands
	// only height/ width changes 
	//if (mArm == ArmType::ShortArms)
	//{
	//	//half height of legs
	//	Bone* LUpperArm = new Bone();
	//	Bone* LLowerArm = new Bone();
	//	//build shoulder piece
	//	torso_width /= 2;
	//	mBuilder->SetDimensions(arm_Width, arm_height/2);
	//	mBuilder->SetRelativePosition(Ogre::Vector3(torso_width, 0, 0), Ogre::Quaternion::IDENTITY, *mShouldernode->GetNode());
	//	if (!mBuilder->BuildBone(*LUpperArm))
	//		return false;
	//	

	//	mBuilder->SetDimensions(arm_Width, arm_height / 2);
	//	mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height / 2, 0), Ogre::Quaternion::IDENTITY, *LUpperArm->GetNode());
	//	if (!mBuilder->BuildBone(*LLowerArm))
	//		return false;

	//	mBones.push_back(LUpperArm);
	//	mBones.push_back(LLowerArm);

	//	//Bone* RUpperArm = new Bone();
	//	//Bone* RLowerArm = new Bone();

	//	//mBuilder->SetDimensions(arm_Width, arm_height / 2);
	//	//mBuilder->SetRelativePosition(Ogre::Vector3(-torso_width, 0, 0), Ogre::Quaternion::IDENTITY, *mShouldernode->GetNode());
	//	//if (!mBuilder->BuildBone(*RUpperArm))
	//	//	return false;
	//	//
	//	//mBuilder->SetDimensions(arm_Width, arm_height / 2);
	//	//mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height / 2, 0), Ogre::Quaternion::IDENTITY, *RUpperArm->GetNode());
	//	//if (!mBuilder->BuildBone(*RLowerArm))
	//	//	return false;

	//	//mBones.push_back(RUpperArm);
	//	//mBones.push_back(RLowerArm);
	//	//add joints
	//	btTransform localA, localB;
	//	Ogre::Vector3 offset;
	//	offset = Ogre::Vector3(torso_width, 0, 0);
	//	//auto vec = mShouldernode->GetNode()->_getDerivedPosition();
	//	//vec += offset;

	//	SetJointTransform(localA, localB, Utils::OgreBTVector(mShouldernode->GetNode()->_getDerivedPosition() + offset), mShouldernode, LUpperArm);
	//	btConeTwistConstraint* LshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(), *LUpperArm->GetRigidBody(), localA, localB);
	//	LshoulderConst->setLimit(M_PI_2, M_PI_2, 0);
	//	//SetJointTransform(localA, localB, Utils::OgreBTVector(mShouldernode->GetNode()->_getDerivedPosition() - offset), mShouldernode, RUpperArm);
	//	//btConeTwistConstraint* RshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(), *RUpperArm->GetRigidBody(), localA, localB);
	//	//RshoulderConst->setLimit(M_PI_2, M_PI_2, 0);

	//	offset = Ogre::Vector3(0, arm_height/4, 0);

	//	SetJointTransform(localA, localB, Utils::OgreBTVector(LUpperArm->GetNode()->_getDerivedPosition() - offset), LUpperArm, LLowerArm);
	//	btHingeConstraint* LArmConst = new btHingeConstraint(*LUpperArm->GetRigidBody(), *LLowerArm->GetRigidBody(), localA, localB);

	//	//SetJointTransform(localA, localB, Utils::OgreBTVector(RUpperArm->GetNode()->_getDerivedPosition() - offset), RUpperArm, RLowerArm);
	//	//btHingeConstraint* RArmConst = new btHingeConstraint(*RUpperArm->GetRigidBody(), *RLowerArm->GetRigidBody(), localA, localB);

	//	mConstraints.push_back(LshoulderConst);
	//	mConstraints.push_back(LArmConst);
	//	//mConstraints.push_back(RshoulderConst);
	//	//mConstraints.push_back(RArmConst);
	//	
	//}
	//else if (mArm == ArmType::LongArms)
	{
		//half height of legs
		Bone* LUpperArm = new Bone();
		Bone* RUpperArm = new Bone();
		Bone* LLowerArm = new Bone();
		Bone* RLowerArm = new Bone();

		//build shoulder piece
		//torso_width /= 2;
		mBuilder->SetDimensions(arm_Width, arm_height, mDepth * 0.5f);
		mBuilder->SetRelativePosition(Ogre::Vector3(((torso_width / 2) + arm_Width/2), 0, 0), Ogre::Quaternion::IDENTITY, *mShouldernode->GetNode());
		if (!mBuilder->BuildBone(*LUpperArm,Bone::BoneType::LUpperArm))
			return false;
		mBuilder->SetDimensions(arm_Width, arm_height, mDepth * 0.5f);
		mBuilder->SetRelativePosition(Ogre::Vector3(-((torso_width / 2) + arm_Width/2), 0, 0), Ogre::Quaternion::IDENTITY, *mShouldernode->GetNode());
		if (!mBuilder->BuildBone(*RUpperArm, Bone::BoneType::RUpperArm))
			return false;
		mBuilder->SetDimensions(arm_Width, arm_height, mDepth * 0.5f);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -(arm_height), 0), Ogre::Quaternion::IDENTITY, *LUpperArm->GetNode());
		if (!mBuilder->BuildBone(*LLowerArm, Bone::BoneType::LowerArm))
			return false;
		mBuilder->SetDimensions(arm_Width, arm_height, mDepth * 0.5f);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -(arm_height), 0), Ogre::Quaternion::IDENTITY, *RUpperArm->GetNode());
		if (!mBuilder->BuildBone(*RLowerArm, Bone::BoneType::LowerArm))
			return false;
		mBones.push_back(LUpperArm);
		mBones.push_back(RUpperArm);
		mBones.push_back(LLowerArm);
		mBones.push_back(RLowerArm);
		if (mHasMuscle)
		{
			mMuscleBuilder->CreateMuscle(mShouldernode, LUpperArm, mMuscles);
			mMuscleBuilder->CreateMuscle(LUpperArm, LLowerArm, mMuscles);
			mMuscleBuilder->CreateMuscle(mShouldernode, RUpperArm, mMuscles);
			mMuscleBuilder->CreateMuscle(RUpperArm, RLowerArm, mMuscles);
		}
		//add joints
		btTransform localA, localB;
		btVector3 JointPos;

		JointPos = Utils::OgreBTVector(mShouldernode->GetNode()->_getDerivedPosition() + (torso_width / 2 + arm_Width * 0.10f));
		SetJointTransform(localA, localB, JointPos, mShouldernode, LUpperArm, btVector3(0, 0, 0));
		// this needed a special case
		localA.getBasis().setEulerZYX(0,0,btScalar(M_PI));
		localB.getBasis().setEulerZYX(0, 0, btScalar(M_PI_2));
		btConeTwistConstraint* LshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(), *LUpperArm->GetRigidBody(), localA, localB);
		LshoulderConst->setLimit(btScalar(M_PI_2), btScalar(M_PI_2), 0);
		

		JointPos = Utils::OgreBTVector(mShouldernode->GetNode()->_getDerivedPosition() - (torso_width / 2 + arm_Width * 0.10f));
		SetJointTransform(localA, localB, JointPos, mShouldernode, RUpperArm, btVector3(0, 0, 0));
		localA.getBasis().setEulerZYX(0, 0, 0);
		localB.getBasis().setEulerZYX(0, 0, btScalar(M_PI_2));
		btConeTwistConstraint* RshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(), *RUpperArm->GetRigidBody(), localA, localB);
		RshoulderConst->setLimit(btScalar(M_PI_2), btScalar(M_PI_2), 0);
		

		JointPos = Utils::OgreBTVector(LUpperArm->GetNode()->_getDerivedPosition() - arm_height / 2);
		SetJointTransform(localA, localB, JointPos, LUpperArm, LLowerArm, btVector3(0, btScalar(M_PI_2), 0));
		btHingeConstraint* LArmConst = new btHingeConstraint(*LUpperArm->GetRigidBody(), *LLowerArm->GetRigidBody(), localA, localB);
		LArmConst->setLimit(0, btScalar(M_PI_2));

		JointPos = Utils::OgreBTVector(RUpperArm->GetNode()->_getDerivedPosition() - arm_height / 2);
		SetJointTransform(localA, localB, JointPos, RUpperArm, RLowerArm, btVector3(0, btScalar(M_PI_2), 0));
		btHingeConstraint* RArmConst = new btHingeConstraint(*RUpperArm->GetRigidBody(), *RLowerArm->GetRigidBody(), localA, localB);
		RArmConst->setLimit(0, btScalar(M_PI_2));

		mConstraints.push_back(LshoulderConst);
		mConstraints.push_back(RshoulderConst);
		mConstraints.push_back(LArmConst);
		mConstraints.push_back(RArmConst);

		//auto prestore = mMuscleBuilder->CreateMuscle(LUpperArm, LLowerArm);
		//for (auto m : prestore)
		//	mMuscles.push_back(m);
	}
	// means no arms to continue

	return true;
}
bool Skeleton_Builder::BuildNeck(float neck_width, float neck_height, float torso_height)
{
	Bone* UNeck = new Bone();
	Bone* MNeck = new Bone();
	Bone* LNeck = new Bone();
	Bone* LLNeck = new Bone();
	neck_height /= 4;
	mBuilder->SetDimensions(neck_width, neck_height, mDepth * 0.5f);
	Ogre::Quaternion neckrotation;
	neckrotation.FromAngleAxis(Ogre::Radian(Ogre::Degree(mNeckIncline)), Ogre::Vector3::UNIT_X);

	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height, 0), neckrotation, *mShouldernode->GetNode());
	if (!mBuilder->BuildBone(*LLNeck,Bone::BoneType::Neck))
		return false;
	mBuilder->SetDimensions(neck_width, neck_height, mDepth * 0.5f);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height, 0), neckrotation, *LLNeck->GetNode());
	if (!mBuilder->BuildBone(*LNeck, Bone::BoneType::Neck))
		return false;
	mBuilder->SetDimensions(neck_width, neck_height, mDepth * 0.5f);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height, 0), neckrotation, *LNeck->GetNode());
	if (!mBuilder->BuildBone(*MNeck, Bone::BoneType::Neck))
		return false;
	mBuilder->SetDimensions(neck_width, neck_height, mDepth * 0.5f);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height, 0), neckrotation, *MNeck->GetNode());
	if (!mBuilder->BuildBone(*UNeck, Bone::BoneType::Neck))
		return false;
	if (mHasMuscle)
	{
		mMuscleBuilder->CreateMuscle(mShouldernode, LLNeck, mMuscles);
		mMuscleBuilder->CreateMuscle(LLNeck, LNeck, mMuscles);
		mMuscleBuilder->CreateMuscle(LNeck, MNeck, mMuscles);
		mMuscleBuilder->CreateMuscle(MNeck, UNeck, mMuscles);
	}
	
	//add joints
	btTransform localA, localB;
	btVector3 JointPos;

	JointPos = Utils::OgreBTVector(mShouldernode->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (torso_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, mShouldernode, LLNeck, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* shoulderNeckConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(), *LLNeck->GetRigidBody(), localA, localB);
	shoulderNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

	JointPos = Utils::OgreBTVector(LLNeck->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (neck_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, LLNeck, LNeck, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* LNeckConst = new btConeTwistConstraint(*LLNeck->GetRigidBody(), *LNeck->GetRigidBody(), localA, localB);
	LNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

	JointPos = Utils::OgreBTVector(mHipNode->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (neck_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, LNeck, MNeck, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* MNeckConst = new btConeTwistConstraint(*LNeck->GetRigidBody(), *MNeck->GetRigidBody(), localA, localB);
	MNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

	JointPos = Utils::OgreBTVector(mHipNode->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (neck_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, MNeck, UNeck, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* HNeckConst = new btConeTwistConstraint(*MNeck->GetRigidBody(), *UNeck->GetRigidBody(), localA, localB);
	HNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4),btScalar(M_PI_4 / 4));

	mConstraints.push_back(shoulderNeckConst);
	mConstraints.push_back(LNeckConst);
	mConstraints.push_back(MNeckConst);
	mConstraints.push_back(HNeckConst);

	return true;
}
bool Skeleton_Builder::BuildLeg(float leg_width, float leg_height, float torso_width, float torso_height)
{
	auto halfgap = (torso_width / 2) - leg_width;
	auto legnodepos = halfgap + leg_width / 2;

	Bone* LUpperLeg = new Bone();
	Bone* LLowerLeg = new Bone();
	Bone* LFoot = new Bone();
	Bone* RUpperLeg = new Bone();
	Bone* RLowerLeg = new Bone();
	Bone* RFoot = new Bone();
	if (mLeg == LegType::Uninverted)
	{
		mBuilder->SetDimensions(leg_width, leg_height / 2, mDepth *0.75f);
		mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, -((torso_height / 2) + (leg_height / 4)), 0), Ogre::Quaternion::IDENTITY, *mHipNode->GetNode());
		if (!mBuilder->BuildBone(*LUpperLeg,Bone::BoneType::LUpperLeg))
			return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2, mDepth *0.75f);
		mBuilder->SetRelativePosition(Ogre::Vector3(-legnodepos, -((torso_height / 2) + (leg_height / 4)), 0), Ogre::Quaternion::IDENTITY, *mHipNode->GetNode());
		if (!mBuilder->BuildBone(*RUpperLeg, Bone::BoneType::RUpperLeg))
			return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2, mDepth *0.75f);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -leg_height / 2, 0), Ogre::Quaternion::IDENTITY, *LUpperLeg->GetNode());
		if (!mBuilder->BuildBone(*LLowerLeg, Bone::BoneType::LowerLeg))
			return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2, mDepth *0.75f);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -leg_height / 2, 0), Ogre::Quaternion::IDENTITY, *RUpperLeg->GetNode());
		if (!mBuilder->BuildBone(*RLowerLeg, Bone::BoneType::LowerLeg))
			return false;

		// foot generation
		mBuilder->SetDimensions(leg_width*1.5f, leg_height / 4, mDepth);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -leg_height / 2, 0), Ogre::Quaternion::IDENTITY, *LLowerLeg->GetNode());
		if (!mBuilder->BuildBone(*LFoot,Bone::BoneType::Foot))
			return false;
		mBuilder->SetDimensions(leg_width*1.5f, leg_height / 4, mDepth);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -leg_height / 2, 0), Ogre::Quaternion::IDENTITY, *RLowerLeg->GetNode());
		if (!mBuilder->BuildBone(*RFoot, Bone::BoneType::Foot))
			return false;

	}
	else
	{
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::IDENTITY, *mHipNode->GetNode());
		//if (!mBuilder->BuildBone(*LUpperLeg))
		//	return false;
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(0, leg_height / 2, 0), Ogre::Quaternion::IDENTITY, *LUpperLeg->GetNode());
		//if (!mBuilder->BuildBone(*LLowerLeg))
		//	return false;
		// foot generation
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::IDENTITY, Hip->GetNode());
		//if (!mBuilder->BuildBone(LUpperLeg))
		//	return false;
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(-legnodepos, 0, 0), Ogre::Quaternion::IDENTITY, *mHipNode->GetNode());
		//if (!mBuilder->BuildBone(*RUpperLeg))
		//	return false;
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(0, leg_height / 2, 0), Ogre::Quaternion::IDENTITY, *RUpperLeg->GetNode());
		//if (!mBuilder->BuildBone(*RLowerLeg))
		//	return false;
		// foot generation
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::IDENTITY, Hip->GetNode());
		//if (!mBuilder->BuildBone(LUpperLeg))
		//	return false;
	}
	mBones.push_back(LUpperLeg);
	mBones.push_back(RUpperLeg);
	mBones.push_back(LLowerLeg);
	mBones.push_back(RLowerLeg);
	mBones.push_back(LFoot);
	mBones.push_back(RFoot);
	if (mHasMuscle)
	{
		mMuscleBuilder->CreateMuscle(mHipNode, LUpperLeg, mMuscles);
		mMuscleBuilder->CreateMuscle(LUpperLeg, LLowerLeg, mMuscles);
		mMuscleBuilder->CreateMuscle(LLowerLeg, LFoot, mMuscles);

		mMuscleBuilder->CreateMuscle(mHipNode, RUpperLeg, mMuscles);
		mMuscleBuilder->CreateMuscle(RUpperLeg, RLowerLeg, mMuscles);
		mMuscleBuilder->CreateMuscle(RLowerLeg, RFoot, mMuscles);
	}
	//add joints
	Ogre::Vector3 offset;
	btTransform localA, localB;
	btVector3 JointPos;

	JointPos = Utils::OgreBTVector(mHipNode->GetNode()->_getDerivedPosition() + Ogre::Vector3(legnodepos, -(torso_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, mHipNode, LUpperLeg, btVector3(0, btScalar(M_PI_2), 0));
	btConeTwistConstraint* LhipConst = new btConeTwistConstraint(*mHipNode->GetRigidBody(), *LUpperLeg->GetRigidBody(), localA, localB);
	LhipConst->setLimit(btScalar(M_PI_4), btScalar(M_PI_4), 0);

	JointPos = Utils::OgreBTVector(mHipNode->GetNode()->_getDerivedPosition() + Ogre::Vector3(-legnodepos, -(torso_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, mHipNode, RUpperLeg, btVector3(0, 0, btScalar(M_PI_4)));
	btConeTwistConstraint* RhipConst = new btConeTwistConstraint(*mHipNode->GetRigidBody(), *RUpperLeg->GetRigidBody(), localA, localB);
	RhipConst->setLimit(btScalar(M_PI_4), btScalar(M_PI_4), 0);

	JointPos = Utils::OgreBTVector(LUpperLeg->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, -(leg_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, LUpperLeg, LLowerLeg, btVector3(0, btScalar(M_PI_2), 0));
	btHingeConstraint* LLegConst = new btHingeConstraint(*LUpperLeg->GetRigidBody(), *LLowerLeg->GetRigidBody(), localA, localB);
	LLegConst->setLimit(0, btScalar(M_PI_2));

	JointPos = Utils::OgreBTVector(RUpperLeg->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, -(leg_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, RUpperLeg, RLowerLeg, btVector3(0, btScalar(M_PI_2), 0));
	btHingeConstraint* RLegConst = new btHingeConstraint(*RUpperLeg->GetRigidBody(), *RLowerLeg->GetRigidBody(), localA, localB);
	RLegConst->setLimit(0, btScalar(M_PI_2));

	JointPos = Utils::OgreBTVector(LLowerLeg->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, -(leg_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, LLowerLeg, LFoot, btVector3(0, btScalar(M_PI_2), 0));
	btHingeConstraint* LFootConst = new btHingeConstraint(*LLowerLeg->GetRigidBody(), *LFoot->GetRigidBody(), localA, localB);
	LFootConst->setLimit(0, btScalar(M_PI_2));

	JointPos = Utils::OgreBTVector(RLowerLeg->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, -(leg_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, RLowerLeg, RFoot, btVector3(0, btScalar(M_PI_2), 0));
	btHingeConstraint* RFootConst = new btHingeConstraint(*RLowerLeg->GetRigidBody(), *RFoot->GetRigidBody(), localA, localB);
	RFootConst->setLimit(0, btScalar(M_PI_2));

	mConstraints.push_back(LhipConst);
	mConstraints.push_back(RhipConst);
	mConstraints.push_back(LLegConst);
	mConstraints.push_back(RLegConst);
	mConstraints.push_back(LFootConst);
	mConstraints.push_back(RFootConst);

	return true;
}
bool Skeleton_Builder::BuildTail(float tail_width, float tail_height, float torso_height)
{
	Bone* UTail = new Bone();
	Bone* MTail = new Bone();
	Bone* LTail = new Bone();
	Bone* LLTail = new Bone();
	tail_height /= 4;
	
	Ogre::Quaternion neckrotation;
	neckrotation.FromAngleAxis(Ogre::Radian(Ogre::Degree(mTailIncline)), Ogre::Vector3::UNIT_X);
	btTransform t;
	btVector3 min, max;

	mHipNode->GetCollider()->getAabb(t, min, max);
	mBuilder->SetDimensions(tail_width, tail_height, mDepth / 2);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, -(torso_height /2), mDepth / 2), neckrotation, *mHipNode->GetNode());
	if (!mBuilder->BuildBone(*LLTail, Bone::BoneType::Tail))
		return false;
	mBuilder->SetDimensions(tail_width, tail_height, mDepth / 2);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, -tail_height , 0), neckrotation, *LLTail->GetNode());
	if (!mBuilder->BuildBone(*LTail, Bone::BoneType::Tail))
		return false;
	mBuilder->SetDimensions(tail_width, tail_height, mDepth / 2);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, -tail_height, 0), neckrotation, *LTail->GetNode());
	if (!mBuilder->BuildBone(*MTail, Bone::BoneType::Tail))
		return false;
	mBuilder->SetDimensions(tail_width, tail_height, mDepth / 2);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, -tail_height, 0), neckrotation, *MTail->GetNode());
	if (!mBuilder->BuildBone(*UTail, Bone::BoneType::Tail))
		return false;
	if (mHasMuscle)
	{
		mMuscleBuilder->CreateMuscle(mHipNode, LLTail, mMuscles);
		mMuscleBuilder->CreateMuscle(LLTail, LTail, mMuscles);
		mMuscleBuilder->CreateMuscle(LTail, MTail, mMuscles);
		mMuscleBuilder->CreateMuscle(MTail, UTail, mMuscles);
	}

	//add joints
	btTransform localA, localB;
	btVector3 JointPos;

	JointPos = Utils::OgreBTVector(mHipNode->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (torso_height / 2), 0));
	SetJointTransform(localA, localB, JointPos, mHipNode, LLTail, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* shoulderNeckConst = new btConeTwistConstraint(*mHipNode->GetRigidBody(), *LLTail->GetRigidBody(), localA, localB);
	shoulderNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4),btScalar(M_PI_4 / 4));

	JointPos = Utils::OgreBTVector(LLTail->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (tail_height), 0));
	SetJointTransform(localA, localB, JointPos, LLTail, LTail, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* LNeckConst = new btConeTwistConstraint(*LLTail->GetRigidBody(), *LTail->GetRigidBody(), localA, localB);
	LNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

	JointPos = Utils::OgreBTVector(LTail->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (tail_height), 0));
	SetJointTransform(localA, localB, JointPos, LTail, MTail, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* MNeckConst = new btConeTwistConstraint(*LTail->GetRigidBody(), *MTail->GetRigidBody(), localA, localB);
	MNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

	JointPos = Utils::OgreBTVector(MTail->GetNode()->_getDerivedPosition() + Ogre::Vector3(0, (tail_height), 0));
	SetJointTransform(localA, localB, JointPos, MTail, UTail, btVector3(0, btScalar(-M_PI_2), 0));
	btConeTwistConstraint* HNeckConst = new btConeTwistConstraint(*MTail->GetRigidBody(), *UTail->GetRigidBody(), localA, localB);
	HNeckConst->setLimit(btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4), btScalar(M_PI_4 / 4));

	mConstraints.push_back(shoulderNeckConst);
	mConstraints.push_back(LNeckConst);
	mConstraints.push_back(MNeckConst);
	mConstraints.push_back(HNeckConst);

	return true;
}
void Skeleton_Builder::SetJointTransform(btTransform& TransformA, btTransform& TransformB, btVector3& JointPosWorld, Bone* boneA, Bone* boneB, btVector3& Axis)
{
	btQuaternion btquat;
	btTransform JointTrans;
	btquat.setEulerZYX(Axis.z(), Axis.y(), Axis.x());
	JointTrans.setIdentity();
	JointTrans.setRotation(btquat);
	JointTrans.setOrigin(JointPosWorld);
	TransformA = boneA->GetRigidBody()->getWorldTransform().inverse() * JointTrans;
	TransformB = boneB->GetRigidBody()->getWorldTransform().inverse() * JointTrans;
}