#include "FurnaceItemInstance.h"
#include "GameManager.h"

FurnaceItemInstance :: FurnaceItemInstance(int itemID, vector3df position, vector3df rotation)
	: ItemInstance(itemID, 1, 100, 0, position, rotation)
{
	fuel = 0;
	fuelTowardsProduct = 0;
	fuelSlot.Init(256, 160);
	rawSlot.Init(256, 112);
	productSlot.Init(320, 112);
}

FurnaceItemInstance :: ~FurnaceItemInstance()
{
}

void FurnaceItemInstance :: DrawInterface()
{
	fuelSlot.Draw(true);
	rawSlot.Draw(true);
	productSlot.Draw(true);
	IRR->DrawText(IRR->ToString(fuel), 260 , 176, SColor(255, 255, 0,0));

	IRR->DrawText(IRR->ToString(fuelTowardsProduct), 296 , 128, SColor(255, 255, 0,0));	
}

void FurnaceItemInstance :: Update()
{
	if(rawSlot.GetItemID() != -1
	&& GManager->itemData[rawSlot.GetItemID()].furnaceProduct != -1
	&& ((GManager->itemData[rawSlot.GetItemID()].furnaceProduct == productSlot.GetItemID() && productSlot.GetItemAmount() < 64)
	|| productSlot.IsEmpty()))
	{
		if(fuel > 0)
		{
			fuel--;
			fuelTowardsProduct++;
		}

		if(fuelTowardsProduct >  GManager->itemData[rawSlot.GetItemID()].furnaceFuelCost)
		{
			
			fuelTowardsProduct = 0;

			productSlot.AddToSlot(GManager->itemData[rawSlot.GetItemID()].furnaceProduct, 
								  1,
								  0,
								  GManager->itemData[rawSlot.GetItemID()].maxDurability );

			rawSlot.RemoveFromSlot(1);
		}
			
	}
}