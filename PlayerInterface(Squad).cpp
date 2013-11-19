#include "PlayerInterface.h"
#include "CharacterInstance.h"
#include "GameManager.h"
#include "CharacterCommand.h"
#include "PathPlanner.h"
#include <vector>
#include "CommandMoveToPosition.h"
#include "CommandDestroyBlock.h"
#include "CommandPathToPosition.h"
#include "CommandPlaceBlock.h"
#include "CommandAttackCharacter.h"

PlayerInterface* PlayerInterface :: Instance()
{

	static PlayerInterface instance;
	return &instance;


}

PlayerInterface :: PlayerInterface()
{
	pcInstances = GManager->GetPlayerInstances();
	cameraTarget = vector3df(20,20,20);
	cameraAngle.Y = 0.0f;
	cameraAngle.X = 45.0f;
	cameraDistance = 10.0f;

	isRotatingCamera = false;
	IRR->device->getCursorControl()->setVisible(true);

	interfaceState = INTERFACE_STATE_GAME;

	targetCube = IRR->sceneMgr->addCubeSceneNode(1);
	targetCube->setMaterialFlag(EMF_WIREFRAME, true);
	targetCube->setMaterialFlag(EMF_LIGHTING, false);
	targetCube->setVisible(true);

	hotbarSlotSelectTex = IRR->driver->getTexture("tex/InventorySelect.bmp");
	issueCommandModeOn = false;
	issueCommandPlayer = -1;
	issueCommandHotbarSlot = -1;

	HUDYPos = SCREEN_SIZE_Y - 134;
}


void PlayerInterface :: Update()
{
	UpdateCameraRotation();
	UpdateCameraZoom();
	UpdateCameraMovement();
	UpdateCameraPosition();

	UpdateSelectingPlayers();
	UpdateInventoryManagement();
	UpdateHUDInteraction();

	IssueMovementCommands();
	IssueBlockBreakCommands();
	IssuePlaceBlockCommands();
	IssueAttackCommands();

	DrawHUD();
	IRR->DrawText("CameraAngleX: " + IRR->ToString((int)cameraAngle.X), 10, 10, SColor(255,255,255,255));
	IRR->DrawText("CameraAngleY: " + IRR->ToString((int)cameraAngle.Y), 10, 30, SColor(255,255,255,255));
}

void PlayerInterface :: UpdateCameraRotation()
{
	ICursorControl* cursorControl = IRR->device->getCursorControl();
	position2d<f32> cursorPos = cursorControl->getRelativePosition();

	// Begin rotating when middle mouse key is first pressed:
	if(IRR->receiver->key_is_down[KEY_MBUTTON] && !isRotatingCamera)
	{
		cameraStartAngle.X = cameraAngle.X;
		cameraStartAngle.Y = cameraAngle.Y;
		mouseStartPosition.X = cursorPos.X;
		mouseStartPosition.Y = cursorPos.Y;
		isRotatingCamera = true;
	}

	// Rotate camera while middle mouse key is pressed:
	if(isRotatingCamera)
	{
		// Note here that mouse deltaX affects camera angle deltaY and vice versa:
		cameraAngle.Y = cameraStartAngle.Y + (cursorPos.X - mouseStartPosition.X) * 120.0f;
		cameraAngle.X = cameraStartAngle.X + (cursorPos.Y - mouseStartPosition.Y) * 120.0f;

		// Check if middle mouse key has been released:
		if(!IRR->receiver->key_is_down[KEY_MBUTTON])
		{
			isRotatingCamera = false;
		}
	}

	// Cap camera angles:
	if(cameraAngle.X > 90)
		cameraAngle.X = 90;
	else if(cameraAngle.X < 0)
		cameraAngle.X = 0;
}

void PlayerInterface :: UpdateCameraZoom()
{
	cameraDistance -= IRR->receiver->mouse_wheel;

	if(cameraDistance < 5)
		cameraDistance = 5;
	else if(cameraDistance > 100)
		cameraDistance = 60;
}

void PlayerInterface :: UpdateCameraMovement()
{
	bool up =  IRR->receiver->key_is_down[KEY_KEY_W];
	bool down = IRR->receiver->key_is_down[KEY_KEY_S];
	bool left = IRR->receiver->key_is_down[KEY_KEY_A];
	bool right = IRR->receiver->key_is_down[KEY_KEY_D];

	if(up || down || left || right)
	{
		// Get normalized camera heading:
		vector3df camHeading = cameraTarget - IRR->camera->getPosition();
		camHeading.normalize();
		camHeading.Y = 0;

		float moveAngle = 0.0f;

		if(up && !down && !left && !right)			moveAngle = 0.0f;
		else if(!up && down && !left && !right)		moveAngle = 180.0f;
		else if(!up && !down && left && !right)		moveAngle = 270.0f;
		else if(!up && !down && !left && right)		moveAngle = 90.0f;
		else if(up && !down && left && !right)		moveAngle = 315.0f;
		else if(up && !down && !left && right)		moveAngle = 45.0f;
		else if(!up && down && left && !right)		moveAngle = 225.0f;
		else if(!up && down && !left && right)		moveAngle = 135.0f;

		camHeading = IRR->RotateVectorAboutVector(camHeading, vector3df(0,1,0), moveAngle * PI/180.0f);
		cameraTarget += camHeading * 0.3f;
	}
}

void PlayerInterface :: UpdateCameraPosition()
{
	vector3df cameraPosVector(1,0,0);
	cameraPosVector = IRR->RotateVectorAboutVector(cameraPosVector, vector3df(0,0,1), cameraAngle.X * PI/180);
	cameraPosVector = IRR->RotateVectorAboutVector(cameraPosVector, vector3df(0,1,0), cameraAngle.Y * PI/180);

	IRR->camera->setTarget(cameraTarget);
	IRR->camera->setPosition(cameraTarget + cameraPosVector * cameraDistance);
}

void PlayerInterface :: UpdateHUDInteraction()
{
	int selectedPlayer = -1;
	int selectedHotbarSlot = -1;

	bool mouseIsOverHUD = IRR->receiver->mouse_y > HUDYPos;
	bool mouseIsOverPortraits = IRR->receiver->mouse_x < 35;

	for(int x = 0; x < 9; x++)
		if(IRR->receiver->mouse_x > 35 + (x * 32) && IRR->receiver->mouse_x <= 35 + ((x + 1) * 32))
		{
				selectedHotbarSlot = x;
				break;
		}

	for(int y = 0; y < 4; y++)
		if(IRR->receiver->mouse_y > HUDYPos + y * 33 && IRR->receiver->mouse_y < HUDYPos + (y + 1) * 33)
		{
			selectedPlayer = y;
			break;
		}

	if(mouseIsOverHUD && IRR->receiver->key_is_released[KEY_LBUTTON] && selectedPlayer != -1 && selectedHotbarSlot != -1
	&& pcInstances->at(selectedPlayer)->GetInventorySlot(selectedHotbarSlot, INVENTORY_SIZE_Y - 1)->GetItemID() != -1)
	{
		issueCommandModeOn = true;
		issueCommandPlayer = selectedPlayer;
		issueCommandHotbarSlot = selectedHotbarSlot;
		issueCommandItemID = pcInstances->at(selectedPlayer)->GetInventorySlot(selectedHotbarSlot, INVENTORY_SIZE_Y - 1)->GetItemID();

		// Select only the PC for who we are issuing commands:
		selectedPCs.clear();
		selectedPCs.push_back(pcInstances->at(selectedPlayer));
	}

	if(mouseIsOverHUD && mouseIsOverPortraits && IRR->receiver->key_is_released[KEY_LBUTTON])
	{
		if(!IRR->receiver->key_is_down[KEY_LSHIFT])
		{
			selectedPCs.clear();
		}

		selectedPCs.push_back(pcInstances->at(selectedPlayer));
	}
}

void PlayerInterface :: IssueMovementCommands()
{
	if(IRR->receiver->key_is_released[KEY_RBUTTON])
	{
		// Get the position of the players click and store it in openBlock:
		line3df line = IRR->collisionMgr->getRayFromScreenCoordinates(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y));
		vector3di openBlock;
		mapMgr->GetLastOpenBlock(line.start, line.getVector(), 100.0f, openBlock);

		// Send movement command to all selected players:
		for(std::vector<PlayerInstance*>::iterator it = selectedPCs.begin(); it != selectedPCs.end(); it++)
		{
			// Clear player command list:
			(*it)->ClearCommandProcessor();

			(*it)->IssueCommand(new CommandPathToPosition((CharacterInstance*)(*it), vector3df(openBlock.X + 0.5f, openBlock.Y + 0.5f, openBlock.Z + 0.5f)),
				false);
		}
	}
}

void PlayerInterface :: IssueBlockBreakCommands()
{
	if(issueCommandModeOn && IRR->receiver->key_is_released[KEY_LBUTTON] && IRR->receiver->mouse_y < HUDYPos
	&& GManager->itemData[issueCommandItemID].type == MELEE)
	{
		// Get the block of the players click and store it in closedBlock:
		line3df line = IRR->collisionMgr->getRayFromScreenCoordinates(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y));
		vector3di closedBlock;
		mapMgr->GetFirstClosedBlock(line.start, line.getVector(), 100.0f, closedBlock);

		// Get list of adjacent voxels:
		std::vector<vector3di> adjacentVoxels;
		mapMgr->GetAdjacentVoxelsOrdered(pcInstances->at(issueCommandPlayer)->GetPosition(), closedBlock, &adjacentVoxels);

		// Clear player command list:
		if(!IRR->receiver->key_is_down[KEY_LSHIFT])
		{
			pcInstances->at(issueCommandPlayer)->ClearCommandProcessor();
			issueCommandModeOn = false;
		}

		// For now just get path to the closest (by H value) voxel:
		pcInstances->at(issueCommandPlayer)->IssueCommand(
			new CommandPathToPosition((CharacterInstance*)pcInstances->at(issueCommandPlayer), vector3df(adjacentVoxels[0].X + 0.5f, adjacentVoxels[0].Y + 0.5f, adjacentVoxels[0].Z + 0.5f)),
			false);

		// Issue block breaking command:
		pcInstances->at(issueCommandPlayer)->IssueCommand(
			new CommandDestroyBlock( (CharacterInstance*)pcInstances->at(issueCommandPlayer), closedBlock, issueCommandHotbarSlot),
			false);


	}
}

void PlayerInterface :: IssuePlaceBlockCommands()
{
	if(issueCommandModeOn && IRR->receiver->key_is_released[KEY_LBUTTON] && IRR->receiver->mouse_y < HUDYPos
	&& GManager->itemData[issueCommandItemID].type == BLOCK)
	{
		// Get the position of the players click and store it in openBlock:
		line3df line = IRR->collisionMgr->getRayFromScreenCoordinates(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y));
		vector3di openBlock;
		mapMgr->GetLastOpenBlock(line.start, line.getVector(), 100.0f, openBlock);

		// Get list of adjacent voxels:
		std::vector<vector3di> adjacentVoxels;
		mapMgr->GetAdjacentVoxelsOrdered(pcInstances->at(issueCommandPlayer)->GetPosition(), openBlock, &adjacentVoxels);

		// Clear player command list:
		if(!IRR->receiver->key_is_down[KEY_LSHIFT])
		{
			pcInstances->at(issueCommandPlayer)->ClearCommandProcessor();
			issueCommandModeOn = false;
		}

		// For now just get path to the closest (by H value) voxel:
		pcInstances->at(issueCommandPlayer)->IssueCommand(
			new CommandPathToPosition((CharacterInstance*)pcInstances->at(issueCommandPlayer), vector3df(adjacentVoxels[0].X + 0.5f, adjacentVoxels[0].Y + 0.5f, adjacentVoxels[0].Z + 0.5f)),
			false);

		// Issue block placing command:
		pcInstances->at(issueCommandPlayer)->IssueCommand(
			new CommandPlaceBlock( (CharacterInstance*)pcInstances->at(issueCommandPlayer), openBlock, issueCommandHotbarSlot),
			false);


	}
}

void PlayerInterface :: IssueAttackCommands()
{
	if(issueCommandModeOn && IRR->receiver->key_is_released[KEY_LBUTTON] && IRR->receiver->mouse_y < HUDYPos
	&& GManager->itemData[issueCommandItemID].type == PROJECTILE_SHOOTER)
	{
		CharacterInstance* target = 0;

		if( GManager->GetCharacterUnderScreenPos(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y), target) )
		{
			// Send attack command to all selected players:
			for(std::vector<PlayerInstance*>::iterator it = selectedPCs.begin(); it != selectedPCs.end(); it++)
			{
				// Clear player command list:
				(*it)->ClearCommandProcessor();

				(*it)->IssueCommand(new CommandAttackCharacter( (CharacterInstance*)(*it) , target, issueCommandHotbarSlot),
					false);
			}

			issueCommandModeOn = false;
		}
	}
}

void PlayerInterface :: UpdateSelectingPlayers()
{
	// Left click for selecting pc's:
	if(IRR->receiver->key_is_down[KEY_LBUTTON] && interfaceState == INTERFACE_STATE_GAME && !issueCommandModeOn)
	{
		if(!IRR->receiver->key_is_down[KEY_LSHIFT])
		{
			selectedPCs.clear();
		}

		// If the mouse is on the game screen:
		// For all existing player characters:
		for(std::vector<PlayerInstance*>::iterator it = pcInstances->begin(); it != pcInstances->end(); it++)
		{
			// Check if the player is beneath the mouse:
			if((*it)->IsUnderScreenPos(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y)))
			{
				if(!IsPlayerSelected((*it)))
				{
					selectedPCs.push_back(*it);
				}
			}
		}
	}
}

void PlayerInterface :: UpdateInventoryManagement()
{
	if(IRR->receiver->key_is_released[KEY_KEY_E])
	{
		if(interfaceState == INTERFACE_STATE_GAME)
			interfaceState = INTERFACE_STATE_INVENTORY;
		else if(interfaceState == INTERFACE_STATE_INVENTORY)
			interfaceState = INTERFACE_STATE_GAME;
	}

	if(interfaceState == INTERFACE_STATE_INVENTORY && selectedPCs.size() > 0)
	{
		selectedPCs.back()->UpdateInventoryState();
	}
}

void PlayerInterface :: DrawHUD()
{
	int yPos = HUDYPos;
	int pcInstance = 0;

	// For all existing player characters:
	for(std::vector<PlayerInstance*>::iterator it = pcInstances->begin(); it != pcInstances->end(); it++)
	{


		// Draw portrait:
		IRR->driver->draw2DImage( (*it)->GetPortrait(), position2d<s32>(3, yPos),  rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);

		if(IsPlayerSelected((*it)))
		{
			IRR->driver->draw2DImage( hotbarSlotSelectTex, position2d<s32>(3, yPos),  rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);
		}

		// Draw hotbar:
		for(int x = 0; x < INVENTORY_SIZE_X; x++)
		{
			(*it)->GetInventorySlot(x, INVENTORY_SIZE_Y-1)->Draw(true, 35 + x * 32, yPos);

			if(issueCommandModeOn && issueCommandPlayer == pcInstance && issueCommandHotbarSlot == x)
				IRR->driver->draw2DImage(hotbarSlotSelectTex, position2d<s32>(35 + x * 32, yPos),  rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);

		}

		yPos += 33;
		pcInstance++;
	}

	// For selected player characters:
	for(std::vector<PlayerInstance*>::iterator it = selectedPCs.begin(); it != selectedPCs.end(); it++)
	{
		// Identify selected players on game screen with text tag:
		vector2di screenPos = IRR->collisionMgr->getScreenCoordinatesFrom3DPosition( (*it)->GetPosition());
		IRR->DrawText("Selected", screenPos.X, screenPos.Y, SColor(255,255,255,255));

		// Identify selected players on
	}
}

bool PlayerInterface :: IsPlayerSelected(PlayerInstance* pc)
{
	for(std::vector<PlayerInstance*>::iterator it = selectedPCs.begin(); it != selectedPCs.end(); it++)
	{
		if(pc == (*it))
			return true;
	}

	return false;
}

