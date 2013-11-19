#ifndef COMPLEX_BLOCK_H
#define COMPLEX_BLOCK_H

#include "BaseBlock.h"
#include "Socket.h"
struct ComplexBlockTemplate;


class ComplexBlock : public BaseBlock
{
public:
	ComplexBlock(ComplexBlockTemplate* setElementTemplate, vector3di setPos, vector3di setDirr);
	~ComplexBlock();

	virtual void Update(){}
	virtual std::string GetDebugString(float x, float y, float z);

	// Socket Accessors:
	Socket* GetSocket(int socketSide) {return &sockets[socketSide];}

	// ElectricalAccessors:
	virtual void ConsumePower(float amount);
	virtual float ProvidePower(float desiredAmount);
	virtual bool IsElectrical();

	// Get Accessors:
	vector3di GetBlockPos() {return vector3di(sceneNode->getPosition().X, sceneNode->getPosition().Y, sceneNode->getPosition().Z);}

protected:
	ComplexBlockTemplate* GetTemplate() {return (ComplexBlockTemplate*)pTemplate;}

	IMeshSceneNode* sceneNode;

	Socket sockets[6];
};
#endif