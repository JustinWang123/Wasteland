#include "MessageManager.h"
#include "BaseInstance.h"
#include "InstanceManager.h"

MessageManager* MessageManager :: Instance()
{
  static MessageManager instance;

  return &instance;
}

void MessageManager :: Discharge(BaseInstance* receiver, Telegram &msg)
{
	receiver->HandleMessage(msg);
}


//---------------------------- DispatchMsg ---------------------------
//
//  given a message, a receiver, a sender and any time delay, this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void MessageManager :: DispatchMsg(double delay, int senderID, int receiverID, int msg, void* extraInfo)
{
	//get a pointer to the receiver
	BaseInstance* receiver = InstanceMgr->GetInstanceFromID(receiverID);

	//create the telegram
	Telegram telegram(0.0, senderID, receiverID, msg, extraInfo);

	//if there is no delay, route telegram immediately
	if (delay <= 0.0)
	{
		//send the telegram to the recipient immediatly:
		Discharge(receiver, telegram);
	}
	//else calculate the time when the telegram should be dispatched
	else
	{
		double CurrentTime = IRR->GetTime();

		telegram.dispatchTime = CurrentTime + delay;

		//and put it in the queue
		priorityQ.insert(telegram);
	}
}


//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
void MessageManager::DispatchDelayedMsgs()
{
    /*
	//first get current time
	double CurrentTime = IRR->GetTime();

	// now peek at the queue to see if any telegrams need dispatching.
	// remove all telegrams from the front of the queue that have gone
	// past their sell by date
	while(!priorityQ.empty()
	&&    (priorityQ.begin()->dispatchTime < CurrentTime)
	&&    (priorityQ.begin()->dispatchTime > 0) )
	{
		//read the telegram from the front of the queue
		std::set<Telegram>::iterator it = priorityQ.begin();


		Telegram& telegram( (*it) );

		//get a pointer to the receiver
		BaseInstance* receiver = InstanceMgr->GetInstanceFromID(telegram.receiver);

		//send the telegram to the recipient
		Discharge(receiver, telegram);

		//remove it from the queue
		priorityQ.erase(priorityQ.begin());
	}
	*/
}
