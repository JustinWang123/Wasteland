#include "ItemInstance.h"
#include "GameManager.h"
#include "ItemManager.h"


ItemInstance :: ItemInstance(BaseItem* setItem, int setAmount, vector3df position, vector3df rotation)
{
	Init(setItem, setAmount, position, rotation);
}

ItemInstance :: ItemInstance(BaseItem* setItem, vector3df position, vector3df rotation)
{
	Init(setItem, 1, position, rotation);
}

void ItemInstance :: Init(BaseItem* setItem, int setAmount, vector3df position, vector3df rotation)
{
	pItem = setItem;
	itemAmount = setAmount;
	bitMask = BITMASK_ITEM;

	if(pItem->GetMesh() != 0)
	{
		sceneNode = IRR->sceneMgr->addMeshSceneNode(pItem->GetMesh(), 0, BITMASK_ITEM, position, rotation, vector3df(0.5, 0.5, 0.5));
		sceneNode->setMaterialFlag(EMF_LIGHTING, false);
		sceneNode->setMaterialFlag(EMF_FOG_ENABLE, false);

		// Collision Triangle Selector for collision:
		ITriangleSelector* collisionSelector = IRR->sceneMgr->createTriangleSelector(sceneNode->getMesh(), sceneNode);
		sceneNode->setTriangleSelector(collisionSelector);
		collisionSelector->drop();
	}
	else
	{
		sceneNode = 0;
	}
}

ItemInstance :: ~ItemInstance()
{
	if(sceneNode != 0)
		sceneNode->remove();
}

 void ItemInstance :: Update()
 {
 }
