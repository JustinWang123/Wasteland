#ifndef ZOMBIE_INSTANCE
#define ZOMBIE_INSTANCE

#include "IrrWrapper.h"
#include "CharacterInstance.h"

enum ZombieState
{
	IDLE = 0,
	WANDERING,
	PURSUING
};

class ZombieInstance : public CharacterInstance
{
public:
	ZombieInstance(vector3df position);
	~ZombieInstance();

	virtual void Update();
	virtual void Damage(int amount, vector3df force);

	// Message handling:
	virtual bool HandleMessage(Telegram& msg);


private:
	virtual void CheckDeath();
	virtual void ChangeState(ZombieState state);

	void IdleState();
	void WanderState();
	void PursuitState();

	vector3df	playerLastSeenPos;

	int			timer;
	int			attackDelay;
};
#endif
