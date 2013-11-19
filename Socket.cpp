#include "Socket.h"
#include "ComplexBlock.h"

Socket :: Socket():
	isElectricalSocket(false), powerProvided(0.0f), powerConsumed(0.0f), pMapElement(0)
{
}

void Socket :: Init(ComplexBlock* setPMapElement, int setSocketSide)
{
	pMapElement = setPMapElement;
	vector3di position = pMapElement->GetBlockPos();

	switch(setSocketSide)
	{
	case SOCKET_TOP:
		pos =  vector3di(position.X, position.Y + 1, position.Z);
		break;

	case SOCKET_BOTTOM:
		pos = vector3di(position.X, position.Y - 1, position.Z);
		break;

	case SOCKET_LEFT:
		pos = vector3di(position.X - 1, position.Y, position.Z);
		break;

	case SOCKET_RIGHT:
		pos = vector3di(position.X + 1, position.Y, position.Z);
		break;

	case SOCKET_FRONT:
		pos = vector3di(position.X, position.Y, position.Z + 1);
		break;

	case SOCKET_BACK:
		pos = vector3di(position.X, position.Y, position.Z - 1);
		break;
	}
}

void Socket :: ConsumePower(float amount)
{
	pMapElement->ConsumePower(amount);
}

float Socket :: ProvidePower(float desiredAmount)
{
	if(desiredAmount > powerProvided)
	{
		return powerProvided;
	}
	else
	{
		return desiredAmount;
	}
}
