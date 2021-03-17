// TODO: hocanin dedigi memory  toolu
// TODO: algorithmler farklı dosyaya itelenecek

#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include "main.h"
#include "Hacker.h"
#include <queue>
#include <vector>
#include <fstream>
#include <list>
#include "EventQueue.h"
#include <map>
#include "Desk.h"

using namespace std;

uint16_t number_of_hackers;
uint16_t number_of_commits;
uint16_t number_of_attempts;
uint16_t number_of_sticker_Q ;
uint16_t number_of_hoodie_Q;
uint16_t number_of_total_gifts = 0;
uint16_t avg_waiting_time = 0;
uint16_t max_len_of_sticker_q = 0;
uint16_t max_len_of_hoodie_q = 0;
uint16_t invalid_att_for_sticker_q = 0;
uint16_t exceed_att_for_sticker_q = 0;
vector<uint32_t> commit_lengths;
float max_sec_for_queue=-1;
float last_event_time;

bool hoodie_desk_empty = true;
bool sticker_desk_empty = true;

std::vector<Desk> sticker_desks;
std::vector<Desk> hoodie_desks;

#define FLOAT_LIMIT 0.00001
#define MAX_NUM_OF_GIFTS_PER_HACKER 3
#define MIN_COMMIT_LIMIT 3
#define CODE_LIMIT_FOR_VALID_COMMIT 20
struct stickercompare
{
	bool operator()(Hacker &h1, Hacker &h2)
	{
		if (abs(h1.last_q_enter_time - h2.last_q_enter_time) > FLOAT_LIMIT)
			return h1.last_q_enter_time > h2.last_q_enter_time;
		else
			return h1.getHackerID() > h2.getHackerID();
	}
};

struct hoodiecompare
{
	bool operator()(Hacker &h1, Hacker &h2)
	{
		if (abs(h1.getNumberOfCommits() - h2.getNumberOfCommits() ) > FLOAT_LIMIT)
			return h1.getNumberOfCommits() < h2.getNumberOfCommits();

		else if (abs(h1.last_q_enter_time - h2.last_q_enter_time) > FLOAT_LIMIT)
			return h1.last_q_enter_time > h2.last_q_enter_time;

		else
			return h1.getArrivalTime() > h2.getArrivalTime(); // not actually arrival time
	}
};

struct heap_algo
{
	bool operator()(EventQueue &d1, EventQueue &d2)
	{
		uint16_t h1 = d1.getHackerID();
		uint16_t h2 = d2.getHackerID();

		if (abs(d1.getEventTime() - d2.getEventTime()) > FLOAT_LIMIT)
			return d1.getEventTime() > d2.getEventTime();
		else
			return h1 > h2;
	}
};

priority_queue<Hacker, vector<Hacker>, stickercompare> sticker_queue;
priority_queue<Hacker, vector<Hacker>, hoodiecompare> hoodie_queue;

void inputParse(std::vector<Hacker> &hckr,
		std::priority_queue<EventQueue, vector<EventQueue>, heap_algo> &event_heap,
		char **argv);
void outputParse(std::vector<Hacker> &hckr,
		std::priority_queue<EventQueue, vector<EventQueue>, heap_algo> &event_heap,
		char **argv);


int main(int argc, char **argv)
{
	priority_queue<EventQueue, vector<EventQueue>, heap_algo> event_heap;
	std::vector<Hacker> hackers;
	float curr_time;
	float next_time;

	inputParse(hackers, event_heap, argv);

	//print(hackers);

	while (event_heap.empty() == false)
	{
		//    HACKER ENTERS STICKER QUEUE
		EventQueue curr_event = event_heap.top();
		event_heap.pop();
		curr_time = curr_event.getEventTime();

		Hacker& curr_hacker = hackers[curr_event.getHackerID() - 1];

		if(abs(curr_time - 46) < 0.01)
		{
			cout << " ";
			int a;
			int b;
			int c;
		}


		switch (curr_event.getEventType())
		{
		case A_HACKER_ARRIVES:
			break;
		case A_HACKER_COMMITS:
			commit_lengths.push_back(curr_event.getCommitLength());
			if(curr_event.getCommitLength() >= CODE_LIMIT_FOR_VALID_COMMIT)
				curr_hacker.pushCommit();
			break;
		case A_HACKER_ENTERS_STICKER_QUEUE:
			last_event_time = curr_time;
			curr_hacker.last_q_enter_time = curr_time;
			if(curr_hacker.getNumberOfCommits() < MIN_COMMIT_LIMIT )
			{
				invalid_att_for_sticker_q++;
				break;
			}

			if(curr_hacker.getNumofGifts() >= MAX_NUM_OF_GIFTS_PER_HACKER)
			{
				exceed_att_for_sticker_q++;
				break;
			}

			curr_hacker.sticker_q_events.push_back ( pair<EVENT_ENUM,float> (A_HACKER_ENTERS_STICKER_QUEUE, curr_time));
			if (sticker_desk_empty == true)
			{
				sticker_desk_empty = false;
				next_time = curr_time + 3; //TODO: 3 degil de in alinacak
				
				curr_hacker.last_q_enter_time = curr_time;

				event_heap.push(EventQueue(curr_hacker.getHackerID(),
						A_HACKER_LEAVES_STICKER_QUEUE,
						next_time));
			}
			else
			{
				sticker_queue.push(curr_hacker);
				if(sticker_queue.size() > max_len_of_sticker_q)
					max_len_of_sticker_q = sticker_queue.size();
			}
			break;
		case A_HACKER_LEAVES_STICKER_QUEUE:
			curr_hacker.last_q_exit_time = curr_time;			
			curr_hacker.last_q_enter_time = curr_time;

			curr_hacker.sticker_q_events.push_back ( pair<EVENT_ENUM,float> (A_HACKER_LEAVES_STICKER_QUEUE, curr_time));
			curr_hacker.hoodie_q_events.push_back ( pair<EVENT_ENUM,float> (A_HACKER_ENTERS_HOODIE_QUEUE, curr_time));

			if (hoodie_desk_empty == true)
			{
				hoodie_desk_empty = false;
				next_time = curr_time + 4;
				event_heap.push(EventQueue(curr_hacker.getHackerID(),
						A_HACKER_LEAVES_HOODIE_QUEUE,
						next_time));
			}
			else
			{
				hoodie_queue.push(curr_hacker);
			}
			if (sticker_queue.empty() == false)
			{
				// hacker in sticker q schd to leave
				Hacker temp_hacker = sticker_queue.top();
				sticker_queue.pop();
				next_time = curr_time  + 3;
				event_heap.push(EventQueue(temp_hacker.getHackerID(),
						A_HACKER_LEAVES_STICKER_QUEUE,
						next_time));
			}
			else
			{
				sticker_desk_empty = true;
			}
			break;
		case A_HACKER_ENTERS_HOODIE_QUEUE:
			break;
		case A_HACKER_LEAVES_HOODIE_QUEUE :		
			curr_hacker.last_q_exit_time = curr_time;
			curr_hacker.incrementNumofGifts();
			curr_hacker.hoodie_q_events.push_back ( pair<EVENT_ENUM,float> (A_HACKER_LEAVES_HOODIE_QUEUE, curr_time));
			if(hoodie_queue.empty() == false)
			{
				if(hoodie_queue.size() > max_len_of_hoodie_q)
					max_len_of_hoodie_q = hoodie_queue.size();

				Hacker temp_hacker = hoodie_queue.top();
				hoodie_queue.pop();
				next_time = curr_time + 4;
				event_heap.push(EventQueue(temp_hacker.getHackerID(),
						A_HACKER_LEAVES_HOODIE_QUEUE,
						next_time));
			}
			else
			{
				hoodie_desk_empty = true;
			}
			break;
		default:
			break;
			// curr_hacker_2
			// hoodieq_find(curr_hacker); hoodie'sini aldı.
			// curr_hacker 1 gone...

			// hoodieq_top curr_hacker_2
			// new_event(curr_hacker_2, A_HACKER_LEAVES_HOODIE_QUEUE, leavetime)

			// hoodieq_pop
			// hoodieq_push if not empty
			// push commit'i yuksek olan hacker..

		}
	}

	outputParse(hackers, event_heap, argv);

	return 0;
}

bool empty_desk_check(DESK_ENUM desk, float& waiting_time)
{
	switch(desk)
	{

	case STICKER_DESK:
		for (std::vector<Desk>::iterator it = sticker_desks.begin() ; it != sticker_desks.end(); ++it)
		{
			if(it->getDeskStatus() == true)
			{
				it->setDeskStatus(false);
				waiting_time = it->getDeskTime();
				return true;
			}
		}
		return false;
	case HOODIE_DESK:
		for (std::vector<Desk>::iterator it = hoodie_desks.begin() ; it != hoodie_desks.end(); ++it)
		{
			if(it->getDeskStatus() == true)
			{
				it->setDeskStatus(false);
				waiting_time = it->getDeskTime();
				return true;
			}
		}
		return false;
	default:
		break;
		return true;
	}
}

void outputParse(std::vector<Hacker> &hckr,
		std::priority_queue<EventQueue, vector<EventQueue>, heap_algo> &event_heap,
		char **argv)
{
	ofstream myfile;
	float avg_wait_time_sticker_q = 0;
	float avg_wait_time_hoodie_q = 0;
	myfile.open (argv[2]);
	float sticker_time = 0;
	uint64_t sticker_time_trials = 0;
	float hoodie_time = 0;
	uint64_t hoodie_time_trials = 0;

	/*1*/cout << max_len_of_sticker_q << "\n";
	/*2*/cout << max_len_of_hoodie_q << "\n";
	/*3*/cout << hckr[0].getNumofTotalGifts() / 3 << "\n";

	// ;TODO  hacker sayısını fixle
	float temp_sec_for_queue;

	for (auto  i = 0; i <3 ; i++)
	{
		for ( auto j = hckr[i].sticker_q_events.begin(); j != hckr[i].sticker_q_events.end(); j++)
		{
			auto next = j;
			if ( next != hckr[i].sticker_q_events.end())
			{
				next++;
			}

			if (j->first == A_HACKER_ENTERS_STICKER_QUEUE && next->first == A_HACKER_LEAVES_STICKER_QUEUE)
			{
				sticker_time += next->second - j->second;
				sticker_time_trials++;

			}
		}
		for ( auto j = hckr[i].hoodie_q_events.begin(); j != hckr[i].hoodie_q_events.end(); j++)
		{
			auto next = j;
			if ( next != hckr[i].hoodie_q_events.end())
			{
				next++;
			}

			if (j->first == A_HACKER_ENTERS_HOODIE_QUEUE && next->first == A_HACKER_LEAVES_HOODIE_QUEUE)
			{
				hoodie_time += next->second - j->second;
				hoodie_time_trials++;
			}
		}
	}
	float avg_len_of_commits=0;
	for(auto k=commit_lengths.begin(); k<commit_lengths.end(); k++)
	{
		avg_len_of_commits += *k;
	}
	avg_len_of_commits /= (float)hckr[0].getNumofTotalCommits();
	auto turn_around_time = (float)(sticker_time + hoodie_time)/ hckr[0].getNumofTotalGifts() ;
	sticker_time -=(hckr[0].getNumofTotalGifts()*3);
	hoodie_time  -=(hckr[0].getNumofTotalGifts()*4);
	avg_wait_time_sticker_q = sticker_time/sticker_time_trials;
	float avg_commits = (float) hckr[0].getNumofTotalCommits() / number_of_hackers;
	/*<<"4. " */cout<< avg_wait_time_sticker_q  <<"\n";
	avg_wait_time_hoodie_q = hoodie_time/hckr[0].getNumofTotalGifts();
	/*<<"5. " */cout<< avg_wait_time_hoodie_q << "\n";
	/*<<"6. " */cout<< avg_commits << "\n";
	/*7. change length of commits*/ cout<< avg_len_of_commits <<"\n";
	/*8. avg turnaround time*/ cout << turn_around_time << "\n";
	/*<<"9. " */cout<< invalid_att_for_sticker_q << "\n";
	/*<<"10. " */cout<< exceed_att_for_sticker_q << "\n";
	/*<<"4. " */cout<< exceed_att_for_sticker_q << "\n";
	/*11. ID of the hacker who spent the most time in the queues and the waiting time of that
	hacker in seconds. If more than one hacker spent the same amount of time, choose the
	one with the smallest ID.*/
	cout << "-2\n";
	/*12. ID of the hacker who spent the least time in the queues and the waiting time of that
	hacker in seconds, among the ones who grabbed three stickers and hoodies. If more than
	one hacker spent the same amount of time, choose the one with the smallest ID. If there
	is no hacker that grabbed three stickers and hoodies, print -1 for both.*/
	cout << "-2\n";
	/*13. Total seconds passed during the hackathon.*/
	cout << last_event_time <<"\n";
}

/*
    read inputFile 
    fills follwings:
    hckr
    event_heap
    sticker_Q_service_rates
    hoodie_Q_service_rates
 */

void inputParse(
		std::vector<Hacker> &hckr,
		std::priority_queue<EventQueue, vector<EventQueue>, heap_algo> &event_heap,
		char **argv)
{
	std::ifstream file(argv[1]);

	if (file.is_open())
	{
		file >> number_of_hackers;

		for (size_t i = 0; i < number_of_hackers; i++)
		{
			uint16_t arrival_time;
			file >> arrival_time;

			hckr.push_back(Hacker(arrival_time));
			hckr[i].setHackerID(i+1);
			event_heap.push(EventQueue(hckr[i].getHackerID(), A_HACKER_ARRIVES, arrival_time));
		}

		file >> number_of_commits;

		for (size_t i = 0; i < number_of_commits; i++)
		{
			uint16_t commiter_id;
			uint16_t commit_len;
			float commit_time;

			file >> commiter_id;
			file >> commit_len;
			file >> commit_time;

			event_heap.push(EventQueue(hckr[commiter_id-1].getHackerID(),
					A_HACKER_COMMITS,
					commit_time,
					commit_len));
		}

		file >> number_of_attempts;

		for (size_t i = 0; i < number_of_attempts; i++)
		{
			uint16_t hacker_id;
			float attemp_time;

			file >> hacker_id;
			file >> attemp_time;

			event_heap.push(EventQueue(hckr[hacker_id-1].getHackerID(),
					A_HACKER_ENTERS_STICKER_QUEUE,
					attemp_time));
		}

		file >> number_of_sticker_Q;

		for (size_t i = 0; i < number_of_sticker_Q; i++)
		{
			float service_rate;
			file >> service_rate;
			sticker_desks.push_back(Desk(i+1, service_rate));
		}

		file >> number_of_hoodie_Q;

		for (size_t i = 0; i < number_of_sticker_Q; i++)
		{
			float service_rate;
			file >> service_rate;
			hoodie_desks.push_back(Desk(i+1, service_rate));
		}
	}
	else
	{
		std::cout << "Error on reading in put file " << std::endl;
		exit(-1);
	}
}

void print(std::vector<Hacker> const &list)
{
	for (Hacker i : list)
	{

		int x = i.getArrivalTime();

		std::cout << x << "\n";
	}
}

