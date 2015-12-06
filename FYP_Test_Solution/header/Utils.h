#pragma once
#include<OGRE/Ogre.h>
#include<Bullet/btBulletCollisionCommon.h>

class Utils
{
public:
	static Ogre::Vector3 BTOgreVector(btVector3 vec)
	{
		return Ogre::Vector3(vec.x(), vec.y(), vec.z());
	}
	static btVector3 OgreBTVector(Ogre::Vector3 vec)
	{
		return btVector3(vec.x, vec.y, vec.z);
	}
	static Ogre::Quaternion BTOgreQuat(btQuaternion quat)
	{
		return Ogre::Quaternion(quat.w(), quat.x(), quat.y(), quat.z());
	}
	static btQuaternion OgreBTQuat(Ogre::Quaternion quat)
	{
		return btQuaternion(quat.w, quat.x, quat.y, quat.z);
	}
};