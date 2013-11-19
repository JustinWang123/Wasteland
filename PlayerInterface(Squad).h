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
	
	void UpdateCameraMovement();
	void UpdateCameraRotation();
	void UpdateCameraZoom();
	void UpdateCameraPosition();

	void UpdateSelectingPlayers();
	void UpdateInventoryManagement();
	void UpdateHUDInteraction();

	void IssueMovementCommands();
	void IssueBlockBreakCommands();
	void IssuePlaceBlockCommands();
	void IssueAttackCommands();

	void DrawHUD();

	vector3df	GetCameraTarget() const {return cameraTarget;}

	bool	IsPlayerSelected(PlayerInstance* pc);
private:
	PlayerInstance* playerInstance;

	// Camera stuff:
	vector3df		cameraTarget;
	vector2df		cameraAngle;
	float			cameraDistance;
	vector2df		cameraStartAngle;
	vector2df		mouseStartPosition;
	bool			isRotatingCamera;

	InterfaceState	interfaceState;
	int				playerInventorySelected;

	ISceneNode*	targetCube;

	// For issuing commands from the HUDs hotbar:
	ITexture*	hotbarSlotSelectTex;
	bool		issueCommandModeOn;
	int			issueCommandPlayer;
	int			issueCommandHotbarSlot;
	int			issueCommandItemID;

	int			HUDYPos;
};

#define PInterface PlayerInterface::Instance()

#endif