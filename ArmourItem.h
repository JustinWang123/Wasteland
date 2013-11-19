#ifndef ARMOUR_ITEM_H
#define ARMOUR_ITEM_H

#include "BaseItem.h"
#include "ArmourItemTemplate.h"

class ArmourItem : public BaseItem
{
public:
	ArmourItem(ArmourItemTemplate* setTemplate);
	~ArmourItem(){}

	int GetSlot() const {return ((ArmourItemTemplate*)pTemplate)->slot;}
	int GetMaxDurability() const {return ((ArmourItemTemplate*)pTemplate)->maxDurability;}
	int GetDamageMitigationMod() const {return ((ArmourItemTemplate*)pTemplate)->damageMitigationMod;}
	int GetFireMitigationMod() const {return ((ArmourItemTemplate*)pTemplate)->fireMitigationMod;}
	int GetStrengthMod() const {return ((ArmourItemTemplate*)pTemplate)->strengthMod;}
	int GetSpeedMod() const {return ((ArmourItemTemplate*)pTemplate)->speedMod;}
	int GetPerceptionMod() const {return ((ArmourItemTemplate*)pTemplate)->perceptionMod;}
	int GetStaminaMod() const {return ((ArmourItemTemplate*)pTemplate)->staminaMod;}

protected:
	int durability;
};
#endif
