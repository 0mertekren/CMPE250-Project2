#include "Desk.h"

Desk::Desk(uint16_t desk_id, float desk_time, bool status)
: desk_id(desk_id), desk_time(desk_time), is_desk_available()
{
}

void Desk::setDeskStatus(bool status)
{
	is_desk_available = status;
}

bool Desk::getDeskStatus()
{
	return this->is_desk_available;
}

float Desk::getDeskTime()
{
	return desk_time;
}
