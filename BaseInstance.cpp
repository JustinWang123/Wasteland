#include "BaseInstance.h"
#include "InstanceManager.h"

int BaseInstance :: nextID = 0;

BaseInstance :: BaseInstance()
{
	id = nextID++;
	InstanceMgr->Register(this);
}

BaseInstance :: ~BaseInstance()
{
	InstanceMgr->Remove(this);
}

bool BaseInstance :: IsUnderScreenPos(vector2di screenPos)
{
    if(IRR->collisionMgr->getSceneNodeFromScreenCoordinatesBB(screenPos) == sceneNode)
    {
        return true;
    }
    else
    {
        return false;
    }
}
