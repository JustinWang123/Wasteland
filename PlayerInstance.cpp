#include "PlayerInstance.h"
#include "GameManager.h"
#include "Perameters.h"
#include "ItemInstance.h"
#include <iostream>
#include "ZombieInstance.h"
#include "ItemManager.h"
#include "ProjectileWeaponItem.h"
#include "InstanceManager.h"
#include "MessageManager.h"
#include "CharacterInstanceTemplate.h"

// -----------------------------------------------------------------------------------------------
// Constructor and Deconstructor:
// -----------------------------------------------------------------------------------------------
PlayerInstance :: PlayerInstance(CharacterInstanceTemplate* setTemplate, vector3df setPos) :
	CharacterInstance(setTemplate, setPos),
	inventoryInterface(this, &inventory, hotBarSlots, armourSlots)
{
	sceneNode->setVisible(true);

	targetCube = IRR->sceneMgr->addCubeSceneNode(1);
	targetCube->setMaterialFlag(EMF_WIREFRAME, true);
	targetCube->setMaterialFlag(EMF_LIGHTING, false);
	targetCube->setVisible(true);

	objectPlaceNode = IRR->sceneMgr->addMeshSceneNode(ItemMgr->GetMesh(ITEM_SCRAP_HAMMER));
	objectPlaceNode->setScale(vector3df(0.5f, 0.5f, 0.5f));
	objectPlaceNode->setMaterialFlag(EMF_WIREFRAME, true);
	objectPlaceNode->setMaterialFlag(EMF_LIGHTING, false);
	objectPlaceNode->setVisible(false);

	selectedItemNode = IRR->sceneMgr->addMeshSceneNode(ItemMgr->GetMesh(ITEM_SCRAP_HAMMER), IRR->camera);
	selectedItemNode->setVisible(false);
	selectedItemNode->setMaterialFlag(EMF_LIGHTING, false);
	selectedItemNode->setPosition(ItemMgr->GetEquipPos(ITEM_SCRAP_HAMMER));
	selectedItemNode->setRotation(ItemMgr->GetEquipRot(ITEM_SCRAP_HAMMER));
	selectedItemNode->setScale(vector3df(1,1,1) * ItemMgr->GetEquipScale(ITEM_SCRAP_HAMMER));

	aimmerTex = IRR->driver->getTexture("tex/Aimmer.bmp");
	
	inventorySelectTex = IRR->driver->getTexture("tex/InventorySelect.bmp");
	
	IRR->driver->makeColorKeyTexture(inventorySelectTex, video::SColor(255,255,0,255));
	IRR->driver->makeColorKeyTexture(aimmerTex, video::SColor(255,255,0,255));

	yRotation = 0;
	xRotation = 0;
	firstRot = true;

	IRR->camera->bindTargetAndRotation(true);
	IRR->camera->setInputReceiverEnabled(false);

	playerState = PLAYER_STATE_USE_ITEM;
}

PlayerInstance :: ~PlayerInstance()
{

}

// -----------------------------------------------------------------------------------------------
// Update and Update States:
// -----------------------------------------------------------------------------------------------
void PlayerInstance :: Update()
{
	if(playerState == PLAYER_STATE_USE_ITEM)
	{
		UpdateGameState();
	}
	else if(playerState == PLAYER_STATE_BUILD)
	{

	}
	else if(playerState == PLAYER_STATE_INVENTORY)
	{
		UpdateInventoryState();
	}

	DrawGameHUD();
	
	vector3df camPos = position + vector3df(0,PLAYER_HEIGHT,0) + vector3df(lookAtHeading.X, 0, lookAtHeading.Z) * 0.5f;

	IRR->camera->setPosition(camPos);
	IRR->camera->setTarget(camPos + lookAtHeading);

	CharacterInstance::Update();

	

		/*
		// Place targeting cube:
		vector3di openBlock;
		if(MapMgr->GetLastOpenBlock(position, camHeading, 3.0f, openBlock))
		{
			targetCube->setPosition(vector3df(0.5f + openBlock.X, 0.5f + openBlock.Y, 0.5f + openBlock.Z));
		}
		*/

		/*
		// Place Items by first holding down Q to display objectPlaceNode
		if(IRR->receiver->key_is_down[KEY_KEY_Q] && !selectedSlot->IsEmpty())
		{
			if( !objectPlaceNode->isVisible() )
			{
				objectPlaceNode->setVisible(true);
				objectPlaceNode->setMesh( inventorySlots[hotBarSlotSelected][HOT_BAR_Y].GetMesh());
			}

			vector3df pos;

			if( GManager->GetPointOfItemOrMapCollision(position, camHeading, 3.0f, pos))
			{
				objectPlaceNode->setPosition(pos);
				objectPlaceNode->setRotation( vector3df(0, yRotation, 0) );
			}
			else
			{
				objectPlaceNode->setVisible(false);
			}
			
		}
		
		// While Q is down and mouse is clicked, Place currently selected item:
		vector3df pos;
		if(IRR->receiver->key_is_down[KEY_KEY_Q] 
		&& IRR->receiver->key_is_down[KEY_LBUTTON] 
		&& actionDelay <= 0 
		&& !selectedSlot->IsEmpty() 
		&& GManager->GetPointOfItemOrMapCollision(position, camHeading, 3.0f, pos))
		{
			GManager->CreateItemInstance(selectedSlot->GetItem(), pos, vector3df(0, yRotation, 0));
			selectedSlot->RemoveFromSlot(1);
			actionDelay = 20;
		}
		

		vector3df pos;
		// While F is down, display Zombie Mesh for placing:
		if(IRR->receiver->key_is_down[KEY_KEY_F])
		{
			if( !objectPlaceNode->isVisible() )
			{
				objectPlaceNode->setVisible(true);
				objectPlaceNode->setMesh(IRR->sceneMgr->getMesh("mdl/Zombie.irrmesh"));
			}

			vector3df pos;
			if( MapMgr->GetPointOfMapCollision(position, camHeading, 3.0f, pos) )
			{
				objectPlaceNode->setPosition(pos);
				objectPlaceNode->setRotation( vector3df(0, yRotation, 0) );
			}
			else
			{
				objectPlaceNode->setVisible(false);
			}
			
		}
		

		// While F is down and mouse is pressed, place a zombie
		if(IRR->receiver->key_is_down[KEY_KEY_F] 
		&& IRR->receiver->key_is_down[KEY_LBUTTON] 
		&& actionDelay <= 0 
		&& MapMgr->GetPointOfMapCollision(position, camHeading, 3.0f, pos))
		{
			InstanceMgr->CreateZombieInstance(pos);
			actionDelay = 20;
		}

		if(!IRR->receiver->key_is_down[KEY_KEY_Q] && !IRR->receiver->key_is_down[KEY_KEY_F])
		{
			objectPlaceNode->setVisible(false);
		}
	}
	*/
}

void PlayerInstance :: UpdateGameState()
{
	UpdateMovementVelocity();
	UpdateRotation();
	UpdateCamRecoil();
		

	// Use item currently selected in hotbar when LBUTTON is down:
	if(IRR->receiver->key_is_down[KEY_LBUTTON] && actionDelay <= 0)
	{
		UseCurrentlySelectedItem();
	}
		
	// If item currently selected is a projectile weapon then release trigger when LBUTTON is not down
	if(!IRR->receiver->key_is_down[KEY_LBUTTON] && IsProjectileWeaponSelected())
	{
		GetSelectedProjectileWeapon()->ReleaseTrigger();
	}

	// If item currently selected is a projectile weapon and it does not have a full clip and R is pressed then reload
	if(IsProjectileWeaponSelected())
	{
		int ammoInClip = GetSelectedProjectileWeapon()->GetAmmoInClip();
		int clipSize = GetSelectedProjectileWeapon()->GetClipSize();

		if(IRR->receiver->key_is_down[KEY_KEY_R] && ammoInClip < clipSize)
		{
			int ammoItemTemplateID = GetSelectedProjectileWeapon()->GetAmmoItemTemplateID();
			int ammoInInventory = AmountOfItemInInventory(ammoItemTemplateID);

			if(ammoInInventory > (clipSize - ammoInClip) )
			{
				RemoveItemTypeFromInventory(ammoItemTemplateID, clipSize - ammoInClip);
				GetSelectedProjectileWeapon()->Reload(clipSize - ammoInClip);
			}
			else if(ammoInInventory > 0)
			{
				RemoveItemTypeFromInventory(ammoItemTemplateID, ammoInInventory);
				GetSelectedProjectileWeapon()->Reload(ammoInInventory);
			}
		}
	}

	// Open Inventory if not open:
	if(IRR->receiver->key_is_released[KEY_KEY_E])
	{
		playerState = PLAYER_STATE_INVENTORY;
		IRR->device->getCursorControl()->setVisible(true);
	}

	// Inventory methods:
	if(IRR->receiver->key_is_down[KEY_KEY_1])		hotBarSlotSelected = 0;
	else if(IRR->receiver->key_is_down[KEY_KEY_2])	hotBarSlotSelected = 1;
	else if(IRR->receiver->key_is_down[KEY_KEY_3])	hotBarSlotSelected = 2;
	else if(IRR->receiver->key_is_down[KEY_KEY_4])	hotBarSlotSelected = 3;
	else if(IRR->receiver->key_is_down[KEY_KEY_5])	hotBarSlotSelected = 4;
	else if(IRR->receiver->key_is_down[KEY_KEY_6])	hotBarSlotSelected = 5;
	else if(IRR->receiver->key_is_down[KEY_KEY_7])	hotBarSlotSelected = 6;
	else if(IRR->receiver->key_is_down[KEY_KEY_8])	hotBarSlotSelected = 7;
	else if(IRR->receiver->key_is_down[KEY_KEY_9])	hotBarSlotSelected = 8;
}

void PlayerInstance :: UpdateBuildState()
{
	UpdateMovementVelocity();
	UpdateRotation();
	UpdateCamRecoil();	
}

void PlayerInstance :: UpdateInventoryState()
{
	inventoryInterface.Update();
	inventoryInterface.DrawInventory();

	if(IRR->receiver->key_is_released[KEY_KEY_E])
	{
		playerState = PLAYER_STATE_USE_ITEM;
		IRR->device->getCursorControl()->setVisible(false);
	}
}

// -----------------------------------------------------------------------------------------------
// Update Sub Routines:
// -----------------------------------------------------------------------------------------------
void PlayerInstance :: UpdateMovementVelocity()
{
	bool up =  IRR->receiver->key_is_down[KEY_KEY_W];
	bool down = IRR->receiver->key_is_down[KEY_KEY_S];
	bool left = IRR->receiver->key_is_down[KEY_KEY_A];
	bool right = IRR->receiver->key_is_down[KEY_KEY_D];

	movementDir = vector3df(0,0,0);

	if(up && !down && !left && !right)			movementDir = heading;
	else if(!up && down && !left && !right)		movementDir = -heading;
	else if(!up && !down && left && !right)		movementDir = -tangent;
	else if(!up && !down && !left && right)		movementDir = tangent;
	else if(up && !down && left && !right)		movementDir = heading - tangent;
	else if(up && !down && !left && right)		movementDir = heading + tangent;
	else if(!up && down && left && !right)		movementDir = -heading - tangent;
	else if(!up && down && !left && right)		movementDir = -heading + tangent;

	movementDir.normalize();

	if(IRR->receiver->key_is_down[KEY_LSHIFT])
		isRunning = true;
	else
		isRunning = false;
}

void PlayerInstance :: UpdateRotation()
{
	ICursorControl* cursorControl = IRR->device->getCursorControl();
	position2d<f32> cursorPos = cursorControl->getRelativePosition();
	static float rotateSpeed = 4.0f;

	if(firstRot) // it checks if it is the first time camera rotates and than fill tables with proper values
	{
		rotX[0] = rotX[1] = 0.5f - cursorPos.X;
		rotY[0] = rotY[1] = 0.5f - cursorPos.Y;
		firstRot = false;
	}
      
	// puts values from this frame into table
	rotX[2] = 0.5f - cursorPos.X;
	rotY[2] = 0.5f - cursorPos.Y; 

	// If any change in cursorPos this frame:
	if (cursorPos.X < 0.5 || cursorPos.X > 0.5 || cursorPos.Y < 0.5 || cursorPos.Y > 0.5 )
	{
		// Rotate Y Axis:
		yRotation -= (rotX[0]+rotX[1]+rotX[2])/3 * rotateSpeed;

		// Rotate X Axis:
		xRotation -= (rotY[0]+rotY[1]+rotY[2])/3 * rotateSpeed;

		// Restrict X Axis:
		if(xRotation > PI/2.1f) xRotation = PI/2.1f;
		if(xRotation < -PI/2.1f) xRotation = -PI/2.1f;
		
		cursorControl->setPosition(0.5f, 0.5f);
      
		rotX[0] = rotX[1]; // finaly we move values to make room for a new ones
		rotX[1] = rotX[2];
      
		rotY[0] = rotY[1];
		rotY[1] = rotY[2];
	}

	
	lookAtHeading = IRR->RotateVectorAboutVector(vector3df(1,0,0), vector3df(0,1,0), yRotation);

	vector3df newTangent = lookAtHeading.crossProduct(vector3df(0,-1,0));
	newTangent.Y = 0;
	newTangent.normalize();

	lookAtHeading = IRR->RotateVectorAboutVector(lookAtHeading, newTangent, xRotation); // + camRecoil.X
	camUp = lookAtHeading.crossProduct(newTangent);

	SetHeading(vector3df(lookAtHeading.X, 0, lookAtHeading.Z)); 
}

// -----------------------------------------------------------------------------------------------
// Message Handling:
// -----------------------------------------------------------------------------------------------
bool PlayerInstance :: HandleMessage(Telegram& msg)
{
	if(msg.msg == MESSAGE_TYPE_WEAPON_RECOIL)
	{
		desiredCamRecoil.X += ((vector2df*)msg.extraInfo)->X;
		desiredCamRecoil.Y += ((vector2df*)msg.extraInfo)->Y;
	}

	// Pass any unhandled messages up:
	if(CharacterInstance::HandleMessage(msg))
	{
		return true;
	}

	return false;
}

// -----------------------------------------------------------------------------------------------
// HUD:
// -----------------------------------------------------------------------------------------------
void PlayerInstance :: DrawGameHUD()
{
	// Draw Debug info and HUD:
	IRR->driver->draw2DImage(aimmerTex, position2d<s32>(SCREEN_SIZE_X/2 - 2, SCREEN_SIZE_Y/2 -2), rect<s32>(0,0,4,4), 0, SColor(255,255,255,255), true);
	IRR->DrawText("X: " + IRR->ToString((int)position.X), 10, 10, SColor(255,255,255,255));
	IRR->DrawText("Y: " + IRR->ToString((int)position.Y), 10, 30, SColor(255,255,255,255));
	IRR->DrawText("Z: " + IRR->ToString((int)position.Z), 10, 50, SColor(255,255,255,255));
	IRR->DrawText("ActionDelay: " + IRR->ToString(actionDelay), 10, 70, SColor(255,255,255,255));
	IRR->DrawText("HP: " + IRR->ToString(currentHealth) + "/" + IRR->ToString(pTemplate->maxHealth), 10, 90, SColor(255,255,255,255));
	IRR->DrawText("Food: " + IRR->ToString(currentFood) + "/" + IRR->ToString(pTemplate->maxFood), 10, 110, SColor(255,255,255,255));
	IRR->DrawText("Time: " + IRR->ToString(GManager->GetTime()), 10, 130, SColor(255,255,255,255));

	if(IsProjectileWeaponSelected())
	{
		IRR->DrawText("Ammo: " + IRR->ToString(GetSelectedProjectileWeapon()->GetAmmoInClip()) + "/" + IRR->ToString(GetSelectedProjectileWeapon()->GetClipSize()), 10, 150, SColor(255,255,255,255));
	}

	inventoryInterface.DrawHotBarSlots();
	DrawSelectedItem();
	IRR->driver->draw2DImage(inventorySelectTex, position2d<s32>(hotBarSlotSelected * 32, SCREEN_SIZE_Y - 32), rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);
	
	int chatWindowSize;

	if(chatWindow.size() < 5)
		chatWindowSize = chatWindow.size();
	else
		chatWindowSize = 5;

	for(int i = 0; i < chatWindowSize; i++)
	{
		IRR->DrawText(chatWindow[i], 10, 580 - i * 20, SColor(255,255,255,255));
	}
}

void PlayerInstance :: DrawSelectedItem()
{
	
	// Set SceneNode of currently held item:
	if(!hotBarSlots[hotBarSlotSelected].IsEmpty())
	{
		if(hotBarSlots[hotBarSlotSelected].GetMesh() != selectedItemNode->getMesh())
		{
			selectedItemNode->setMesh(hotBarSlots[hotBarSlotSelected].GetMesh());
			selectedItemNode->setMaterialFlag(EMF_LIGHTING, false);
			selectedItemNode->setPosition(hotBarSlots[hotBarSlotSelected].GetEquipPos());
			selectedItemNode->setRotation(hotBarSlots[hotBarSlotSelected].GetEquipPos());
			selectedItemNode->setScale(vector3df(1,1,1) * hotBarSlots[hotBarSlotSelected].GetEquipScale());
		}

		selectedItemNode->setVisible(true);
	}
	else
	{
		selectedItemNode->setVisible(false);
	}
	
}

// -----------------------------------------------------------------------------------------------
// Misc:
// -----------------------------------------------------------------------------------------------
void PlayerInstance :: UseItem()
{
	/*
	if(!selectedSlot->IsEmpty() && selectedSlot->GetType() == ITEM_TYPE_PROJECTILE_WEAPON)
	{
		((ProjectileWeaponItem*)selectedSlot->GetItem())->Fire(position, camHeading, this);
			
	}
	*/
	/*
	if(selectedSlot->IsEmpty())
	{
		
	}
	// Placing Blocks:
	else if(GManager->itemData[hotBarSlotSelectedItemID].type == BLOCK)
	{
		vector3di openBlock;

		if(!IRR->receiver->key_is_down[KEY_KEY_R] && MapMgr->GetLastOpenBlock(position, camHeading, 3.0f, openBlock))
		{
			MapMgr->BuildBlock(openBlock, GManager->itemData[hotBarSlotSelectedItemID].data[0]);

			// Only remove items from inventory if not in creative mode:
			if(!GManager->IsCreativeModeOn())
			{
				inventorySlots[hotBarSlotSelected][6].RemoveFromSlot(1);
			}

			actionDelay = PLACE_BLOCK_DEALY;
		}
		else if(IRR->receiver->key_is_down[KEY_KEY_R] && GManager->IsCreativeModeOn())
		{
			vector3di openBlock;
			MapMgr->GetLastOpenBlock(position, camHeading, 3.0f, openBlock);
			
			MapMgr->BuildLineOfBlocks(
				GManager->itemData[hotBarSlotSelectedItemID].data[0],
				vector3df(openBlock.X + 0.5f, openBlock.Y + 0.5f, openBlock.Z + 0.5f),
				vector3df(camHeading.X, 0, camHeading.Z),
				100.0f);

			
			actionDelay = PLACE_BLOCK_DEALY;
		}
	}
	// Using Tools / Weapons:
	else if(GManager->itemData[hotBarSlotSelectedItemID].type == MELEE)
	{
		// Deal damage based on block type:
		vector3di closedBlock;

		if(MapMgr->GetFirstClosedBlock(position, camHeading, 3.0f, closedBlock))
		{
			DamageBlock(closedBlock, hotBarSlotSelected);
		}
	}
	// Shooting Guns:
	else if(GManager->itemData[hotBarSlotSelectedItemID].type == PROJECTILE_SHOOTER)
	{
		// Set action delay to weapon/tool delay in data[10]:
		actionDelay = GManager->itemData[hotBarSlotSelectedItemID].data[10];
		inventorySlots[hotBarSlotSelected][6].DamageItem(1); // Deal Damage to weapon:

		GManager->AddProjectile(position, camHeading, this);
	}
	// Eatting Food:
	else if(GManager->itemData[hotBarSlotSelectedItemID].type == FOOD)
	{
		actionDelay = EAT_FOOD_TIME;

		EatFood(GManager->itemData[hotBarSlotSelectedItemID].data[0]);
		
		inventorySlots[hotBarSlotSelected][6].RemoveFromSlot(1);
	}
	else if(GManager->itemData[hotBarSlotSelectedItemID].type == MEDICAL)
	{
		actionDelay = USE_MEDICAL_TIME;

		HealHealth(GManager->itemData[hotBarSlotSelectedItemID].data[0]);
		
		inventorySlots[hotBarSlotSelected][6].RemoveFromSlot(1);
	}
	*/
}
