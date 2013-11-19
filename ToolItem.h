#ifndef TOOL_ITEM_H
#define TOOL_ITEM_H

#include "BaseItem.h"
#include "ToolItemTemplate.h"

class ToolItem : public BaseItem
{
public:
	ToolItem(ToolItemTemplate* setTemplate);
	~ToolItem(){}

	virtual void Update();
	virtual void Use(CharacterInstance* owner);

protected:
	ToolItemTemplate* GetTemplate() {return (ToolItemTemplate*)pTemplate;}

	float currentUseDelay;
	int durability;
};
#endif
