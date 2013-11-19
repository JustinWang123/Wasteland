#include "ToolItem.h"
#include "InstanceManager.h"
#include "CharacterInstance.h"
#include "MessageManager.h"

ToolItem :: ToolItem(ToolItemTemplate* setTemplate)
	: BaseItem(setTemplate)
{
	currentUseDelay = 0.0f;
	durability = setTemplate->maxDurability;
}

void ToolItem :: Update()
{
}

void ToolItem :: Use(CharacterInstance* owner)
{
	owner->AddActionDelay(GetTemplate()->delay);

	int id;
	vector3df impactPos;

	if(InstanceMgr->IsRaySolid(owner->GetHeadPosition(), owner->GetLookAtHeading(), GetTemplate()->range, BITMASK_CHARACTER | BITMASK_WORLD, id, impactPos))
	{
		DamageHitMessage message;
		message.damage = GetTemplate()->damage;
		message.position = impactPos;
		message.heading = owner->GetLookAtHeading();

		MessageMgr->DispatchMsg(0.0f, 0, id, MESSAGE_TYPE_DAMAGE_HIT, (void*)&message);
			
	}
}