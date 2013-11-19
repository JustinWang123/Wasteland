#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include "BaseItem.h"
#include "BlockItemTemplate.h"

class BlockItem : public BaseItem
{
public:
	BlockItem(int setID, BlockItemTemplate* setTemplate);
	~BlockItem(){}

protected:

};
#endif
