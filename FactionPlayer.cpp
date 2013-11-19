#include "FactionPlayer.h"
#include "CharacterInstance.h"
#include "MapManager.h"
#include "CharacterInstanceSoldier.h"
#include "InstanceManager.h"

#include "CommandPathToPosition.h"
#include "CommandAttackCharacter.h"
#include "CommandPickUpItem.h"
#include "CommandHarvestBlock.h"

FactionPlayer :: FactionPlayer()
{
    // Init Camera Members:
    cameraTarget = vector3df(175,22,60);
	cameraAngle.Y = 0.0f;
	cameraAngle.X = 45.0f;
	cameraDistance = 10.0f;
	isRotatingCamera = false;

    // Interface:
    interfaceState = PLAYER_FACTION_INTERFACE_STATE_GAME;
    leftClickFunction = LEFT_CLICK_FUNCTION_SELECT_SOLDIER;
	standButton.Init(0, SCREEN_SIZE_Y - 32, IRR->driver->getTexture("tex/StandButtonUp.bmp"), IRR->driver->getTexture("tex/StandButtonOver.bmp"), IRR->driver->getTexture("tex/StandButtonDown.bmp"));
	crouchButton.Init(32, SCREEN_SIZE_Y - 32, IRR->driver->getTexture("tex/CrouchButtonUp.bmp"), IRR->driver->getTexture("tex/CrouchButtonOver.bmp"), IRR->driver->getTexture("tex/CrouchButtonDown.bmp"));
	proneButton.Init(64, SCREEN_SIZE_Y - 32, IRR->driver->getTexture("tex/ProneButtonUp.bmp"), IRR->driver->getTexture("tex/ProneButtonOver.bmp"), IRR->driver->getTexture("tex/ProneButtonDown.bmp"));
	harvestButton.Init(SCREEN_SIZE_X - 32, SCREEN_SIZE_Y - 32, IRR->driver->getTexture("tex/HarvestButtonUp.bmp"), IRR->driver->getTexture("tex/HarvestButtonOver.bmp"), IRR->driver->getTexture("tex/HarvestButtonDown.bmp"));

	// Character Select SceneNode:
    for(unsigned short i = 0; i < FACTION_PLAYER_MAX_SOLDIERS; i++)
    {
		characterSelectSceneNode[i] = IRR->sceneMgr->addMeshSceneNode(IRR->sceneMgr->getMesh("mdl/Plane.obj"), 0, 0);
		characterSelectSceneNode[i]->setScale(vector3df(0.75f ,0.75f ,0.75f));
		characterSelectSceneNode[i]->setMaterialTexture(0, IRR->driver->getTexture("tex/CharacterSelect.png"));
		characterSelectSceneNode[i]->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
		characterSelectSceneNode[i]->setMaterialFlag(EMF_LIGHTING, false);
		characterSelectSceneNode[i]->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
		characterSelectSceneNode[i]->setMaterialFlag(EMF_BILINEAR_FILTER, false);
		characterSelectSceneNode[i]->setMaterialFlag(EMF_USE_MIP_MAPS, false);
	}

	emptySlotTex = IRR->driver->getTexture("tex/InventorySlot.bmp");
	soldierInventoryInterface.SetInterfacePosition(vector2di(96,32), vector2di(96, SCREEN_SIZE_Y - 32));


    // Debug:
    debugSceneNode = IRR->sceneMgr->addCubeSceneNode(1);
	debugSceneNode->setMaterialFlag(EMF_WIREFRAME, true);
	debugSceneNode->setMaterialFlag(EMF_LIGHTING, false);
	debugSceneNode->setVisible(true);

	IRR->device->getCursorControl()->setVisible(true);
}

FactionPlayer :: ~FactionPlayer()
{
}

void FactionPlayer :: Update()
{
    if(interfaceState == PLAYER_FACTION_INTERFACE_STATE_GAME)
    {
        UpdateGameState();
    }
    else
    {
        UpdateInventoryState();
    }
}

// ------------------------------------------------------------------------
// States:
// ------------------------------------------------------------------------
void FactionPlayer :: UpdateGameState()
{
    UpdateCameraRotation();
	UpdateCameraZoom();
	UpdateCameraMovement();
	UpdateCameraPosition();
	UpdateLeftClickFunction();
    IssueCommands();
	harvestButton.Draw();
	harvestButton.Update();

	IRR->DrawText(IRR->ToString((int)commandQueue.size()), 200,200);


	if(harvestButton.IsClicked())
	{
		leftClickFunction = LEFT_CLICK_FUNCTION_HARVEST_BLOCK;
	}

	// Hide all character select SceneNodes (only make them visible if they are used)
	for(unsigned short i = 0; i < FACTION_PLAYER_MAX_SOLDIERS; i++)
    {
		characterSelectSceneNode[i]->setVisible(false);
	}

	// Single solider selected:
    if(selectedSoldiers.size() == 1)
    {
		UpdateInterfaceForSingleSelection();
        DrawInterfaceForSingleSelection();
    }

	// Single or multiple soliders selected:
    if(selectedSoldiers.size() >= 1)
    {
		UpdateInterfaceForMultipleSelection();
		DrawInterfaceForMultipleSelection();
	}
}

void FactionPlayer :: DrawInterfaceForSingleSelection()
{
	soldierInventoryInterface.DrawHotbarSlots();
}

void FactionPlayer :: DrawInterfaceForMultipleSelection()
{
	standButton.Draw();
	crouchButton.Draw();
	proneButton.Draw();

	// Draw selected soldiers portraits in top left corner:
	int yPos = 0;
	for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
	{
        IRR->DrawTexture( (*it)->GetPortrait(), 0, yPos);
        BaseItem* selectedItem = (*it)->GetSelectedItem();

        if(selectedItem != 0)
			IRR->DrawTexture(selectedItem->GetInvTex(), 32, yPos);
		else
			IRR->DrawTexture(emptySlotTex, 32, yPos);

        yPos += 32;
	}

	// Draw character select SceneNode:
	int i = 0;
	for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
	{
		characterSelectSceneNode[i]->setPosition((*it)->GetPosition() + vector3df(0, 0.1f, 0));
		characterSelectSceneNode[i]->setVisible(true);
		i++;
	}
}

void FactionPlayer :: UpdateInterfaceForSingleSelection()
{
	// Change State to Inventory:
	if(IRR->receiver->key_is_released[KEY_KEY_I])
	{
		interfaceState = PLAYER_FACTION_INTERFACE_STATE_INVENTORY;
	}

	soldierInventoryInterface.UpdateHotbarSelect();
}

void FactionPlayer :: UpdateInterfaceForMultipleSelection()
{
	standButton.Update();
	crouchButton.Update();
	proneButton.Update();

	// Stand Button:
	if(standButton.IsClicked())
	{
		for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
		{
			(*it)->ChangeStance(STANCE_STANDING);
		}
	}
	// Crouch Button:
	else if(crouchButton.IsClicked())
	{
		for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
		{
			(*it)->ChangeStance(STANCE_CROUCHING);
		}
	}
	// Prone Button:
	else if(proneButton.IsClicked())
	{
		for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
		{
			(*it)->ChangeStance(STANCE_PRONE);
		}
	}
}


void FactionPlayer :: UpdateInventoryState()
{
    // Note: when in inventory state only a single soldier is selected:
    soldierInventoryInterface.UpdateInventoryManagement();
    soldierInventoryInterface.DrawInventoryList();
    soldierInventoryInterface.DrawHotbarSlots() ;
	soldierInventoryInterface.DrawMouseSlot();

    // Change State to game:
	if(IRR->receiver->key_is_released[KEY_KEY_I])
    {
        interfaceState = PLAYER_FACTION_INTERFACE_STATE_GAME;
    }
}

// ------------------------------------------------------------------------
// Update Camera:
// ------------------------------------------------------------------------
void FactionPlayer :: UpdateCameraRotation()
{
	ICursorControl* cursorControl = IRR->device->getCursorControl();
	position2d<f32> cursorPos = cursorControl->getRelativePosition();

	// Begin rotating when middle mouse key is first pressed:
	if(IRR->receiver->key_is_down[KEY_MBUTTON] && !isRotatingCamera && IsMouseOnGameScreen())
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

void FactionPlayer :: UpdateCameraZoom()
{
    if(IsMouseOnGameScreen())
    {
        cameraDistance -= IRR->receiver->mouse_wheel;

        if(cameraDistance < 5)
            cameraDistance = 5;
        else if(cameraDistance > 100)
            cameraDistance = 60;
    }
}

void FactionPlayer :: UpdateCameraMovement()
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

void FactionPlayer :: UpdateCameraPosition()
{
	vector3df cameraPosVector(1,0,0);
	cameraPosVector = IRR->RotateVectorAboutVector(cameraPosVector, vector3df(0,0,1), cameraAngle.X * PI/180);
	cameraPosVector = IRR->RotateVectorAboutVector(cameraPosVector, vector3df(0,1,0), cameraAngle.Y * PI/180);

	IRR->camera->setTarget(cameraTarget);
	IRR->camera->setPosition(cameraTarget + cameraPosVector * cameraDistance);
}

// ------------------------------------------------------------------------
// Soldiers:
// ------------------------------------------------------------------------
void FactionPlayer :: UpdateLeftClickFunction()
{
	// Left click
	if(IRR->receiver->key_is_down[KEY_LBUTTON]
	&& IsMouseOnGameScreen()
	&& interfaceState == PLAYER_FACTION_INTERFACE_STATE_GAME)
	{
		if(leftClickFunction == LEFT_CLICK_FUNCTION_SELECT_SOLDIER)
		{
			//for selecting pc's:
			// Holding shift allows multiple soldiers to be selected:
			if(!IRR->receiver->key_is_down[KEY_LSHIFT])
			{
				selectedSoldiers.clear();
			}

			// Add the first soldiers which is under the mouse to the selectedSoldiers list:
			for(std::list<CharacterInstanceSoldier*>::iterator it = soldierInstances.begin(); it != soldierInstances.end(); it++)
				if((*it)->IsUnderScreenPos(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y))
				&& !IsSoldierSelected((*it)))
				{
					selectedSoldiers.push_back(*it);
					soldierInventoryInterface.SetSoldier(*it);
					break;
				}
		}
		else if(leftClickFunction == LEFT_CLICK_FUNCTION_HARVEST_BLOCK)
		{
			line3df line = IRR->collisionMgr->getRayFromScreenCoordinates(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y));

			int instanceID;
			vector3df impactPos;
			InstanceMgr->IsRaySolid(line.start, line.getVector(), 100.0f, BITMASK_WORLD, instanceID, impactPos);
			debugSceneNode->setPosition(vector3df((int)impactPos.X + 0.5f, (int)impactPos.Y + 0.5f, (int)impactPos.Z + 0.5f));

			AddCommand(new CommandHarvestBlock(vector3di(impactPos.X, impactPos.Y, impactPos.Z)));
		}
	}
}

void FactionPlayer :: IssueCommands()
{
    if(IRR->receiver->key_is_released[KEY_RBUTTON]
	&& IsMouseOnGameScreen())
	{
        line3df line = IRR->collisionMgr->getRayFromScreenCoordinates(vector2di(IRR->receiver->mouse_x, IRR->receiver->mouse_y));

        int instanceID;
        vector3df impactPos;

        InstanceMgr->IsRaySolid(line.start, line.getVector(), 100.0f, BITMASK_CHARACTER | BITMASK_WORLD | BITMASK_ITEM, instanceID, impactPos);

        if(InstanceMgr->GetInstanceFromID(instanceID)->GetBitMask() == BITMASK_CHARACTER)
        {
            // Send attack command to all selected soldiers:
            for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
            {
                (*it)->ClearBrain();
                (*it)->IssueCommand(new CommandAttackCharacter((CharacterInstance*)(*it), (CharacterInstance*)InstanceMgr->GetInstanceFromID(instanceID)));
            }
        }
        else if(InstanceMgr->GetInstanceFromID(instanceID)->GetBitMask() == BITMASK_ITEM)
        {
			// Send pick up item to first soldier on list:
			(*selectedSoldiers.begin())->ClearBrain();
			(*selectedSoldiers.begin())->IssueCommand(new CommandPickUpItem((CharacterInstance*)(*selectedSoldiers.begin()), (ItemInstance*)InstanceMgr->GetInstanceFromID(instanceID)));

        }
        else if(InstanceMgr->GetInstanceFromID(instanceID)->GetBitMask() == BITMASK_WORLD)
        {
            // Send movement command to all selected soldiers:
            for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
            {
                (*it)->ClearBrain();
                (*it)->IssueCommand(new CommandPathToPosition((CharacterInstance*)(*it), impactPos + vector3df(0,0.5,0)));
            }
        }
	}

}

bool FactionPlayer :: IsSoldierSelected(CharacterInstanceSoldier* soldier)
{
    for(std::list<CharacterInstanceSoldier*>::iterator it = selectedSoldiers.begin(); it != selectedSoldiers.end(); it++)
	{
		if(soldier == (*it))
			return true;
	}

	return false;
}

// ------------------------------------------------------------------------
// Soldiers:
// ------------------------------------------------------------------------
bool FactionPlayer :: IsMouseOnGameScreen()
{
    return IRR->receiver->mouse_y < (SCREEN_SIZE_Y - 32);
}

