#ifndef PROJECTILE_WEAPON_ITEM_H
#define PROJECTILE_WEAPON_ITEM_H

#include "BaseItem.h"
#include "ProjectileWeaponItemTemplate.h"
class CharacterInstanceSoldier;
struct Damage;

class ProjectileWeaponItem : public BaseItem
{
public:
	ProjectileWeaponItem(ProjectileWeaponItemTemplate* setTemplate);
	~ProjectileWeaponItem(){}

	virtual void Update();
	virtual void Use(CharacterInstance* user);

	bool Fire(CharacterInstanceSoldier* owner, vector3df target);
	void Reload(int numOfBullets);

	// Item Accessors:
	int 			GetAmmoInClip() const 					{return ammoInClip;}
	bool 			IsClipFull() const 						{return ammoInClip == GetClipSize();}
	bool			IsClipEmpty() const						{return ammoInClip == 0;}
	int 			GetDurability() const 					{return durability;}

	// Template Accessors:
	int             GetClipSize() const                     {return ((ProjectileWeaponItemTemplate*)pTemplate)->clipSize;}
	float           GetReloadTime() const                   {return ((ProjectileWeaponItemTemplate*)pTemplate)->reloadTime;}
	float           GetShootDelay() const                   {return ((ProjectileWeaponItemTemplate*)pTemplate)->shootDelay;}
	FireMode        GetFireMode() const                     {return ((ProjectileWeaponItemTemplate*)pTemplate)->fireMode;}
	CartridgeType   GetCartridgeType() const                {return ((ProjectileWeaponItemTemplate*)pTemplate)->cartridgeType;}
	int             GetAmmoItemTemplateID() const           {return ((ProjectileWeaponItemTemplate*)pTemplate)->ammoItemTemplateID;}
	int             GetMaxDurability() const                {return ((ProjectileWeaponItemTemplate*)pTemplate)->maxDurability;}
    int             GetRange()                         		{return GetTemplate()->range;}
	Damage*			GetDamage();
	float			GetDPS();

protected:
    ProjectileWeaponItemTemplate* GetTemplate() {return (ProjectileWeaponItemTemplate*)pTemplate;}

	int ammoInClip;
	int durability;
	int numShots;
};
#endif
