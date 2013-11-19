#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include "IrrWrapper.h"
#include "Telegram.h"
#include "DamageHitMessage.h"
#include <set>
class   BaseInstance;

const double	SEND_MSG_IMMEDIATELY = 0.0;
const int		NO_ADDITIONAL_INFO   = 0;
const int		SENDER_ID_IRRELEVANT = -1;

enum MessageType
{
	MESSAGE_TYPE_DAMAGE_HIT,
	MESSAGE_TYPE_WEAPON_RECOIL,
	MESSAGE_TYPE_GIVE_ITEM
};




class   MessageManager
{
public:
	static MessageManager* Instance();

	void DispatchMsg(double delay, int senderID, int receiverID, int msg, void* extraInfo);
	void DispatchDelayedMsgs();

private:
	MessageManager(){}

	std::set<Telegram> priorityQ;
	void Discharge(BaseInstance* receiver, Telegram &msg);
};

#define MessageMgr MessageManager::Instance()
#endif
