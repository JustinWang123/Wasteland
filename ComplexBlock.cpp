#include "ComplexBlock.h"
#include "ComplexBlockTemplate.h"

ComplexBlock :: ComplexBlock(ComplexBlockTemplate* setElementTemplate, vector3di setPos, vector3di setDirr)
{
	name = setElementTemplate->name;
	pTemplate = setElementTemplate;

	sceneNode = IRR->sceneMgr->addMeshSceneNode(GetTemplate()->mesh);
	sceneNode->setScale(vector3df(0.5f,0.5f,0.5f));
	sceneNode->setPosition(vector3df(setPos.X + 0.5f, setPos.Y + 0.5, setPos.Z + 0.5));
	sceneNode->setMaterialFlag(EMF_LIGHTING, false);

	for(int socket = FIRST_SOCKET; socket < LAST_SOCKET; socket++)
		sockets[socket].Init(this, socket);
}

ComplexBlock :: ~ComplexBlock()
{
	sceneNode->remove();
}

std::string ComplexBlock :: GetDebugString(float x, float y, float z)
{
	return name + "\n";
}

// ElectricalAccessors:
 void ComplexBlock :: ConsumePower(float amount)
 {
 }

 float ComplexBlock :: ProvidePower(float desiredAmount)
 {
	 return 0.0f;
 }

 bool ComplexBlock :: IsElectrical()
 {
	 return pTemplate->isElectricalComponenet;
 }


