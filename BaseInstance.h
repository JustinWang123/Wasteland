#ifndef BASE_INSTANCE_H
#define BASE_INSTANCE_H

#include "IrrWrapper.h"
#include "Telegram.h"

class BaseInstance
{
public:
	BaseInstance();
	virtual ~BaseInstance();

	virtual bool 		HandleMessage(Telegram& msg) { return false;}
	int 				GetID() {return id;}
	IMeshSceneNode* 	GetSceneNode() {return sceneNode;}
	unsigned int 		GetBitMask() {return bitMask;}

	virtual vector3df 	GetPosition() {return sceneNode->getPosition();}
	vector3df 			GetRotation() {return sceneNode->getRotation();}

	virtual bool 		IsPosSolid(vector3df pos) { return false;}
    virtual bool 		IsUnderScreenPos(vector2di screenPos);
protected:
	int 				id;
	static int 			nextID;

	int 				bitMask;
	IMeshSceneNode* 	sceneNode;
};

#endif
