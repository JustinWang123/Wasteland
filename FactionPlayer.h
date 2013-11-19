#ifndef FACTION_PLAYER_H
#define FACTION_PLAYER_H

#include "Faction.h"
#include "IrrWrapper.h"
#include <list>
#include "Button.h"
#include "SoldierInventoryInterface.h"

class CharacterInstanceSoldier;

enum PlayerFactionInterfaceState
{
	PLAYER_FACTION_INTERFACE_STATE_GAME = 0,
	PLAYER_FACTION_INTERFACE_STATE_INVENTORY
};

enum LeftClickFunction
{
	LEFT_CLICK_FUNCTION_SELECT_SOLDIER,
	LEFT_CLICK_FUNCTION_HARVEST_BLOCK
};

enum MouseContext
{
    MOUSE_CONTEXT_MOVE,
    MOUSE_CONTEXT_ATTACK
};

const unsigned short FACTION_PLAYER_MAX_SOLDIERS = 6;

class FactionPlayer : public Faction
{
public:
    FactionPlayer();
    ~FactionPlayer();

    virtual void Update();

private:
    // States:
    void UpdateGameState();
    void UpdateInventoryState();
	void DrawInterfaceForSingleSelection();
	void DrawInterfaceForMultipleSelection();
	void UpdateInterfaceForSingleSelection();
	void UpdateInterfaceForMultipleSelection();

    // Update Camera Methods:
    void UpdateCameraMovement();
	void UpdateCameraRotation();
	void UpdateCameraZoom();
	void UpdateCameraPosition();

    // Soldiers:
    void UpdateLeftClickFunction();
    int  GetMouseContext();
    void IssueCommands();
    bool IsSoldierSelected(CharacterInstanceSoldier* soldier);

    // Interface:
    bool IsMouseOnGameScreen();

	// Camera Members:
	vector3df		cameraTarget;
	vector2df		cameraAngle;
	float			cameraDistance;
	vector2df		cameraStartAngle;
	vector2df		mouseStartPosition;
	bool			isRotatingCamera;

	// Soldiers:
	std::list<CharacterInstanceSoldier*> selectedSoldiers;

	// Interface:
	PlayerFactionInterfaceState interfaceState;
	LeftClickFunction			leftClickFunction;
	Button 						standButton;
	Button 						crouchButton;
	Button 						proneButton;
	Button						harvestButton;
	IMeshSceneNode* 			characterSelectSceneNode[FACTION_PLAYER_MAX_SOLDIERS];
	ITexture*					emptySlotTex;
	SoldierInventoryInterface	soldierInventoryInterface;

	// Debug:
	ISceneNode*	debugSceneNode;
};
#endif
