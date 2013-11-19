#include "SoldierInventoryInterface.h"
#include "CharacterInstanceSoldier.h"
#include "InstanceManager.h"
#include "ItemList.h"
#include "ItemSlot.h"
#include "ProjectileWeaponItem.h"
#include "ArmourItem.h"

SoldierInventoryInterface :: SoldierInventoryInterface()
{
	pSoldier = 0;

	// These vectors determine where the interface will be drawn onscreen:
	inventoryListPos = vector2di(0,0);
	hotbarPos = vector2di(0,0);

	topInventorySlotIndex = 0;

	SetInterfacePosition(inventoryListPos, hotbarPos);

	// Load texture for hotbar select:
    hotbarSelectTexture = IRR->driver->getTexture("tex/InventorySelect.bmp");
    IRR->driver->makeColorKeyTexture(hotbarSelectTexture, core::position2d<s32>(16,16));

    // Load HUD textures:
    hudTopLeft = IRR->driver->getTexture("tex/HUDTopLeft.bmp");
	hudTop = IRR->driver->getTexture("tex/HUDTop.bmp");
	hudTopRight = IRR->driver->getTexture("tex/HUDTopRight.bmp");
	hudRight = IRR->driver->getTexture("tex/HUDRight.bmp");
	hudBottemRight = IRR->driver->getTexture("tex/HUDBottemRight.bmp");
	hudBottem = IRR->driver->getTexture("tex/HUDBottem.bmp");
	hudBottemLeft = IRR->driver->getTexture("tex/HUDBottemLeft.bmp");
	hudLeft = IRR->driver->getTexture("tex/HUDLeft.bmp");
	hudMiddle = IRR->driver->getTexture("tex/HUDMiddle.bmp");
}

void SoldierInventoryInterface :: SetInterfacePosition(vector2di setInventoryListPos, vector2di setHotbarPos)
{
	inventoryListPos = setInventoryListPos;
	hotbarPos = setHotbarPos;

	// Init buttons:
	invListUpButton.Init(inventoryListPos.X - 16, inventoryListPos.Y , IRR->driver->getTexture("tex/UpArrowUnpressed.bmp"), IRR->driver->getTexture("tex/UpArrowUnpressed.bmp"), IRR->driver->getTexture("tex/UpArrowPressed.bmp"));
	invListDownButton.Init(inventoryListPos.X - 16, inventoryListPos.Y + 32 * INVENTORY_VISIBLE_SIZE - 16, IRR->driver->getTexture("tex/DownArrowUnpressed.bmp"), IRR->driver->getTexture("tex/DownArrowUnpressed.bmp"), IRR->driver->getTexture("tex/DownArrowPressed.bmp"));

	// Init inventory list buttons:
	for(int i = 0; i < INVENTORY_VISIBLE_SIZE; i++)
		invListButtons[i].Init(inventoryListPos.X, inventoryListPos.Y + i * 32, IRR->driver->getTexture("tex/InventoryButtonUp.bmp"), IRR->driver->getTexture("tex/InventoryButtonOver.bmp"), IRR->driver->getTexture("tex/InventoryButtonDown.bmp"));

	// Init hotbar buttons:
	for( int i = 0; i < HOT_BAR_SIZE; i++)
		hotBarButtons[i].Init(hotbarPos.X + i * 32, hotbarPos.Y, IRR->driver->getTexture("tex/InventorySlot.bmp"), IRR->driver->getTexture("tex/InventorySlot.bmp"), IRR->driver->getTexture("tex/InventorySlot.bmp"));

	// Init armor buttons:
	armorSlotButtons[ARMOR_SLOT_HEAD].Init(inventoryListPos.X + 344, inventoryListPos.Y + 190, IRR->driver->getTexture("tex/ArmorSlotHead.bmp"), IRR->driver->getTexture("tex/ArmorSlotHead.bmp"), IRR->driver->getTexture("tex/ArmorSlotHead.bmp"));
	armorSlotButtons[ARMOR_SLOT_CHEST].Init(inventoryListPos.X + 392, inventoryListPos.Y + 190, IRR->driver->getTexture("tex/ArmorSlotChest.bmp"), IRR->driver->getTexture("tex/ArmorSlotChest.bmp"), IRR->driver->getTexture("tex/ArmorSlotChest.bmp"));
	armorSlotButtons[ARMOR_SLOT_LEGS].Init(inventoryListPos.X + 344, inventoryListPos.Y + 238, IRR->driver->getTexture("tex/ArmorSlotLegs.bmp"), IRR->driver->getTexture("tex/ArmorSlotLegs.bmp"), IRR->driver->getTexture("tex/ArmorSlotLegs.bmp"));
	armorSlotButtons[ARMOR_SLOT_FEET].Init(inventoryListPos.X + 392, inventoryListPos.Y + 238, IRR->driver->getTexture("tex/ArmorSlotFeet.bmp"), IRR->driver->getTexture("tex/ArmorSlotFeet.bmp"), IRR->driver->getTexture("tex/ArmorSlotFeet.bmp"));

}

void  SoldierInventoryInterface :: UpdateInventoryManagement()
{
	// Scroll the inventory list up:
	if(invListUpButton.IsClicked() == true && topInventorySlotIndex > 0)
	{
		topInventorySlotIndex--;
	}

	// Scroll the inventory list down:
	if(invListDownButton.IsClicked() == true && topInventorySlotIndex + INVENTORY_VISIBLE_SIZE < pSoldier->inventory.GetSize())
	{
		topInventorySlotIndex++;
	}

	if(IRR->receiver->key_is_released[KEY_LBUTTON] && !mouseSlot.IsEmpty() && !IsMouseOnHUD())
	{
		InstanceMgr->CreateItemInstance(mouseSlot.GetItem(), mouseSlot.GetItemAmount(), pSoldier->GetFeetPosition(), vector3df(0,0,0));
		mouseSlot.ClearSlot();
	}


	// Place items and remove items from inventory list:
	for(int i = 0; i < INVENTORY_VISIBLE_SIZE ; i++)
	{
		if(invListButtons[i].IsClicked())
		{
			// If inventory list is pressed and the mouse is full then drop mouse slot into inventory
			if(!mouseSlot.IsEmpty())
			{
				pSoldier->inventory.AddToList(mouseSlot.GetItem(), mouseSlot.GetItemAmount());
				mouseSlot.ClearSlot();
			}
			// If an inventory item is pressed and the mouse slot is empty:
			// Add the item to the mouse slot and remove it from the inventory
			else
			{
				mouseSlot.AddToSlot(pSoldier->inventory.GetSlotItem(i + topInventorySlotIndex), pSoldier->inventory.GetSlotAmount(i + topInventorySlotIndex));
				pSoldier->inventory.ClearSlot(i + topInventorySlotIndex);
			}
		}
	}

	// Place items and remove items from hotbar:
	for(int i = 0; i < HOT_BAR_SIZE; i++)
	{
		if(hotBarButtons[i].IsClicked())
		{
			// If an inventory item is pressed and the mouse is full:
			// Place the contents of mouse slot in the inventory and pick up the new item from inventory
			if(!mouseSlot.IsEmpty())
			{
				if(mouseSlot.GetType() == ITEM_TYPE_PROJECTILE_WEAPON || mouseSlot.GetType() == ITEM_TYPE_MELEE_WEAPON || mouseSlot.GetType() == ITEM_TYPE_CONSUMABLE)
				{
					BaseItem* tempItem = mouseSlot.GetItem();
					int tempAmount = mouseSlot.GetItemAmount();

					mouseSlot.ClearSlot();
					mouseSlot.AddToSlot(pSoldier->hotbarSlots[i].GetItem(), pSoldier->hotbarSlots[i].GetItemAmount());

					pSoldier->hotbarSlots[i].ClearSlot();
					pSoldier->hotbarSlots[i].AddToSlot(tempItem, tempAmount);
				}
			}
			// If an inventory item is pressed and the mouse slot is empty:
			// Add the item to the mouse slot and remove it from the inventory
			else
			{
				mouseSlot.AddToSlot(pSoldier->hotbarSlots[i].GetItem(), pSoldier->hotbarSlots[i].GetItemAmount());
				pSoldier->hotbarSlots[i].ClearSlot();
			}
		}
	}

	// Place items and remove items from armourSlots:
	for(int i = 0; i < 4; i++)
	{
		if(armorSlotButtons[i].IsClicked())
		{
			// If an armour slot is pressed and the mouse is full:
			// Place the contents of mouse slot in the armour slot and place the previous contents of the slow on the mouse
			if(!mouseSlot.IsEmpty() )
			{
				if(mouseSlot.GetType() == ITEM_TYPE_ARMOUR && ((ArmourItem*)mouseSlot.GetItem())->GetSlot() == i)
				{
					BaseItem* tempItem = mouseSlot.GetItem();
					int tempAmount = mouseSlot.GetItemAmount();

					mouseSlot.ClearSlot();
					mouseSlot.AddToSlot(pSoldier->armourSlots[i].GetItem(), pSoldier->armourSlots[i].GetItemAmount());

					pSoldier->armourSlots[i].ClearSlot();
					pSoldier->armourSlots[i].AddToSlot(tempItem, tempAmount);

					pSoldier->CalculateDerivedStats();
				}
			}
			// If an inventory item is pressed and the mouse slot is empty:
			// Add the item to the mouse slot and remove it from the armour slot
			else
			{
				mouseSlot.AddToSlot(pSoldier->armourSlots[i].GetItem(), pSoldier->armourSlots[i].GetItemAmount());
				pSoldier->armourSlots[i].ClearSlot();
				pSoldier->CalculateDerivedStats();
			}
		}
	}

	// Update inventory list buttons:
	for(int i = 0; i < INVENTORY_VISIBLE_SIZE; i++)
		invListButtons[i].Update();

	// Update hotbar buttons:
	for( int i = 0; i < HOT_BAR_SIZE; i++)
		hotBarButtons[i].Update();

	// Update armor buttons:
	for( int i = 0; i < 4; i++)
		armorSlotButtons[i].Update();

	// Update list scroll up and down buttons:
	invListUpButton.Update();
	invListDownButton.Update();
}

void SoldierInventoryInterface :: DrawMouseSlot()
{
	// Draw mouse slot:
	if(!mouseSlot.IsEmpty())
	{
		IRR->driver->draw2DImage(mouseSlot.GetInvTex(), position2d<s32>(IRR->receiver->mouse_x, IRR->receiver->mouse_y), rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);
	}
}

void SoldierInventoryInterface :: DrawInventoryList()
{
	// Draw Inventory list buttons:
	for(int i = 0; i < INVENTORY_VISIBLE_SIZE; i++)
		invListButtons[i].Draw();

	// Draw the inventory list items:
	for(int i = 0; i < INVENTORY_VISIBLE_SIZE && i + topInventorySlotIndex < pSoldier->inventory.GetSize(); i++)
	{
		IRR->driver->draw2DImage(pSoldier->inventory.GetSlotItem(i+topInventorySlotIndex)->GetInvTex(), position2d<s32>(inventoryListPos.X, inventoryListPos.Y + i * 32), rect<s32>(0,0,32,32), 0, SColor(255,255,255,255), true);

		if(pSoldier->inventory.GetSlotItem(i+topInventorySlotIndex)->GetIsStackable())
		{
			IRR->DrawText(pSoldier->inventory.GetSlotItem(i+topInventorySlotIndex)->GetName() + " - " + IRR->ToString(pSoldier->inventory.GetSlotAmount(i+topInventorySlotIndex)), inventoryListPos.X + 40, inventoryListPos.Y + i * 32 + 4, SColor(255, 0,0,0));
		}
		else
		{
			IRR->DrawText(pSoldier->inventory.GetSlotItem(i+topInventorySlotIndex)->GetName(), inventoryListPos.X + 40, inventoryListPos.Y + i * 32 + 4, SColor(255, 0,0,0));
		}
	}

	// Draw scroll up and down buttons:
	invListUpButton.Draw();
	invListDownButton.Draw();

	// Draw HUD Background:
	DrawHUDBackground(inventoryListPos + vector2di(160,0), vector2di(5,9));
	DrawHUDBackground(inventoryListPos + vector2di(320,0), vector2di(4,9));


	armorSlotButtons[ARMOR_SLOT_HEAD].Draw();
	armorSlotButtons[ARMOR_SLOT_CHEST].Draw();
	armorSlotButtons[ARMOR_SLOT_LEGS].Draw();
	armorSlotButtons[ARMOR_SLOT_FEET].Draw();

	if(!pSoldier->armourSlots[ARMOR_SLOT_HEAD].IsEmpty())
		IRR->DrawTexture(pSoldier->armourSlots[ARMOR_SLOT_HEAD].GetInvTex(), inventoryListPos.X + 344, inventoryListPos.Y + 190);
	if(!pSoldier->armourSlots[ARMOR_SLOT_CHEST].IsEmpty())
		IRR->DrawTexture(pSoldier->armourSlots[ARMOR_SLOT_CHEST].GetInvTex(), inventoryListPos.X + 392, inventoryListPos.Y + 190);
	if(!pSoldier->armourSlots[ARMOR_SLOT_LEGS].IsEmpty())
		IRR->DrawTexture(pSoldier->armourSlots[ARMOR_SLOT_LEGS].GetInvTex(), inventoryListPos.X + 344, inventoryListPos.Y + 238);
	if(!pSoldier->armourSlots[ARMOR_SLOT_FEET].IsEmpty())
		IRR->DrawTexture(pSoldier->armourSlots[ARMOR_SLOT_FEET].GetInvTex(), inventoryListPos.X + 392, inventoryListPos.Y + 238);

	// Draw Attributes:
	IRR->DrawText("Strength: " + IRR->ToString(pSoldier->strength), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 10, SColor(255, 0,0,0));
	IRR->DrawText("Speed: " + IRR->ToString(pSoldier->speed), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 30, SColor(255, 0,0,0));
	IRR->DrawText("Perception: " + IRR->ToString(pSoldier->perception), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 50, SColor(255, 0,0,0));
	IRR->DrawText("Stamina: " + IRR->ToString(pSoldier->stamina), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 70, SColor(255, 0,0,0));

	// Draw Statistics:
	IRR->DrawText("Health: " + IRR->ToString(pSoldier->currentHealth) + "/" + IRR->ToString(pSoldier->maxHealth), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 100, SColor(255, 0,0,0));
	IRR->DrawText("Damage Mitigation: " + IRR->ToString(pSoldier->damageMitigation), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 120, SColor(255, 0,0,0));
	IRR->DrawText("Fire Mitigation: " + IRR->ToString(pSoldier->fireMitigation), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 140, SColor(255, 0,0,0));
	IRR->DrawText("Weight: " + IRR->ToString(pSoldier->currentWeight) + "/" + IRR->ToString(pSoldier->weightCapacity), inventoryListPos.X + 320 + 10, inventoryListPos.Y + 160, SColor(255, 0,0,0));

}


void SoldierInventoryInterface :: DrawHotbarSlots()
{
	// Draw hotbar buttons:
	for( int i = 0; i < HOT_BAR_SIZE; i++)
		hotBarButtons[i].Draw();

	// Draw the hotbar items:
	for(int i = 0; i < HOT_BAR_SIZE; i++)
	{
		IRR->DrawTexture(pSoldier->hotbarSlots[i].GetInvTex(), hotbarPos.X + i*32, hotbarPos.Y);

		if(!pSoldier->hotbarSlots[i].IsEmpty() && pSoldier->hotbarSlots[i].GetType() == ITEM_TYPE_PROJECTILE_WEAPON)
		{
			int ammoItemTemplateID = ((ProjectileWeaponItem*)pSoldier->hotbarSlots[i].GetItem())->GetAmmoItemTemplateID();
			int ammoInInventory = pSoldier->AmountOfItemInInventory(ammoItemTemplateID);
			int ammoInWeapon = ((ProjectileWeaponItem*)pSoldier->hotbarSlots[i].GetItem())->GetAmmoInClip();
			IRR->DrawText(IRR->ToString(ammoInWeapon) + "-" + IRR->ToString(ammoInInventory), hotbarPos.X + i*32 + 4, hotbarPos.Y, SColor(255, 0,0,0));
		}
	}

	// Draw hotbar selector:
	IRR->DrawTexture(hotbarSelectTexture, hotbarPos.X + pSoldier->GetHotbarSelected() * 32, hotbarPos.Y);
}



void SoldierInventoryInterface :: UpdateHotbarSelect()
{
	if(IRR->receiver->key_is_released[KEY_LBUTTON] && IsMouseOnHotbar())
	{
		pSoldier->SetHotbarSelected( (IRR->receiver->mouse_x - hotbarPos.X) / 32  );
	}
}

bool SoldierInventoryInterface :: IsMouseOnHotbar()
{
	return IRR->receiver->mouse_y >= hotbarPos.Y
		&& IRR->receiver->mouse_y <  hotbarPos.Y + 32
		&& IRR->receiver->mouse_x >= hotbarPos.X
		&& IRR->receiver->mouse_x < hotbarPos.X + HOT_BAR_SIZE * 32;
}

void SoldierInventoryInterface :: DrawHUDBackground(vector2di pos, vector2di numOfTiles)
{
	for(int x = 0; x < numOfTiles.X; x++)
		for(int y = 0; y < numOfTiles.Y; y++)
		{
			if(x == 0 && y == 0)
			{
				IRR->DrawTexture(hudTopLeft, pos.X + x * 32, pos.Y + y * 32);
			}
			else if(x == numOfTiles.X - 1 && y == 0)
			{
				IRR->DrawTexture(hudTopRight, pos.X + x * 32, pos.Y + y * 32);
			}
			else if(x == numOfTiles.X - 1 && y == numOfTiles.Y - 1)
			{
				IRR->DrawTexture(hudBottemRight, pos.X + x * 32, pos.Y + y * 32);
			}
			else if(x == 0 && y == numOfTiles.Y - 1)
			{
				IRR->DrawTexture(hudBottemLeft, pos.X + x * 32, pos.Y + y * 32);
			}
			else if(x == 0)
			{
				IRR->DrawTexture(hudLeft, pos.X + x * 32, pos.Y + y * 32);
			}
			else if(x == numOfTiles.X - 1)
			{
				IRR->DrawTexture(hudRight, pos.X + x * 32, pos.Y + y * 32);
			}
			else if(y == 0)
			{
				IRR->DrawTexture(hudTop, pos.X + x * 32, pos.Y + y * 32);
			}
			else if(y == numOfTiles.Y - 1)
			{
				IRR->DrawTexture(hudBottem, pos.X + x * 32, pos.Y + y * 32);
			}
			else
			{
				IRR->DrawTexture(hudMiddle, pos.X + x * 32, pos.Y + y * 32);
			}

		}
}

bool SoldierInventoryInterface :: IsMouseOnHUD()
{
	int mouseX = IRR->receiver->mouse_x;
	int mouseY = IRR->receiver->mouse_y;

	return 	(mouseX >= inventoryListPos.X && mouseX < inventoryListPos.X + 448
		&&	mouseY >= inventoryListPos.Y && mouseY < inventoryListPos.Y + 288)
		||
		(mouseX >= hotbarPos.X && mouseX < hotbarPos.X + 160
		&&	mouseY >= hotbarPos.Y && mouseY < hotbarPos.Y + 32);
}
