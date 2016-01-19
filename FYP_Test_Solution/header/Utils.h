#pragma once
#include<OGRE/Ogre.h>
#include<Bullet/btBulletCollisionCommon.h>
#define BIT(x) (1<<(x))
class Utils
{
public:
	static inline Ogre::Vector3 BTOgreVector(const btVector3 &vec)
	{
		return Ogre::Vector3(vec.x(), vec.y(), vec.z());
	}
	static inline btVector3 OgreBTVector(const Ogre::Vector3 vec)
	{
		return btVector3(vec.x, vec.y, vec.z);
	}
	static inline Ogre::Quaternion BTOgreQuat(const btQuaternion &quat)
	{
		return Ogre::Quaternion(quat.w(), quat.x(), quat.y(), quat.z());
	}
	static inline btQuaternion OgreBTQuat(const Ogre::Quaternion quat)
	{
		return btQuaternion(quat.w, quat.x, quat.y, quat.z);
	}
	static inline btTransform OgreNodeBtTransform(const Ogre::Node* node)
	{

		return btTransform(OgreBTQuat(node->_getDerivedOrientation()), OgreBTVector(node->_getDerivedPosition()));
	}
	static enum collisiontypes
	{
		COL_NOTHING = 0, //<Collide with nothing
		COL_SKELETON = BIT(1), //<Collide with skeleton
		COL_WALL = BIT(2) //<Collide with walls
	};
};