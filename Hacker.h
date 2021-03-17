#ifndef HACKER_H
#define HACKER_H

#include <cstdint>
#include <vector>
#include <map>

using namespace std;
enum EVENT_ENUM{ 
    A_HACKER_ARRIVES,
    A_HACKER_COMMITS,
    A_HACKER_ENTERS_STICKER_QUEUE,    
    A_HACKER_LEAVES_STICKER_QUEUE,
    A_HACKER_ENTERS_HOODIE_QUEUE,    
    A_HACKER_LEAVES_HOODIE_QUEUE
    };
enum DESK_ENUM{
	STICKER_DESK,
	HOODIE_DESK
};
class Hacker
{
private:
    float arrival_time;
    uint16_t number_of_commits;
    uint16_t hacker_id;
    uint8_t num_of_gifts;
    static uint16_t num_of_total_gifts;
    static uint16_t num_of_total_commits;

public:

    Hacker( const float arrival_time = -1);
    ~Hacker();
    
    void setArrivalTime(float time);
    void pushCommit(void);
    void setHackerID(uint16_t id);
    void incrementNumofGifts();
    uint8_t getNumofGifts();
    uint16_t getNumofTotalGifts();
    uint16_t getHackerID();
    uint16_t getNumberOfCommits();
    uint16_t getNumofTotalCommits();
    float getArrivalTime();

    std::vector<pair<EVENT_ENUM, float>> sticker_q_events;
    std::vector<pair<EVENT_ENUM, float>> hoodie_q_events;

    float last_q_enter_time;
    float last_q_exit_time;

};

#endif /* HACKER_H */
