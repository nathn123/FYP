#include "..\header\Skeleton_Builder.h"
#include "Utils.h"



Skeleton_Builder::Skeleton_Builder(Ogre::SceneManager* scene,btDynamicsWorld* world)
{
	mSceneMgr = scene;
	mWorld = world;
	mBuilder = new Bone_Builder(mSceneMgr, mWorld);
}


Skeleton_Builder::~Skeleton_Builder()
{
}

void Skeleton_Builder::SetDimensions(int NeckIncline, int TailIncline, int height,int width)
{
	mNeckIncline = NeckIncline;
	mTailIncline = TailIncline;
	mHeight = height;
	mWidth = width;
}
void Skeleton_Builder::SetBodyType(LegType leg, ArmType arm, TorsoType torso)
{
	mLeg = leg;
	mArm = arm;
	mTorso = torso;
}
bool Skeleton_Builder::BuildSkeleton(Skeleton* newSkel)
{
	if (mLeg == LegType::None || mTorso == TorsoType::None1 || mHeight <= 10) // check to ensure all attribs required
		return false;
	
	int neck_height;
	int torso_height;
	int Leg_height;
	int tail_height;
	//create base node for skeleton
	auto node = mSceneMgr->createSceneNode();
	// set block area heights
	// if longneck  neck/legs ratio = 2:1
	// if shortneck neck/legs ratio = 1:2
	// if upright torso 40% 
	// else torso 25% 
	
	
	if (Upright())
	{
		auto onepercent = mHeight / 100;
		torso_height = onepercent * 40;
		if (LongNeck())
		{
			neck_height = onepercent * 40;
			Leg_height = onepercent * 20;
		}
		else
		{
			neck_height = onepercent * 20;
			Leg_height = onepercent * 40;
		}
	}
	else
	{
		auto onepercent = mHeight / 100;
		torso_height = onepercent * 25;
		if (LongNeck())
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
	if (LongTail())
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
	int neck_width;
	int torso_width;
	int Leg_width;
	int Arm_width;

	auto onepercentW = mWidth / 100;
	// arms take 35% of width i.e 17.5% each side if needed
	torso_width = onepercentW * 65;
	Leg_width = onepercentW * 25; // ensures gaps between legs like realistic bipeds
	Arm_width = onepercentW * 35;
	neck_width = onepercentW * 35;

	//build the torso here

	if (Upright())
	{
		Bone* CentreTorso = nullptr;
		Bone* TopTorso = nullptr;
		Bone* BotTorso = nullptr;
		//build 3 torso pieces
		auto torsoPiece_height = torso_height / 3;
		//build centre piece
		mBuilder->SetDimensions(torso_width, torsoPiece_height);
		mBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, node);
		if (!mBuilder->BuildBone(CentreTorso))
			return false;
		//build bot piece
		mBuilder->SetDimensions(torso_width, torsoPiece_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, -torsoPiece_height, 0), Ogre::Quaternion::IDENTITY, CentreTorso->GetNode());
		if (!mBuilder->BuildBone(BotTorso))
			return false;
		//build top piece
		mBuilder->SetDimensions(torso_width, torsoPiece_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, torsoPiece_height, 0), Ogre::Quaternion::IDENTITY, CentreTorso->GetNode());
		if (!mBuilder->BuildBone(TopTorso))
			return false;
		mShouldernode = TopTorso;
		mHipNode = BotTorso;

		mBones.push_back(CentreTorso);
		mBones.push_back(TopTorso);
		mBones.push_back(BotTorso);
		
	}
	else
	{
		Bone* CentreTorso = nullptr;
		//build centre piece
		mBuilder->SetDimensions(torso_width, torso_height);
		mBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, node);
		if (!mBuilder->BuildBone(CentreTorso))
			return false;
		mBones.push_back(CentreTorso);
		mShouldernode = CentreTorso;
		mHipNode = CentreTorso;
	}

	if (!BuildArm(Arm_width,Leg_height,torso_width) || !BuildNeck(neck_width,neck_height) || !BuildLeg(Leg_width,Leg_height,torso_width))
		return false;

	if (ShortTail() || LongTail())
		if(!BuildTail(neck_width,tail_height))
			return false;

		//btCompoundCollisionShape* totalcollider
		//	= new btCompoundCollisionShape();
		//for (auto bone : mBones)
		//	totalcollider->addChildShape(bone->GetCollider(),
		//	bone->GetNode()->convertLocalToWorldPosition(Ogre::Vector3::ZERO),
		//	bone->GetNode()->convertLocalToWorldOrientation(Ogre::Quaternion::IDENTITY));

		newSkel = new Skeleton(mBones,mConstraints, node,mWorld);

		ClearData();
		return true;
}
bool Skeleton_Builder::Upright()
{
	if (mTorso == LongNeckShortTailUpright ||
		mTorso == LongNeckLongTailUpright ||
		mTorso == LongNeckNoTailUpright ||
		mTorso == ShortNeckShortTailUpright ||
		mTorso == ShortNeckLongTailUpright ||
		mTorso == ShortNeckNoTailUpright)
		return true;
	return false;
}
bool Skeleton_Builder::LongNeck()
{
	if (mTorso == LongNeckShortTail ||
		mTorso == LongNeckLongTail ||
		mTorso == LongNeckNoTail ||
		mTorso == LongNeckShortTailUpright ||
		mTorso == LongNeckLongTailUpright ||
		mTorso == LongNeckNoTailUpright)
		return true;
	return false;
}
bool Skeleton_Builder::LongTail()
{
	if (mTorso == LongNeckLongTail ||
		mTorso == ShortNeckLongTail ||
		mTorso == LongNeckLongTailUpright ||
		mTorso == ShortNeckLongTailUpright )
		return true;
	return false;
}
bool Skeleton_Builder::ShortTail()
{
	if (mTorso == LongNeckShortTail ||
		mTorso == ShortNeckShortTail ||
		mTorso == LongNeckShortTailUpright ||
		mTorso == ShortNeckShortTailUpright)
		return true;
	return false;
}
void Skeleton_Builder::ClearData()
{
	mLeg = LegType::None;
	mTorso = TorsoType::None1;
	mHeight = -1;
	mBones.clear();
	mConstraints.clear();
}
bool Skeleton_Builder::BuildArm(int arm_Width, int arm_height,int torso_width)
{
	// 3 blocks no hands
	// only height/ width changes 
	if (mArm == ArmType::ShortArms)
	{
		//half height of legs
		Bone* Lshoulder = nullptr;
		Bone* LUpperArm = nullptr;
		Bone* LLowerArm = nullptr;
		//build shoulder piece
		
		mBuilder->SetDimensions(arm_Width, arm_height/2);
		mBuilder->SetRelativePosition(Ogre::Vector3(torso_width, 0, 0), Ogre::Quaternion::IDENTITY, mShouldernode->GetNode());
		if (!mBuilder->BuildBone(Lshoulder))
			return false;
		

		mBuilder->SetDimensions(arm_Width, arm_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(0,arm_height/2,0), Ogre::Quaternion::IDENTITY, Lshoulder->GetNode());
		if (!mBuilder->BuildBone(LUpperArm))
			return false;

		mBuilder->SetDimensions(arm_Width, arm_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(0,arm_height/2,0), Ogre::Quaternion::IDENTITY, LUpperArm->GetNode());
		if (!mBuilder->BuildBone(LLowerArm))
			return false;
		mBones.push_back(Lshoulder);
		mBones.push_back(LUpperArm);
		mBones.push_back(LLowerArm);

		Bone* Rshoulder = nullptr;
		Bone* RUpperArm = nullptr;
		Bone* RLowerArm = nullptr;

		mBuilder->SetDimensions(arm_Width, arm_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(-torso_width, 0, 0), Ogre::Quaternion::IDENTITY, mShouldernode->GetNode());
		if (!mBuilder->BuildBone(Rshoulder))
			return false;
		

		mBuilder->SetDimensions(arm_Width, arm_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height / 2, 0), Ogre::Quaternion::IDENTITY, Rshoulder->GetNode());
		if (!mBuilder->BuildBone(RUpperArm))
			return false;
		mBuilder->SetDimensions(arm_Width, arm_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height / 2, 0), Ogre::Quaternion::IDENTITY, RUpperArm->GetNode());
		if (!mBuilder->BuildBone(RLowerArm))
			return false;
		mBones.push_back(Rshoulder);
		mBones.push_back(RUpperArm);
		mBones.push_back(RLowerArm);
		//add joints
		Ogre::Quaternion shoulderrotation;
		shoulderrotation.FromAngleAxis(Ogre::Radian(Ogre::Math::PI / 2) / 2, Ogre::Vector3::UNIT_X);
		btConeTwistConstraint* LshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(),
			*Lshoulder->GetRigidBody(), Utils::OgreNodeBtTransform(Lshoulder->GetNode()), Utils::OgreNodeBtTransform(mShouldernode->GetNode()));

		btHingeConstraint* LArmConst = new btHingeConstraint(*LUpperArm->GetRigidBody(), *LLowerArm->GetRigidBody(),
			Utils::OgreNodeBtTransform(LLowerArm->GetNode()), Utils::OgreNodeBtTransform(LUpperArm->GetNode()));

		btConeTwistConstraint* RshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(),*Rshoulder->GetRigidBody(),
			Utils::OgreNodeBtTransform(Rshoulder->GetNode()), Utils::OgreNodeBtTransform(mShouldernode->GetNode()));

		btHingeConstraint* RArmConst = new btHingeConstraint(*RUpperArm->GetRigidBody(), *RLowerArm->GetRigidBody(),
			Utils::OgreNodeBtTransform(RLowerArm->GetNode()), Utils::OgreNodeBtTransform(RUpperArm->GetNode()));

		mConstraints.push_back(LshoulderConst);
		mConstraints.push_back(LArmConst);
		mConstraints.push_back(RshoulderConst);
		mConstraints.push_back(RArmConst);
		
	}
	else if (mArm == ArmType::LongArms)
	{
		// same height of legs
		Bone* Lshoulder = nullptr;
		Bone* LUpperArm = nullptr;
		Bone* LLowerArm = nullptr;
		//build shoulder piece

		mBuilder->SetDimensions(arm_Width, arm_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(torso_width, 0, 0), Ogre::Quaternion::IDENTITY, mShouldernode->GetNode());
		if (!mBuilder->BuildBone(Lshoulder))
			return false;

		mBuilder->SetDimensions(arm_Width, arm_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height, 0), Ogre::Quaternion::IDENTITY, Lshoulder->GetNode());
		if (!mBuilder->BuildBone(LUpperArm))
			return false;

		mBuilder->SetDimensions(arm_Width, arm_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height, 0), Ogre::Quaternion::IDENTITY, LUpperArm->GetNode());
		if (!mBuilder->BuildBone(LLowerArm))
			return false;
		mBones.push_back(Lshoulder);
		mBones.push_back(LUpperArm);
		mBones.push_back(LLowerArm);

		Bone* Rshoulder = nullptr;
		Bone* RUpperArm = nullptr;
		Bone* RLowerArm = nullptr;

		mBuilder->SetDimensions(arm_Width, arm_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(-torso_width, 0, 0), Ogre::Quaternion::IDENTITY, mShouldernode->GetNode() );
		if (!mBuilder->BuildBone(Rshoulder))
			return false;
		mBuilder->SetDimensions(arm_Width, arm_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height, 0), Ogre::Quaternion::IDENTITY, Rshoulder->GetNode());
		if (!mBuilder->BuildBone(RUpperArm))
			return false;
		mBuilder->SetDimensions(arm_Width, arm_height);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, arm_height, 0), Ogre::Quaternion::IDENTITY, RUpperArm->GetNode());
		if (!mBuilder->BuildBone(RLowerArm))
			return false;
		mBones.push_back(Rshoulder);
		mBones.push_back(RUpperArm);
		mBones.push_back(RLowerArm);
		//add joints
		Ogre::Quaternion shoulderrotation;
		shoulderrotation.FromAngleAxis(Ogre::Radian(Ogre::Math::PI / 2) / 2, Ogre::Vector3::UNIT_X);

		btConeTwistConstraint* LshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(),
			*Lshoulder->GetRigidBody(), Utils::OgreNodeBtTransform(Lshoulder->GetNode()), Utils::OgreNodeBtTransform(mShouldernode->GetNode()));

		btHingeConstraint* LArmConst = new btHingeConstraint(*LUpperArm->GetRigidBody(), *LLowerArm->GetRigidBody(),
			Utils::OgreNodeBtTransform(LLowerArm->GetNode()), Utils::OgreNodeBtTransform(LUpperArm->GetNode()));

		btConeTwistConstraint* RshoulderConst = new btConeTwistConstraint(*mShouldernode->GetRigidBody(), *Rshoulder->GetRigidBody(),
			Utils::OgreNodeBtTransform(Rshoulder->GetNode()), Utils::OgreNodeBtTransform(mShouldernode->GetNode()));

		btHingeConstraint* RArmConst = new btHingeConstraint(*RUpperArm->GetRigidBody(), *RLowerArm->GetRigidBody(),
			Utils::OgreNodeBtTransform(RLowerArm->GetNode()), Utils::OgreNodeBtTransform(RUpperArm->GetNode()));

		mConstraints.push_back(LshoulderConst);
		mConstraints.push_back(LArmConst);
		mConstraints.push_back(RshoulderConst);
		mConstraints.push_back(RArmConst);
	}
	// means no arms to continue

	return true;
}
bool Skeleton_Builder::BuildNeck(int neck_width, int neck_height)
{
	Bone* UNeck = nullptr;
	Bone* MNeck = nullptr;
	Bone* LNeck = nullptr;
	Bone* LLNeck = nullptr;

	mBuilder->SetDimensions(neck_width, neck_height / 4);
	Ogre::Quaternion neckrotation;
	neckrotation.FromAngleAxis(Ogre::Radian(Ogre::Degree(mNeckIncline)), Ogre::Vector3::UNIT_X);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height / 4, 0), neckrotation, mShouldernode->GetNode());
	if (!mBuilder->BuildBone(LLNeck))
		return false;
	mBuilder->SetDimensions(neck_width, neck_height / 4);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height / 4, 0), neckrotation, LLNeck->GetNode());
	if (!mBuilder->BuildBone(LNeck))
		return false;
	mBuilder->SetDimensions(neck_width, neck_height / 4);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height / 4, 0), neckrotation, LNeck->GetNode());
	if (!mBuilder->BuildBone(MNeck))
		return false;
	mBuilder->SetDimensions(neck_width, neck_height / 4);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, neck_height / 4, 0), neckrotation, MNeck->GetNode());
	if (!mBuilder->BuildBone(UNeck))
		return false;
	
	//add joints
	Ogre::Quaternion shoulderrotation;
	shoulderrotation.FromAngleAxis(Ogre::Radian(Ogre::Math::PI / 2) / 2, Ogre::Vector3::UNIT_X);

	btHingeConstraint* LNeckConst = new btHingeConstraint(*LLNeck->GetRigidBody(), *LNeck->GetRigidBody(),
		Utils::OgreNodeBtTransform(LNeck->GetNode()), Utils::OgreNodeBtTransform(LLNeck->GetNode()));
	btHingeConstraint* MNeckConst = new btHingeConstraint(*LNeck->GetRigidBody(), *MNeck->GetRigidBody(),
		Utils::OgreNodeBtTransform(MNeck->GetNode()), Utils::OgreNodeBtTransform(LNeck->GetNode()));
	btHingeConstraint* HNeckConst = new btHingeConstraint(*MNeck->GetRigidBody(), *UNeck->GetRigidBody(),
		Utils::OgreNodeBtTransform(UNeck->GetNode()), Utils::OgreNodeBtTransform(MNeck->GetNode()));
	mConstraints.push_back(LNeckConst);
	mConstraints.push_back(MNeckConst);
	mConstraints.push_back(HNeckConst);

	return true;
}
bool Skeleton_Builder::BuildLeg(int leg_width,int leg_height,int torso_width)
{
	auto halfgap = (torso_width / 2) - leg_width;
	auto legnodepos = halfgap + leg_width / 2;

	Bone* LUpperLeg = nullptr;
	Bone* LLowerLeg = nullptr;
	Bone* LFoot = nullptr;
	Bone* RUpperLeg = nullptr;
	Bone* RLowerLeg = nullptr;
	Bone* RFoot = nullptr;
	if (mLeg == LegType::Uninverted)
	{
		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, mHipNode->GetNode());
		if (!mBuilder->BuildBone(LUpperLeg))
			return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, leg_height / 2, 0), Ogre::Quaternion::ZERO, LUpperLeg->GetNode());
		if (!mBuilder->BuildBone(LLowerLeg))
			return false;

		// foot generation
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, Hip->GetNode());
		//if (!mBuilder->BuildBone(LUpperLeg))
		//	return false;

		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, mHipNode->GetNode());
		if (!mBuilder->BuildBone(RUpperLeg))
			return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(0, leg_height / 2, 0), Ogre::Quaternion::ZERO, RUpperLeg->GetNode());
		if (!mBuilder->BuildBone(RLowerLeg))
			return false;

		// foot generation
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, Hip->GetNode());
		//if (!mBuilder->BuildBone(LUpperLeg))
		//	return false;
	}
	else
	{
		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, mHipNode->GetNode());
		if (!mBuilder->BuildBone(LUpperLeg))
			return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::ZERO, LUpperLeg->GetNode());
		if (!mBuilder->BuildBone(LLowerLeg))
			return false;

		// foot generation
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, Hip->GetNode());
		//if (!mBuilder->BuildBone(LUpperLeg))
		//	return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, mHipNode->GetNode());
		if (!mBuilder->BuildBone(RUpperLeg))
			return false;
		mBuilder->SetDimensions(leg_width, leg_height / 2);
		mBuilder->SetRelativePosition(Ogre::Vector3::ZERO, Ogre::Quaternion::ZERO, RUpperLeg->GetNode());
		if (!mBuilder->BuildBone(RLowerLeg))
			return false;

		// foot generation
		//mBuilder->SetDimensions(leg_width, leg_height / 2);
		//mBuilder->SetRelativePosition(Ogre::Vector3(legnodepos, 0, 0), Ogre::Quaternion::ZERO, Hip->GetNode());
		//if (!mBuilder->BuildBone(LUpperLeg))
		//	return false;
	}
	mBones.push_back(LUpperLeg);
	mBones.push_back(LLowerLeg);
	mBones.push_back(RUpperLeg);
	mBones.push_back(RLowerLeg);
	//mBones.push_back(LFoot);
	//mBones.push_back(RFoot);

	//add joints

	btConeTwistConstraint* LhipConst = new btConeTwistConstraint(*mHipNode->GetRigidBody(),*LUpperLeg->GetRigidBody(),
		Utils::OgreNodeBtTransform(LUpperLeg->GetNode()), Utils::OgreNodeBtTransform(mHipNode->GetNode()));
	btHingeConstraint* LLegConst = new btHingeConstraint(*LUpperLeg->GetRigidBody(), *LLowerLeg->GetRigidBody(),
		Utils::OgreNodeBtTransform(LLowerLeg->GetNode()), Utils::OgreNodeBtTransform(LUpperLeg->GetNode()));

	btConeTwistConstraint* RhipConst = new btConeTwistConstraint(*mHipNode->GetRigidBody(),*RUpperLeg->GetRigidBody(),
		Utils::OgreNodeBtTransform(RUpperLeg->GetNode()), Utils::OgreNodeBtTransform(mHipNode->GetNode()));
	btHingeConstraint* RLegConst = new btHingeConstraint(*RUpperLeg->GetRigidBody(), *RLowerLeg->GetRigidBody(),
		Utils::OgreNodeBtTransform(RLowerLeg->GetNode()), Utils::OgreNodeBtTransform(RUpperLeg->GetNode()));

	//btHingeConstraint* LFootConst = new btHingeConstraint(LLowerLeg->GetRigidBody(), LFoot->GetRigidBody(),
	//	Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_Y);
	//btHingeConstraint* RFootConst = new btHingeConstraint(RLowerLeg->GetRigidBody(), RFoot->GetRigidBody(),
	//	Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_Y);

	mConstraints.push_back(LhipConst);
	mConstraints.push_back(LLegConst);
	mConstraints.push_back(RhipConst);
	mConstraints.push_back(RLegConst);
	//mConstraints.push_back(LFootConst);
	//mConstraints.push_back(RFootConst);

	return true;
}
bool Skeleton_Builder::BuildTail(int tail_width,int tail_height)
{
	Bone* UTail = nullptr;
	Bone* MTail = nullptr;
	Bone* LTail = nullptr;
	Bone* LLTail = nullptr;

	mBuilder->SetDimensions(tail_width, tail_height / 4);
	Ogre::Quaternion neckrotation;
	neckrotation.FromAngleAxis(Ogre::Radian(Ogre::Degree(mTailIncline)), Ogre::Vector3::UNIT_X);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, tail_height / 4, 0), neckrotation, mHipNode->GetNode());
	if (!mBuilder->BuildBone(LLTail))
		return false;
	mBuilder->SetDimensions(tail_width, tail_height / 4);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, tail_height / 4, 0), neckrotation, LLTail->GetNode());
	if (!mBuilder->BuildBone(LTail))
		return false;
	mBuilder->SetDimensions(tail_width, tail_height / 4);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, tail_height / 4, 0), neckrotation, LTail->GetNode());
	if (!mBuilder->BuildBone(MTail))
		return false;
	mBuilder->SetDimensions(tail_width, tail_height / 4);
	mBuilder->SetRelativePosition(Ogre::Vector3(0, tail_height / 4, 0), neckrotation, MTail->GetNode());
	if (!mBuilder->BuildBone(UTail))
		return false;

	return true;
}

