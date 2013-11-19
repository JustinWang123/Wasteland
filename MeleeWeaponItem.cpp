#include "MeleeWeaponItem.h"
#include "MeleeWeaponItemTemplate.h"
#include "InstanceManager.h"
#include "CharacterInstance.h"
#include "MessageManager.h"
#include <iostream>

MeleeWeaponItem :: MeleeWeaponItem(MeleeWeaponItemTemplate* setTemplate)
	: BaseItem(setTemplate)
{
	currentUseDelay = 0.0f;
}

void MeleeWeaponItem :: Update()
{
}

void MeleeWeaponItem :: Use(CharacterInstance* owner)
{
	std::cout << "MeleeWeaponItem::Use()\n";

	owner->AddActionDelay(GetTemplate()->delay);

	int id;
	vector3df impactPos;

	if(InstanceMgr->IsRaySolid(owner->GetBodyPosition() + owner->GetLookAtHeading(), owner->GetLookAtHeading(), GetTemplate()->range, BITMASK_CHARACTER, id, impactPos))
	{
		DamageHitMessage message;
		message.damage = GetTemplate()->damage;
		message.position = impactPos;
		message.heading = owner->GetLookAtHeading();

		MessageMgr->DispatchMsg(0.0f, 0, id, MESSAGE_TYPE_DAMAGE_HIT, (void*)&message);
	}
}
