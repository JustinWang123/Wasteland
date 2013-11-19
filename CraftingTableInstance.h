#pragma
#ifndef CRAFTING_TABLE_INSTANCE_H
#define CRAFTING_TABLE_INSTANCE_H

#include "ItemInstance.h"
#include "ItemSlot.h"
#include <vector>

class CraftingTableInstance : public ItemInstance
{
public:
	CraftingTableInstance(int itemID, vector3df position, vector3df rotation);
	~CraftingTableInstance();

	void DrawInterface();

	int GetCraftingSizeX();
	int GetCraftingSizeY();
	int GetToolSize();

	// Methods Crafting Slots:
	bool		IsMouseOverCrafting();
	vector2di	GetCraftingSlotMouseOver();
	bool		IsItemInCraftingSlot(int ID, int amount);

	// Methods Tool Slots:
	bool		IsMouseOverTools();
	int			GetToolSlotMouseOver();
	bool		IsItemInToolSlot(int ID);

	// Recipe Slots:
	void		UpdateRecipes();
	bool		IsMouseOverRecipes();
	int			GetRecipeMouseOver();
	void		CraftRecipe(int recipeID, ItemSlot& mouseSlot);
	bool		IsRecipeCraftable(int ID);

	// Members public for easy access:
	std::vector< std::vector<ItemSlot> >	craftingSlots;
	std::vector<ItemSlot>				toolSlots;

	int		craftingSizeX;
	int		craftingSizeY;
	int		toolSize;

	std::vector<int>	recipeIDs;
	bool				recipeUpArrowPressed;
	bool				recipeDownArrowPressed;
	int					recipeIndex;

private:
	ITexture*	recipeSlotTex;
	ITexture*	upArrowUnpressedTex;
	ITexture*	downArrowUnpressedTex;
	ITexture*	upArrowPressedTex;
	ITexture*	downArrowPressedTex;
};
#endif
