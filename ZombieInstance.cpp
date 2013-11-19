#include "ZombieInstance.h"
#include "GameManager.h"
#include "Perameters.h"
#include "MessageManager.h"
#include "InstanceManager.h"
#include <math.h>

ZombieInstance :: ZombieInstance(vector3df position)
{
	this->position = position;

	sceneNode = IRR->sceneMgr->addMeshSceneNode(IRR->sceneMgr->getMesh("mdl/Zombie.irrmesh"), 0, BITMASK_CHARACTER);
	sceneNode->setMaterialFlag(EMF_LIGHTING, false);
	sceneNode->setPosition(position);

	// Collision Triangle Selector for collision:
	ITriangleSelector* collisionSelector = IRR->sceneMgr->createTriangleSelector(sceneNode->getMesh(), sceneNode);
    sceneNode->setTriangleSelector(collisionSelector);
	collisionSelector->drop();

	timer = ZOMBIE_IDLE_TIME;
	currentHealth = ZOMBIE_MAX_HEALTH;
	removeMe = false;
	attackDelay = 0;
}

ZombieInstance :: ~ZombieInstance()
{
	sceneNode->remove();
}

bool ZombieInstance :: HandleMessage(Telegram& msg)
{

	// Pass any unhandled messages up:
	if(CharacterInstance::HandleMessage(msg))
	{
		return true;
	}

	return false;
}

void ZombieInstance :: Update()
{
	
	/*
	if(state == IDLE)
	{
		IdleState();
	}
	else if(state == WANDERING)
	{
		WanderState();
	}
	else if(state == PURSUING)
	{
		PursuitState();
	}

	velocity = heading * ZOMBIE_ACCELERATION;
	velocity *= ZOMBIE_FRICTION; // friction

	// Cap wander speed:
	if(velocity.getLength() > moveSpeed)
		velocity = velocity.normalize() * moveSpeed;


	// Gravity:
	if( !MapMgr->IsBlockSolid(position - vector3df(0,0.1,0)) )
	{
		verticalVelocity -= 0.01;
	}
	else
	{
		verticalVelocity = 0;
	}

	
	sceneNode->setPosition(position);

	// Handle rotation:
	if(velocity.getLength() > 0.001f)
	{
		float angle = IRR->GetAngle(0,0,velocity.X, velocity.Z);

		sceneNode->setRotation(vector3df(0,angle,0));
	}
	*/
}

void ZombieInstance ::IdleState()
{
	
	timer -= 1;

	if(timer <= 0)
	{
		//ChangeState(WANDERING);
	}

	/*
	vector3df playerPos = InstanceMgr->GetPlayerInstance()->GetPosition();
	float distance = playerPos.getDistanceFrom(position);

	if(distance < ZOMBIE_AGRO_RANGE
	&& MapMgr->IsBlockOpen(position + vector3df(0,2,0), playerPos - (position + vector3df(0,2,0)), distance))
	{
		ChangeState(PURSUING);
	}
	*/
}

void ZombieInstance ::WanderState()
{
	timer -= 1;

	if(timer <= 0)
	{
		//ChangeState(IDLE);
	}

	/*
	// Cap wander speed:
	if(velocity.getLength() > ZOMBIE_WANDER_SPEED)
		velocity = velocity.normalize() * ZOMBIE_WANDER_SPEED;

	
	vector3df playerPos = InstanceMgr->GetPlayerInstance()->GetPosition();
	float distance = playerPos.getDistanceFrom(position);

	if(distance < ZOMBIE_AGRO_RANGE 
	&& MapMgr->IsBlockOpen(position + vector3df(0,2,0), playerPos - (position + vector3df(0,2,0)), distance))
	{
		ChangeState(PURSUING);
	}
	
	// Velocity normal is used to create a radius for collision detection around zombie:
	vector3df velocityNormal = velocity;
	velocityNormal.normalize();
	velocityNormal *= 0.3;

	// If position in front of feet are open:
	if(!MapMgr->IsBlockSolid(position + vector3df(0,0.1,0) + velocityNormal + velocity)
	&& !MapMgr->IsBlockSolid(position + vector3df(0,1.5,0) + velocityNormal + velocity))
	{
		// Move forward normally:
		position += vector3df(velocity.X, verticalVelocity, velocity.Z);
	}
	else if(!MapMgr->IsBlockSolid(position + vector3df(0,1.5,0) + velocityNormal + velocity)
		&& !MapMgr->IsBlockSolid(position + vector3df(0,2.5,0) + velocityNormal + velocity))
	{
		// Move forward normally:
		// Also add vertical velocity to climb:
		verticalVelocity += 0.02;
		position += vector3df(velocity.X, verticalVelocity, velocity.Z);
	}
	else
	{
		ChangeState(IDLE);
	}
	*/
}

void ZombieInstance ::PursuitState()
{
	/*
	
	vector3df playerPos = InstanceMgr->GetPlayerInstance()->GetPosition();
	float distance = playerPos.getDistanceFrom(position);

	// If player is visible set his last seen position;
	if(MapMgr->IsOpen(position + vector3df(0,2,0), playerPos - (position + vector3df(0,2,0)), distance))
	{
		playerLastSeenPos = playerPos;
	}

	// Always head to the players last seen position:
	vector3df toPlayer = playerLastSeenPos - position;
	toPlayer.normalize();
	toPlayer.Y = 0;

	heading = toPlayer;

	// Cap pursuit speed:
	if(velocity.getLength() > ZOMBIE_PURSUIT_SPEED)
		velocity = velocity.normalize() * ZOMBIE_PURSUIT_SPEED;


	
	// Attackng if in attack range:
	if(distance < 2 && attackDelay <= 0)
	{
		GManager->GetPlayer()->Damage(ZOMBIE_DAMAGE, toPlayer);
		attackDelay = ZOMBIE_ATTACK_DELAY;
	}

	if(attackDelay > 0)
		attackDelay--;

	// Stop attacking if:
	// I am at the players last seen position and cannot see the player
	// player is dead
	if((GManager->CheckMapCollision(position + vector3df(0,2,0), playerPos - (position + vector3df(0,2,0)), distance)
		&& position.getDistanceFrom(playerLastSeenPos) < 1.0f))
	{
		ChangeState(IDLE);
	}
	
	// 3 Cases in which zombie should break down wall:
	//
	//			X			1
	//		Z		X	X	2
	//		Z	X	X		3

	

	// Velocity normal is used to create a radius for collision detection around zombie:
	vector3df velocityNormal = velocity;
	velocityNormal.normalize();
	velocityNormal *= 0.3;

	// If position in front of feet are open:
	if(GManager->IsBlockOpen(position+ vector3df(0,0.1,0) + velocityNormal + velocity)
	&& GManager->IsBlockOpen(position+ vector3df(0,1.5,0) + velocityNormal + velocity))
	{
		// Move forward normally:
		position += vector3df(velocity.X, verticalVelocity, velocity.Z);
	}
	else if(GManager->IsBlockOpen(position + vector3df(0,1.5,0) + velocityNormal + velocity)
		&& GManager->IsBlockOpen(position + vector3df(0,2.5,0) + velocityNormal + velocity))
	{
		// Move forward normally:
		// Also add vertical velocity to climb:
		verticalVelocity += 0.05;
		position += vector3df(velocity.X, verticalVelocity, velocity.Z);
	}
	else if(attackDelay <= 0)
	{
		vector3di block1Pos;
		vector3di block2Pos;
		vector3di block3Pos;
		bool block1 = GManager->GetFirstClosedBlock(position + vector3df(0,2.5f,0), heading, 1.0f, block1Pos);
		bool block2 = GManager->GetFirstClosedBlock(position + vector3df(0,1.5f,0), heading, 1.0f, block2Pos);
		bool block3 = GManager->GetFirstClosedBlock(position + vector3df(0,0.5f,0), heading, 1.0f, block3Pos);

		if(block1 && block3)
		{
			GManager->DamageBlock(block3Pos, ZOMBIE_DAMAGE);
			attackDelay = ZOMBIE_ATTACK_DELAY;
		}
		else if(block2 && block3)
		{
			GManager->DamageBlock(block3Pos, ZOMBIE_DAMAGE);
			attackDelay = ZOMBIE_ATTACK_DELAY;
		}
		else if(block2)
		{
			GManager->DamageBlock(block2Pos, ZOMBIE_DAMAGE);
			attackDelay = ZOMBIE_ATTACK_DELAY;
		}
	}
	*/
}

void ZombieInstance :: Damage(int amount, vector3df force)
{
	currentHealth -= amount;

	// Brief stun:
	ChangeState(IDLE);
	timer = amount;

	force.normalize();
	velocity += force * (amount / 100.0f);

	CheckDeath();
}

void ZombieInstance :: CheckDeath()
{
	if(currentHealth <= 0)
	{
		removeMe = true;
	}
}

void ZombieInstance :: ChangeState(ZombieState setState)
{
	/*
	if(setState == IDLE)
	{
		state = IDLE;

		velocity.X = 0;
		velocity.Y = 0;
		heading.X = 0;
		heading.Z = 0;
		moveSpeed = 0;

		timer = ZOMBIE_IDLE_TIME;
	}
	else if(setState == WANDERING)
	{
		state = WANDERING;
			
		double angle = (rand() % 360) * PI/180;

		heading.X = sin(angle);
		heading.Z = cos(angle);
		moveSpeed = ZOMBIE_WANDER_SPEED;

		sceneNode->setRotation(vector3df(0, (angle * 180/PI) + 180, 0));

		timer = ZOMBIE_WANDER_TIME;
	}
	else if(setState == PURSUING)
	{
		state = PURSUING;
		playerLastSeenPos = InstanceMgr->GetPlayerInstance()->GetPosition();
		moveSpeed = ZOMBIE_PURSUIT_SPEED;
		timer = 0;
	}
	*/
}