#ifndef DESK_H
#define DESK_H

#include <cstdint>
#include "Hacker.h"

class Desk
{
private:
	uint16_t desk_id;
	float desk_time;
	bool is_desk_available;
public:
	void setDeskStatus(bool status);
	bool getDeskStatus();
	Desk(uint16_t desk_id, float desk_time, bool status=true);
	float getDeskTime();

};
#endif
