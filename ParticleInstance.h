#ifndef PARTICLE_INSTANCE_H
#define PARTICLE_INSTANCE_H

#include <string>
#include "IrrWrapper.h"

class ParticleInstance
{
public:
	ParticleInstance(std::string text, vector3df position, vector3df velocity, vector3df acceleration, int setLife);
	~ParticleInstance();
	void Update();

	bool		removeMe;
private:
	IBillboardSceneNode* sceneNode;
	vector3df	velocity;
	vector3df	acceleration;
	int			life;

};

#endif
