#include "ParticleInstance.h"

ParticleInstance :: ParticleInstance(std::string text, vector3df setPos, vector3df setVel, vector3df setAcc, int setLife)
{
	stringw text2(text.c_str());

	sceneNode = IRR->sceneMgr->addBillboardTextSceneNode(IRR->gui->getFont("tex/font.bmp"), text2.c_str());
	sceneNode->setSize(dimension2df(0.1,0.1));
	sceneNode->setPosition(setPos);
	velocity = setVel;
	acceleration = setAcc;
	life = setLife;
	removeMe = false;
}

ParticleInstance :: ~ParticleInstance()
{
	sceneNode->remove();
}

void ParticleInstance :: Update()
{
	velocity += acceleration;

	sceneNode->setPosition(sceneNode->getPosition() + velocity);

	life--;

	if(life <= 0)
	{
		removeMe = true;
	}
}