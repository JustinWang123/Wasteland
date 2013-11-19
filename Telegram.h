#ifndef TELEGRAM
#define TELEGRAM
#include <math.h>

enum msgIDs
{
	I_AM_DEAD = 0
};

struct Telegram
{
	Telegram(double setTime, int setSender, int setReceiver, int setMsg, void*  setExtraInfo = 0):
		sender(setSender), receiver(setReceiver), msg(setMsg), dispatchTime(setTime), extraInfo(setExtraInfo){}

	int sender;
	int receiver;
	int msg;
	double dispatchTime;
	void* extraInfo;


};

//these telegrams will be stored in a priority queue. Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SMALLEST_DELAY = 0.25;




inline bool operator == (const Telegram& t1, const Telegram& t2)
{
	return ( fabs( (t1.dispatchTime - t2.dispatchTime) ) < SMALLEST_DELAY) &&
		(t1.sender == t2.sender)        &&
		(t1.receiver == t2.receiver)    &&
		(t1.msg == t2.msg);
}

inline bool operator < (const Telegram& t1, const Telegram& t2)
{
	if (t1 == t2)
	{
		return false;
	}
	else
	{
		return  (t1.dispatchTime < t2.dispatchTime);
	}
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
  return *(T*)(p);
}

#endif
