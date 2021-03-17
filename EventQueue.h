#ifndef EventQueue_H
#define EventQueue_H

#include <cstdint>
#include "Hacker.h"


// TODO: maybe add enter desk to service enum to seperate leave cases..
class EventQueue
{
private:
	uint16_t hacker_id;
    EVENT_ENUM event;
    float event_time;
    uint16_t commit_length;
public:
    //EventQueue();
    ~EventQueue();
    EventQueue (uint16_t hacker_id, EVENT_ENUM event, float event_time, uint16_t commit_length=0);
    float getEventTime();
    uint16_t getCommitLength();
    EVENT_ENUM getEventType();
    uint16_t getHackerID();
    EventQueue(const EventQueue &ob);
    EventQueue& operator=(const EventQueue &ob);
};


#endif /* EventQueue_H */
