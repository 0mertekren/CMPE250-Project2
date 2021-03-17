#include "Hacker.h"
#include <iostream>

uint16_t Hacker::num_of_total_gifts;
uint16_t Hacker::num_of_total_commits;

Hacker::Hacker(const float arrival_time) : arrival_time(arrival_time), number_of_commits(0) , num_of_gifts(0)
{
}

float Hacker::getArrivalTime()
{
    return arrival_time;
}

void Hacker::setArrivalTime(float time)
{
    arrival_time = time;
}


uint16_t Hacker::getNumberOfCommits()
{
    return number_of_commits;
}

void Hacker::pushCommit(void)
{
    number_of_commits++;
    num_of_total_commits++;
}

Hacker::~Hacker()
{
}

uint16_t Hacker::getHackerID()
{
    return hacker_id;
}

void Hacker::setHackerID(uint16_t id)
{
    hacker_id = id;    
}

void Hacker::incrementNumofGifts()
{
    num_of_gifts++;
    Hacker::num_of_total_gifts++;
}

uint8_t Hacker::getNumofGifts()
{
    return num_of_gifts;
}

uint16_t Hacker::getNumofTotalGifts()
{
    return num_of_total_gifts;
}

uint16_t Hacker::getNumofTotalCommits()
{
    return num_of_total_commits;
}
