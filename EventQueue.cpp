#include <iostream>
#include "EventQueue.h"
#include <cstdint>

EventQueue::EventQueue (uint16_t hacker_id, EVENT_ENUM event,float event_time, uint16_t commit_length):
hacker_id(hacker_id), event(event), event_time(event_time), commit_length(commit_length)
{
}

EventQueue::EventQueue(const EventQueue &ob)
: hacker_id(ob.hacker_id), event(ob.event), event_time(ob.event_time),
  commit_length(ob.commit_length)
{
}

EventQueue& EventQueue::operator=(const EventQueue &ob)
{
	hacker_id = ob.hacker_id;
	event = ob.event;
	event_time = ob.event_time;
	commit_length = ob.commit_length;

	return *this;
}
/*EventQueue::EventQueue()
{
}*/

EventQueue::~EventQueue()
{
}

float EventQueue::getEventTime()
{
    return event_time;
}

EVENT_ENUM EventQueue::getEventType()
{
    return event;
}

uint16_t EventQueue::getHackerID()
{
    return hacker_id;
}

uint16_t EventQueue::getCommitLength()
{
    return commit_length;
}
