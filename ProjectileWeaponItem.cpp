#include "ProjectileWeaponItem.h"
#include "InstanceManager.h"
#include "CharacterInstanceSoldier.h"
#include "MessageManager.h"
#include "ItemManager.h"
#include "AmmoItem.h"
#include <iostream>

ProjectileWeaponItem :: ProjectileWeaponItem(ProjectileWeaponItemTemplate* setTemplate)
	: 	BaseItem(setTemplate),
		ammoInClip(0),
		durability(setTemplate->maxDurability),
		numShots(0)
		{}

void ProjectileWeaponItem :: Update()
{
}

void ProjectileWeaponItem :: Use(CharacterInstance* user)
{
	//Fire(user->GetBodyPosition(), user->GetLookAtHeading(), user);
}

bool ProjectileWeaponItem :: Fire(CharacterInstanceSoldier* owner, vector3df target)
{
	std::cout << "ProjectileWeaponItem::Use()\n";

	vector3df pos = owner->GetWeaponPosition();
	vector3df heading = target - pos;
	heading.normalize();

	if(ammoInClip > 0)
	{
		owner->AddActionDelay(GetShootDelay());

		ammoInClip--;

		Damage* damage = GetDamage();

		if(GetCartridgeType() == CARTRIDGE_TYPE_SLUG)
		{
			InstanceMgr->CreateProjectileInstance(PROJECTILE_INSTANCE_BULLET, damage, pos, heading, owner);
		}
		else if(GetCartridgeType() == CARTRIDGE_TYPE_SHOT)
		{
			InstanceMgr->CreateProjectileInstance(PROJECTILE_INSTANCE_BULLET, damage, pos, heading, owner);

			vector3df bulletHeading = heading;
			bulletHeading.rotateXYBy(0.02, pos);
			InstanceMgr->CreateProjectileInstance(PROJECTILE_INSTANCE_BULLET, damage, pos, bulletHeading, owner);

			bulletHeading = heading;
			bulletHeading.rotateXYBy(-0.02, pos);
			InstanceMgr->CreateProjectileInstance(PROJECTILE_INSTANCE_BULLET, damage, pos, bulletHeading, owner);


			bulletHeading = heading;
			bulletHeading.rotateXZBy(0.02, pos);
			InstanceMgr->CreateProjectileInstance(PROJECTILE_INSTANCE_BULLET, damage, pos, bulletHeading, owner);

			bulletHeading = heading;
			bulletHeading.rotateXZBy(-0.02, pos);
			InstanceMgr->CreateProjectileInstance(PROJECTILE_INSTANCE_BULLET, damage, pos, bulletHeading, owner);

		}


		else if(GetFireMode() == FIRE_MODE_BURST)
		{
			numShots++;

			if(numShots == 3)
			{
				numShots = 0;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}



void ProjectileWeaponItem :: Reload(int numOfBullets)
{
	ammoInClip += numOfBullets;

	if(ammoInClip > GetClipSize())
		ammoInClip = GetClipSize();
}

Damage* ProjectileWeaponItem :: GetDamage()
{
	return ((AmmoItem*)ItemMgr->GetItem(GetAmmoItemTemplateID()))->GetDamage();
}

float ProjectileWeaponItem :: GetDPS()
{
	return (float)GetDamage()->type[DAMAGE_TYPE_FLESH] / (float)GetShootDelay();
}

