#pragma once
#include<OGRE/Ogre.h>
#include<Bullet/btBulletCollisionCommon.h>

class Utils
{
public:
	static inline const Ogre::Vector3 BTOgreVector(const btVector3 &vec)
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
};