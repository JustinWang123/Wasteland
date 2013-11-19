#include "PlayerControlledInstance.h"
#include "GameManager.h"
#include "Perameters.h"
#include "ItemInstance.h"
#include <iostream>
#include "ZombieInstance.h"
#include "FurnaceItemInstance.h"
#include "CraftingTableInstance.h"
#include "CharacterCommand.h"
#include "CommandProcessor.h"



PlayerControlledInstance :: PlayerControlledInstance(vector3df setPos)
{
	this->position = setPos;

	sceneNode = IRR->sceneMgr->addMeshSceneNode(IRR->sceneMgr->getMesh("mdl/Zombie.irrmesh"), 0, 2);
	sceneNode->setMaterialFlag(EMF_LIGHTING, false);
	sceneNode->setPosition(setPos);

	portrait = IRR->driver->getTexture("tex/PlayerPortrait.bmp");

	// Collision Triangle Selector for collision:
	ITriangleSelector* collisionSelector = IRR->sceneMgr->createTriangleSelector(sceneNode->getMesh(), sceneNode);
    sceneNode->setTriangleSelector(collisionSelector);
	collisionSelector->drop();

	verticalVelocity = 0;
	decreaseFoodTime = IRR->GetTime() + DECREASE_FOOD_TIME;
	currentFood = 100;
	maxFood = 100;
	armourClass = 0;
	hotBarSelected = 0;
	actionDelay = 0;
	removeMe = false;
	maxSpeed = 0.1f;
	this->instanceID = instanceID;

	inventorySelectTex = IRR->driver->getTexture("tex/InventorySelect.bmp");
	IRR->driver->makeColorKeyTexture(inventorySelectTex, video::SColor(255,255,0,255));


	// Setup command stuff:
	commandProcessor = new CommandProcessor(this);
	isMoving = false;
	destination = vector3df(0,0,0);
}

PlayerControlledInstance :: ~PlayerControlledInstance()
{

}

void PlayerControlledInstance :: Update()
{

	UpdateRotation();
	UpdateVelocity();
	UpdatePosition();
	UpdateFood();

	commandProcessor->Process();

	if(actionDelay > 0)
		actionDelay--;

	sceneNode->setPosition(position);

	// HUD stuff:
	/*
	DrawHotBar();
	DrawMouseSlot();
	*/
}

bool PlayerControlledInstance :: HandleMessage(Telegram& msg)
{
	if(commandProcessor->HandleMessage(msg))
	{
		return true;
	}

	return false;
}

void PlayerControlledInstance :: UpdateInventoryState()
{
	UpdateInventory();
	UpdateCrafting();
	UpdateEquipment();

	DrawHotBar();
	DrawInventory();
	DrawCrafting();
	DrawMouseSlot();
}

void PlayerControlledInstance :: UpdateRotation()
{

}

void PlayerControlledInstance :: UpdateVelocity()
{
	if(isMoving)
	{
		vector3df movementVector = destination - position;
		movementVector.normalize();

		velocity = movementVector * moveSpeed;
	}
}

void PlayerControlledInstance :: UpdatePosition()
{
	if(velocity.getLengthSQ() > 0.00001f)
	{
		vector3df velocityNormal = velocity;
		velocityNormal.normalize();
		velocityNormal *= 0.5;


		if(!mapMgr->IsBlockSolid(position + vector3df(0, 0.5f, 0) + velocityNormal + velocity)
		&& !mapMgr->IsBlockSolid(position + vector3df(0, 1.5f, 0) + velocityNormal + velocity))
		{
			// verticalVelocity needs to go back in some time:
			position += vector3df(velocity.X, velocity.Y, velocity.Z);
		}

	}
}

void PlayerControlledInstance :: UpdateInventory()
{
	if(IRR->receiver->key_is_released[KEY_LBUTTON])
	{
		vector2di slot = GetSlot();
		if(slot != vector2di(-1,-1))
		{
			inventorySlots[slot.X][slot.Y].ExhangeWithMouse(mouseSlot);
		}
	}
}

void PlayerControlledInstance :: UpdateCrafting()
{
	if(IRR->receiver->key_is_released[KEY_LBUTTON])
	{
		if(inventoryCraftingTable->IsMouseOverCrafting())
		{
			vector2di slot = inventoryCraftingTable->GetCraftingSlotMouseOver();
			inventoryCraftingTable->craftingSlots[slot.X][slot.Y].ExhangeWithMouse(mouseSlot);
			inventoryCraftingTable->UpdateRecipes();
		}
		else if(inventoryCraftingTable->IsMouseOverTools())
		{
			int slot = inventoryCraftingTable->GetToolSlotMouseOver();
			inventoryCraftingTable->toolSlots[slot].ExhangeWithMouse(mouseSlot);
			inventoryCraftingTable->UpdateRecipes();
		}
		else if(inventoryCraftingTable->IsMouseOverRecipes() && inventoryCraftingTable->GetRecipeMouseOver() != -1 && (mouseSlot.GetItemID() == -1
		|| (mouseSlot.GetItemID() == GManager->recipeData[inventoryCraftingTable->GetRecipeMouseOver()].resultID && mouseSlot.GetItemAmount() + GManager->recipeData[inventoryCraftingTable->GetRecipeMouseOver()].resultAmount <= 64)))
		{
			inventoryCraftingTable->CraftRecipe(inventoryCraftingTable->GetRecipeMouseOver(), mouseSlot);
			inventoryCraftingTable->UpdateRecipes();
		}
	}

	if(IRR->receiver->key_is_down[KEY_LBUTTON]
	&& IRR->receiver->mouse_x > 508 && IRR->receiver->mouse_x < 524
	&& IRR->receiver->mouse_y > 112 && IRR->receiver->mouse_y < 128
	&& !inventoryCraftingTable->recipeUpArrowPressed)
	{
		inventoryCraftingTable->recipeUpArrowPressed = true;
	}

	if(inventoryCraftingTable->recipeUpArrowPressed &&
	(!IRR->receiver->key_is_down[KEY_LBUTTON]
	|| IRR->receiver->mouse_x < 508 || IRR->receiver->mouse_x > 524
	|| IRR->receiver->mouse_y < 112 || IRR->receiver->mouse_y > 128))
	{
		inventoryCraftingTable->recipeUpArrowPressed = false;

		if(inventoryCraftingTable->recipeIndex > 0)
			inventoryCraftingTable->recipeIndex--;
	}

	if(IRR->receiver->key_is_down[KEY_LBUTTON]
	&& IRR->receiver->mouse_x > 508 && IRR->receiver->mouse_x < 524
	&& IRR->receiver->mouse_y > 160 && IRR->receiver->mouse_y < 176
	&& !inventoryCraftingTable->recipeDownArrowPressed)
	{
		inventoryCraftingTable->recipeDownArrowPressed = true;
	}

	if(inventoryCraftingTable->recipeDownArrowPressed &&
	(!IRR->receiver->key_is_down[KEY_LBUTTON]
	|| IRR->receiver->mouse_x < 508 || IRR->receiver->mouse_x > 524
	|| IRR->receiver->mouse_y < 160 || IRR->receiver->mouse_y > 176))
	{
		inventoryCraftingTable->recipeDownArrowPressed = false;

		if(inventoryCraftingTable->recipeIndex < 20)
			inventoryCraftingTable->recipeIndex++;
	}

}

void PlayerControlledInstance :: UpdateCraftingTable()
{
	/*
	CraftingTableInstance* craftingTable = ((CraftingTableInstance*)(*craftingTableIt));

	if(IRR->receiver->key_is_released[KEY_LBUTTON])
	{
		if(craftingTable->IsMouseOverCrafting())
		{
			vector2di slot = craftingTable->GetCraftingSlotMouseOver();
			craftingTable->craftingSlots[slot.X][slot.Y].ExhangeWithMouse(mouseSlot);
			craftingTable->UpdateRecipes();
		}
		else if(craftingTable->IsMouseOverTools())
		{
			int slot = craftingTable->GetToolSlotMouseOver();
			craftingTable->toolSlots[slot].ExhangeWithMouse(mouseSlot);
			craftingTable->UpdateRecipes();
		}
		else if(craftingTable->IsMouseOverRecipes() && craftingTable->GetRecipeMouseOver() != -1 && (mouseSlot.GetItemID() == -1
		|| (mouseSlot.GetItemID() == GManager->recipeData[craftingTable->GetRecipeMouseOver()].resultID && mouseSlot.GetItemAmount() + GManager->recipeData[craftingTable->GetRecipeMouseOver()].resultAmount <= 64)))
		{
			craftingTable->CraftRecipe(craftingTable->GetRecipeMouseOver(), mouseSlot);
			craftingTable->UpdateRecipes();
		}
	}

	if(IRR->receiver->key_is_down[KEY_LBUTTON]
	&& IRR->receiver->mouse_x > 508 && IRR->receiver->mouse_x < 524
	&& IRR->receiver->mouse_y > 112 && IRR->receiver->mouse_y < 128
	&& !craftingTable->recipeUpArrowPressed)
	{
		craftingTable->recipeUpArrowPressed = true;
	}

	if(craftingTable->recipeUpArrowPressed &&
	(!IRR->receiver->key_is_down[KEY_LBUTTON]
	|| IRR->receiver->mouse_x < 508 || IRR->receiver->mouse_x > 524
	|| IRR->receiver->mouse_y < 112 || IRR->receiver->mouse_y > 128))
	{
		craftingTable->recipeUpArrowPressed = false;

		if(craftingTable->recipeIndex > 0)
			craftingTable->recipeIndex--;
	}

	if(IRR->receiver->key_is_down[KEY_LBUTTON]
	&& IRR->receiver->mouse_x > 508 && IRR->receiver->mouse_x < 524
	&& IRR->receiver->mouse_y > 160 && IRR->receiver->mouse_y < 176
	&& !craftingTable->recipeDownArrowPressed)
	{
		craftingTable->recipeDownArrowPressed = true;
	}

	if(craftingTable->recipeDownArrowPressed &&
	(!IRR->receiver->key_is_down[KEY_LBUTTON]
	|| IRR->receiver->mouse_x < 508 || IRR->receiver->mouse_x > 524
	|| IRR->receiver->mouse_y < 160 || IRR->receiver->mouse_y > 176))
	{
		craftingTable->recipeDownArrowPressed = false;

		if(craftingTable->recipeIndex < 20)
			craftingTable->recipeIndex++;
	}
	*/
}

void PlayerControlledInstance :: UpdateFurnace()
{
	FurnaceItemInstance* furnace = ((FurnaceItemInstance*)(*furnaceIt));

	// Dipositing fuel into fuel slot:
	if(IRR->receiver->key_is_released[KEY_LBUTTON]
	&& mouseSlot.GetItemID() != -1
	&& furnace->fuelSlot.IsMouseOver()
	&& GManager->itemData[mouseSlot.GetItemID()].fuelValue != 0)
	{
		furnace->fuel += GManager->itemData[mouseSlot.GetItemID()].fuelValue * mouseSlot.GetItemAmount();

		mouseSlot.ClearSlot();
	}

	// Handle actual raw slot management:
	if(IRR->receiver->key_is_released[KEY_LBUTTON] && furnace->rawSlot.IsMouseOver())
	{
		furnace->rawSlot.ExhangeWithMouse(mouseSlot);
	}

	// Handle product slot management:
	if(IRR->receiver->key_is_released[KEY_LBUTTON] && furnace->productSlot.IsMouseOver())
	{
		// Placing or exhanging item:
		if(mouseSlot.GetItemID() != -1)
		{
			furnace->productSlot.ExhangeWithMouse(mouseSlot);
		}
	}
}

void PlayerControlledInstance :: UpdateEquipment()
{
	int armourSlot = GetArmourSlotMouseOver();

	// Handle actual raw slot management:
	if(IRR->receiver->key_is_released[KEY_LBUTTON] && armourSlot != -1
	&& (mouseSlot.GetItemID() == -1 || GManager->itemData[mouseSlot.GetItemID()].data[0] == armourSlot))
	{
		armourSlots[armourSlot].ExhangeWithMouse(mouseSlot);
		UpdateStats();
	}
}

void PlayerControlledInstance :: UpdateStats()
{
	armourClass = 0;

	for(int i = 0; i < 8; i++)
		if(!armourSlots[i].IsEmpty())
		{
			armourClass += GManager->itemData[armourSlots[i].GetItemID()].data[1];
		}
}

void PlayerControlledInstance :: UpdateCurrentCommand()
{
	/*
	if(currentCommand != 0 && currentCommand->commandType == MOVE_TO)
	{
		// If arrived at destination then complete command:
		if( (position).getDistanceFromSQ(currentCommand->position) <= 0.01f)
		{
			CompleteCommand(currentCommand);
			delete currentCommand;
			currentCommand = 0;
		}
	}

	if(currentCommand != 0 && currentCommand->commandType == DESTROY_BLOCK)
	{
		if( (position).getDistanceFrom(currentCommand->position + vector3df(0.5f, 0.0f, 0.5f)) > 2.0f)
		{
			// If to far from target block then complete command:
			delete currentCommand;
			currentCommand = 0;
			std::cout << "YARRRR to far from target block!!" << std::endl;
		}
		else if( GManager->IsBlockOpen(currentCommand->position) )
		{
			// If to far from target block then complete command:
			delete currentCommand;
			currentCommand = 0;
			std::cout << "DUUURRR JOB DONE!" << std::endl;
		}
		else
		{
			DamageBlock(vector3di(currentCommand->position.X, currentCommand->position.Y, currentCommand->position.Z), currentCommand->hotBarSlot);
		}
	}
	*/
}

void PlayerControlledInstance :: DrawHotBar()
{
	// Draw Hotbar:
	for(int x = 0; x < INVENTORY_SIZE_X; x++)
	{
		inventorySlots[x][INVENTORY_SIZE_Y - 1].Draw(true);
	}

	// Draw inventory Selected:
	IRR->driver->draw2DImage(inventorySelectTex, position2d<s32>(256 + hotBarSelected * 32, 565), rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);

}

void PlayerControlledInstance :: DrawInventory()
{
	// Draw Inventory:
	for(int y = 0; y < INVENTORY_SIZE_Y - 1; y++)
		for(int x = 0; x < INVENTORY_SIZE_X; x++)
		{
			inventorySlots[x][y].Draw(true);
		}

		// Draw Equipment:
	for(int i = 0; i < 8; i++)
		armourSlots[i].Draw(true);

	// Draw Stats:
	IRR->DrawText("HP: " + IRR->ToString(health) + "/" + IRR->ToString(100), 186, 204, SColor(255,255,255,255));
	IRR->DrawText("AC: " + IRR->ToString(armourClass), 186, 214, SColor(255,255,255,255));
}

void PlayerControlledInstance :: DrawCrafting()
{
	inventoryCraftingTable->DrawInterface();
}

void PlayerControlledInstance :: DrawCraftingTable()
{
	CraftingTableInstance* craftingTable = ((CraftingTableInstance*)(*craftingTableIt));

	craftingTable->DrawInterface();
}

void PlayerControlledInstance :: DrawFurnace()
{
	FurnaceItemInstance* furnace = ((FurnaceItemInstance*)(*furnaceIt));
	furnace->DrawInterface();
}

void PlayerControlledInstance :: DrawMouseSlot()
{
	// Update mouse slot position:
	mouseSlot.Init(IRR->receiver->mouse_x, IRR->receiver->mouse_y);

	// Draw mouse Item:
	mouseSlot.Draw(false);
}

void PlayerControlledInstance :: UpdateFood()
{
	if(IRR->GetTime() > decreaseFoodTime && currentFood > 0)
	{
		currentFood--;
		decreaseFoodTime = IRR->GetTime() + DECREASE_FOOD_TIME;
	}

	if(IRR->GetTime() > decreaseFoodTime && currentFood == 0)
	{
		Damage(1, vector3df(0,0,0));
		decreaseFoodTime = IRR->GetTime() + STARVING_DAMAGE_TIME;
	}
}

void PlayerControlledInstance :: EatFood(int amount)
{
	currentFood += amount;

	if(currentFood > maxFood)
		currentFood = maxFood;
}

void PlayerControlledInstance :: Damage(int amount, vector3df force)
{
	health -= amount;
}

void PlayerControlledInstance :: CheckDeath()
{
}

void PlayerControlledInstance :: ChangeState(CharacterState state)
{
}



