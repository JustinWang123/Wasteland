#include "CraftingTableInstance.h"
#include "GameManager.h"
#include "ItemSlot.h"

CraftingTableInstance :: CraftingTableInstance(int itemID, vector3df position, vector3df rotation)
	: ItemInstance(itemID, 1, 100, 0, position, rotation)
{
	recipeSlotTex = IRR->driver->getTexture("tex/RecipeSlot.bmp");
	upArrowUnpressedTex = IRR->driver->getTexture("tex/UpArrow(UnPressed).bmp");
	downArrowUnpressedTex = IRR->driver->getTexture("tex/DownArrow(UnPressed).bmp");
	upArrowPressedTex = IRR->driver->getTexture("tex/UpArrow(Pressed).bmp");
	downArrowPressedTex = IRR->driver->getTexture("tex/DownArrow(Pressed).bmp");

	craftingSizeX = GManager->itemData[itemID].data[0];
	craftingSizeY = GManager->itemData[itemID].data[1];
	toolSize = GManager->itemData[itemID].data[2];

	recipeUpArrowPressed = false;
	recipeDownArrowPressed = false;
	recipeIndex = 0;
	
	// Init Crafting Slots:
	craftingSlots.resize(craftingSizeX);

	for(int x = 0; x < craftingSizeX; x++)
		craftingSlots[x].resize(craftingSizeY);
	
	for(int x = 0; x < craftingSizeX; x++)
		for(int y = 0; y < craftingSizeY; y++)
			craftingSlots[x][y].Init(256 + x * 32, 112 + y * 32);
	
	// Init Tool Slots:
	toolSlots.resize(toolSize);

	for(int x = 0; x < toolSize; x++)
		toolSlots[x].Init(336, 112 + x * 32);
}

CraftingTableInstance :: ~CraftingTableInstance()
{
}

void CraftingTableInstance :: DrawInterface()
{
	for(int x = 0; x < craftingSizeX; x++)
		for(int y = 0; y < craftingSizeY; y++)
	{
			craftingSlots[x][y].Draw(true);
		}

	for(int x = 0; x < toolSlots.size(); x++)
	{
			toolSlots[x].Draw(true);
	}

	for(int y = 0; y < 4; y++)
	{
		IRR->driver->draw2DImage(recipeSlotTex, position2d<s32>(380, 112 + y * 16), rect<s32>(0,0,128,16), 0, SColor(255,255,255,255), true);
	
		if(y + recipeIndex < recipeIDs.size())
		{
			std::string name = GManager->itemData[GManager->recipeData[ recipeIDs[y + recipeIndex] ].resultID].name;

			name += " x " + IRR->ToString(GManager->recipeData[ recipeIDs[y + recipeIndex] ].resultAmount);

			IRR->DrawText(name, 390, 112 + y * 16);
		}
	}

	if(recipeUpArrowPressed)
	{
		IRR->driver->draw2DImage(upArrowPressedTex, position2d<s32>(508, 112), rect<s32>(0,0,16,16), 0, SColor(255,255,255,255), true);
	}
	else
	{
		IRR->driver->draw2DImage(upArrowUnpressedTex, position2d<s32>(508, 112), rect<s32>(0,0,16,16), 0, SColor(255,255,255,255), true);
	}

	if(recipeDownArrowPressed)
	{
		IRR->driver->draw2DImage(downArrowPressedTex, position2d<s32>(508, 160), rect<s32>(0,0,16,16), 0, SColor(255,255,255,255), true);
	}
	else
	{
		IRR->driver->draw2DImage(downArrowUnpressedTex, position2d<s32>(508, 160), rect<s32>(0,0,16,16), 0, SColor(255,255,255,255), true);
	}

}

void CraftingTableInstance :: UpdateRecipes()
{
	recipeIndex = 0;

	recipeIDs.clear();

	for(int i = 0; i < MAX_RECIPES; i++)
		if(GManager->recipeData[i].resultID != -1 && IsRecipeCraftable(i))
			recipeIDs.push_back(i);
}



// Crafting.............................................
bool CraftingTableInstance :: IsMouseOverCrafting()
{
	return (IRR->receiver->mouse_x >= 256 
		&& IRR->receiver->mouse_x < 256 + craftingSizeX * 32
		&& IRR->receiver->mouse_y >= 112 
		&& IRR->receiver->mouse_y < 112 + craftingSizeY * 32);
}

vector2di CraftingTableInstance :: GetCraftingSlotMouseOver()
{
	vector2di slot(-1,-1);

	if(IsMouseOverCrafting())
	{
		int x = IRR->receiver->mouse_x - 256;
		slot.X = (x - x % 32)/ 32;

		int y = IRR->receiver->mouse_y - 112;
		slot.Y = (y - y % 32)/ 32;
	}
	return slot;
}

bool CraftingTableInstance :: IsItemInCraftingSlot(int ID, int amount)
{

	int amountRemaining = amount;

	for(int x = 0; x < craftingSizeX; x++)
		for(int y = 0; y < craftingSizeY; y++)
			if(craftingSlots[x][y].GetItemID() == ID)
				amountRemaining -= craftingSlots[x][y].GetItemAmount();

	if(amountRemaining <= 0)
		return true;
	else
		return false;
}

// Tools.............................................
bool CraftingTableInstance :: IsMouseOverTools()
{
	return (IRR->receiver->mouse_x >= 336 
	&& IRR->receiver->mouse_x < 336 + toolSize * 32
	&& IRR->receiver->mouse_y >= 112 
	&& IRR->receiver->mouse_y < 144);
}

int CraftingTableInstance :: GetToolSlotMouseOver()
{
	int slot = -1;

	if(IsMouseOverTools())
	{
		int x = IRR->receiver->mouse_x - 336;
		slot = (x - x % 32)/ 32;
	}

	return slot;
}

bool CraftingTableInstance :: IsItemInToolSlot(int ID)
{
	for(int x = 0; x < toolSize; x++)
		if(toolSlots[x].GetItemID() == ID)
				return true;

	return false;
}

bool CraftingTableInstance :: IsRecipeCraftable(int ID)
{

	for(int i = 0; i < GManager->recipeData[ID].itemIDs.size(); i++)
		if(!IsItemInCraftingSlot(GManager->recipeData[ID].itemIDs[i], GManager->recipeData[ID].itemAmounts[i]))
			return false;

	for(int i = 0; i < GManager->recipeData[ID].toolIDs.size(); i++)
		if(!IsItemInToolSlot(GManager->recipeData[ID].toolIDs[i]))
			return false;

	return true;
}

// Recipe Slots:
bool CraftingTableInstance :: IsMouseOverRecipes()
{
	return (IRR->receiver->mouse_x >= 380 
	&& IRR->receiver->mouse_x < 508
	&& IRR->receiver->mouse_y >= 112 
	&& IRR->receiver->mouse_y < 178);
}

int CraftingTableInstance :: GetRecipeMouseOver()
{
	int y = IRR->receiver->mouse_y - 112;

	y = (y - y % 16)/ 16;

	if(recipeIndex + y < recipeIDs.size())
	{
		return recipeIDs[y + recipeIndex];
	}
	else
	{
		return -1;
	}
}

void CraftingTableInstance :: CraftRecipe(int recipeID, ItemSlot& mouseSlot)
{
	for(int i = 0; i < GManager->recipeData[recipeID].itemIDs.size(); i++)
	{
		int remaining = GManager->recipeData[recipeID].itemAmounts[i];

		for(int x = 0; x < craftingSizeX; x++)
			for(int y = 0; y < craftingSizeY; y++)
				if(craftingSlots[x][y].GetItemID() == GManager->recipeData[recipeID].itemIDs[i])
				{
					if(craftingSlots[x][y].GetItemAmount() >= remaining)
					{
						craftingSlots[x][y].RemoveFromSlot(remaining);
						goto nextItem;
					}
					else
					{
						remaining -= craftingSlots[x][y].GetItemAmount();
						craftingSlots[x][y].ClearSlot();
					}
				}

		nextItem:;
	}

	// Adding to empty mouse:
	if(mouseSlot.GetItemID() == -1)
	{
		mouseSlot.SetItemID(GManager->recipeData[recipeID].resultID);
		mouseSlot.SetItemAmount(GManager->recipeData[recipeID].resultAmount);
		mouseSlot.SetItemAmmo(0);
		mouseSlot.SetItemDurability(GManager->itemData[GManager->recipeData[recipeID].resultID].maxDurability);
	}
	// Adding to a stack:
	else
	{
		mouseSlot.AddToSlot(mouseSlot.GetItemID(), 
							GManager->recipeData[recipeID].resultAmount,
							-1,
							-1);
	}
}

int CraftingTableInstance :: GetCraftingSizeX()
{
	return GManager->itemData[itemID].data[0];
}

int CraftingTableInstance :: GetCraftingSizeY()
{
	return GManager->itemData[itemID].data[1];
}

int CraftingTableInstance :: GetToolSize()
{
	return GManager->itemData[itemID].data[2];
}

