#pragma

#ifndef PLAYER_INTERFACE_H
#define PLAYER_INTERFACE_H

#include "IrrWrapper.h"
#include "ItemSlot.h"
#include <vector>
class PlayerInstance;

enum InterfaceState
{
	INTERFACE_STATE_GAME = 0,
	INTERFACE_STATE_INVENTORY
};

class PlayerInterface
{
public:
	static PlayerInterface* Instance();

	PlayerInterface();

	void Update();
	
	void UpdateCamera();
	void UpdateMovement();
	void UpdateInventoryManagement();
	void UpdateHUDInteraction();

	void DrawHUD();

private:
	PlayerInstance* playerInstance;

	// Camera stuff:
	vector2df		cameraAngle;

	InterfaceState	interfaceState;

	ISceneNode*	targetCube;
};

#define PInterface PlayerInterface::Instance()

#endif