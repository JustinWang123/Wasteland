#ifndef BASE_BLOCK_H
#define BASE_BLOCK_H

#include "IrrWrapper.h"
#include <string>
#include "Perameters.h"
#include "BaseBlockTemplate.h"

const int LAST_SIMPLE_BLOCK_ID = 1000;

class BaseBlock
{
public:
	BaseBlock();
	BaseBlock(int setID);
	virtual ~BaseBlock() {}

	virtual void Update()=0;
	virtual std::string GetDebugString(float x, float y, float z)=0;

	// Accessors:
	int	ID() const {return id;}
	std::string GetName() const {return name;}
	bool IsSimpleBlock();

	// ElectricalAccessors:
	virtual bool IsElectrical()=0;
	virtual void ConsumePower(float amount)=0;
	virtual float ProvidePower(float desiredAmount)=0;

	// Accessors:
	virtual int GetDropID();
	virtual int	GetDropAmount();
	virtual int GetDamageType();

protected:
	int id;
	static int nextID;

	std::string name;
	BaseBlockTemplate* pTemplate;


};


#endif
