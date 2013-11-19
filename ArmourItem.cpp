#include "ArmourItem.h"

ArmourItem :: ArmourItem(ArmourItemTemplate* setTemplate)
	: BaseItem(setTemplate)
{
	durability = setTemplate->maxDurability;
}