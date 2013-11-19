#ifndef SOCKET_H
#define SOCKET_H

#include "IrrWrapper.h"
class ComplexBlock;

const int FIRST_SOCKET = 0;
const int LAST_SOCKET = 6;

enum SocketSide
{
	SOCKET_TOP = 0,
	SOCKET_BOTTOM,
	SOCKET_LEFT,
	SOCKET_RIGHT,
	SOCKET_FRONT,
	SOCKET_BACK
};

class Socket
{
public:

	Socket();
	void Init(ComplexBlock* setPMapElement, int setSocketSide);

	// Consume and provide power through socket:
	void ConsumePower(float amount);
	float ProvidePower(float desiredAmount);

	// Accessors:
	float GetPowerProvided() const {return powerProvided;}
	float GetPowerConsumed() const {return powerConsumed;}
	bool IsElectricalSocket() const {return isElectricalSocket;}
	vector3di GetPos() const {return pos;}

	bool isElectricalSocket;
	float powerProvided;
	float powerConsumed;
	int socketSide;
	vector3di pos;
	ComplexBlock* pMapElement;

	// Signal sent
	// Signal received
	// gasSource
	// gasSinkDemand
	// gasSinkSupplied

};

#endif