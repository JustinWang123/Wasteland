#ifndef CONSUMABLE_ITEM_H
#define CONSUMABLE_ITEM_H

#include "BaseItem.h"
#include "ConsumableItemTemplate.h"

class ConsumableItem : public BaseItem
{
public:
	ConsumableItem(int setID, ConsumableItemTemplate* setTemplate);
	~ConsumableItem(){}

protected:

};
#endif
