#pragma
#ifndef PLAYER_H
#define PLAYER_H

#include "CharacterInstance.h"
#include "PlayerInventoryInterface.h"
#include <string>
#include <deque>
struct CharacterInstanceTemplate;

enum PlayerState
{
	PLAYER_STATE_USE_ITEM = 0,
	PLAYER_STATE_BUILD,
	PLAYER_STATE_INVENTORY,
	PLAYER_STATE_BUILD_MENU
};

class PlayerInstance : public CharacterInstance
{
public:
	PlayerInstance(CharacterInstanceTemplate* setTemplate, vector3df setPos);
	~PlayerInstance();

	virtual void	Update();
	virtual bool	HandleMessage(Telegram& msg);
	
private:
	// -------------------------------------------------------------------
	// Private Methods:
	// -------------------------------------------------------------------
	// Game States:
	void UpdateGameState();
	void UpdateBuildState();
	void UpdateInventoryState();

	// Update Sub Routines:
	virtual void UpdateMovementVelocity();
	virtual void UpdateRotation();
	
	// HUD
	void DrawGameHUD();
	void DrawSelectedItem();

	void UseItem();
	void AddToChatWindow(std::string text) { chatWindow.push_front(text);}

	// -------------------------------------------------------------------
	// Private Members:
	// -------------------------------------------------------------------
	// Rotation:
	float	yRotation;
	float	xRotation;
	float	rotX[3];
	float	rotY[3];
	bool	firstRot;

	// Camera Vector:
	vector3df	camUp;

	// State:
	PlayerState playerState;

	// Building:
	int buildBlockIDHotbar[HOT_BAR_SIZE];

	// Interfaces:
	PlayerInventoryInterface inventoryInterface;
	
	// Inventory Textures:
	ITexture*	inventorySelectTex;
	
	// HUD:
	ITexture*	aimmerTex;
	ISceneNode* targetCube; // Cube scene node for visualizing target block:
	IMeshSceneNode* objectPlaceNode; // SceneNode for visualization of object placement:
	IMeshSceneNode* selectedItemNode; // SceneNode for currently selected Item

	// Chat Window for Logging:
	std::deque<std::string>		chatWindow;
};
#endif
