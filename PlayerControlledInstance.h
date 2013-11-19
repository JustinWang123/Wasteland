#pragma
#ifndef PLAYER_H
#define PLAYER_H

#include "IrrWrapper.h"
#include <vector>
#include <list>
#include <deque>
#include <string>
#include "ItemSlot.h"
#include "CraftingTableInstance.h"
#include "CharacterInstance.h"



class PlayerControlledInstance : public CharacterInstance
{
public:
	PlayerControlledInstance(vector3df setPos);
	~PlayerControlledInstance();

	virtual void	Update();
	virtual bool	HandleMessage(Telegram& msg);

	void			UpdateInventoryState();
	virtual void	UpdateCurrentCommand();
	virtual void	Damage(int amount, vector3df force);
	void			AddToChatWindow(std::string text) { chatWindow.push_front(text);}
	void			EatFood(int amount);
	void			SetHUDPosition(int pos);

	vector3df		GetPosition() {return position;}
	ITexture*		GetPortrait() {return portrait;} 
	
	void			DrawHotBar();
	void			DrawInventory();
	void			DrawCrafting();
	void			DrawCraftingTable();
	void			DrawFurnace();
	void			DrawMouseSlot();

private:
	virtual void	CheckDeath();
	virtual void	ChangeState(CharacterState state);

	void			UpdateRotation();
	void			UpdateVelocity();
	void			UpdatePosition();
	void			UpdateInventory();
	void			UpdateCrafting();
	void			UpdateCraftingTable();
	void			UpdateEquipment();
	void			UpdateFurnace();
	void			UpdateFood();
	void			UpdateStats();
	
	void			UseItem();
	
	// Inventory Textures:
	ITexture*		inventorySlotTex;
	ITexture*		inventorySelectTex;
	
	// HUD:
	ITexture*		aimmerTex;
	std::deque<std::string> chatWindow;

	ITexture*		portrait;


};
#endif
