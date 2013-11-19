#ifndef SIMPLE_BLOCK_H
#define SIMPLE_BLOCK_H

#include "BaseBlock.h"
struct BaseBlockTemplate;
class MapManager;


class SimpleBlock : public BaseBlock
{
public:
	SimpleBlock(int setID, BaseBlockTemplate* BaseBlockTemplate); // Used for setting restricted ids: 1-1000 for flyweight entities

	virtual void Update(){};
	virtual std::string GetDebugString(float x, float y, float z){return name;}

	// ElectricalAccessors:
	virtual void ConsumePower(float amount);
	virtual float ProvidePower(float desiredAmount);
	virtual bool IsElectrical();

	// Get Accessors:
	vector2df GetTextureCoords();

protected:
	BaseBlockTemplate* GetTemplate() {return (BaseBlockTemplate*)pTemplate;}

};
#endif