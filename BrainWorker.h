#ifndef BRAIN_WORKER_H
#define BRAIN_WORKER_H

#include "CompoundCharacterCommand.h"

class BrainWorker : public CompoundCharacterCommand
{
public:

	BrainWorker(CharacterInstance* owner)
		: CompoundCharacterCommand(owner, BRAIN_WORKER) {}

	~BrainWorker() {}

	CommandState	Process();
	void			Activate();
	void			Terminate(){}
	virtual bool	HandleMessage(const Telegram& msg);

};

#endif
