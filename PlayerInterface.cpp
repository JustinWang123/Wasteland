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
	IRR->device->getCursorControl()->setVisible(false);

	interfaceState = INTERFACE_STATE_GAME;

	targetCube = IRR->sceneMgr->addCubeSceneNode(1);
	targetCube->setMaterialFlag(EMF_WIREFRAME, true);
	targetCube->setMaterialFlag(EMF_LIGHTING, false);
	targetCube->setVisible(true);
}


void PlayerInterface :: Update()
{
	
	UpdateCamera();
	UpdateMovement();
	UpdateInventoryManagement();
	UpdateHUDInteraction();

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


void PlayerInterface :: UpdateInventoryManagement()
{
	if(IRR->receiver->key_is_released[KEY_KEY_E])
	{
		if(interfaceState == INTERFACE_STATE_GAME)
			interfaceState = INTERFACE_STATE_INVENTORY;
		else if(interfaceState == INTERFACE_STATE_INVENTORY)
			interfaceState = INTERFACE_STATE_GAME;
	}
	
	if(interfaceState == INTERFACE_STATE_INVENTORY)
	{
		playerInstance->UpdateInventoryState();
	}
}

void PlayerInterface :: DrawHUD()
{

}
