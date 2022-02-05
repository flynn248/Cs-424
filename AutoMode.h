#ifndef AUTOMODE_H
#define AUTOMODE_H
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include <mutex>
#include <ctime>
#include "Banker.h"

std::mutex banker_algo_mutex;
std::mutex display_mutex;
bool binSemaphoreResourceRequest = false;
int semaphoreResourceRelease = 0;

namespace pt {

	int getRandomNumber(const int r) { //get random number between 0 < ri <= r
		srand(time(0));
		thread_local std::mt19937 rng(rand() % r); //seed
		std::uniform_int_distribution<int> generator(0, r); // min, max
		return generator(rng);
	}

	void aProcessThread(const int proID, std::shared_ptr<Banker> banker) {
		thread_local int processID = proID;
		thread_local int resourceID;
		thread_local int resourceQty;

		for (int i = 0; i < 3; i++) 		{
			//request resource
			resourceID = getRandomNumber(banker->getNumResources() - 1);
			resourceQty = getRandomNumber(banker->getProcessMaxClaims(processID, resourceID));
			{
				std::lock_guard<std::mutex> lock(banker_algo_mutex);
				binSemaphoreResourceRequest = true;
				while(semaphoreResourceRelease > 0){ //if other threads are releasing resources
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				bool safeRequest = banker->makeResourceRequest(processID, resourceID, resourceQty);
				{ //prevent the console from having intermingled outputs
					std::lock_guard<std::mutex> lock(display_mutex);

					std::cout << "Process " << processID << " requests " << resourceQty << " of resource " << resourceID << ": ";
					if (safeRequest == false)
						std::cout << "Denied\n";
					else
						std::cout << "Accepted\n";
				}

				binSemaphoreResourceRequest = false;
			}

			//release resource
			resourceID = getRandomNumber(banker->getNumResources() - 1);
			while (binSemaphoreResourceRequest){ //if a thread is requesting a resource
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			semaphoreResourceRelease ++;
			resourceQty = getRandomNumber(banker->getProcessAllocatedResources(processID, resourceID));
			banker->makeResourceRelease(processID, resourceID, resourceQty);

			{//prevent the console from having intermingled outputs
				std::lock_guard<std::mutex> lock(display_mutex);
				std::cout << "Process " << processID << " releases " << resourceQty << " of resource " << resourceID << std::endl;
			}
			semaphoreResourceRelease--;
		}
	}
}

class AutoMode 				{
public:
	AutoMode(char *[]);
	void startAutomationOfProcesses();

private:
	std::shared_ptr<Banker> banker;	
	void processThreadHandler();
};

AutoMode::AutoMode(char * argv[]) 				{
	banker = std::make_shared<Banker>(argv);
}

void AutoMode::startAutomationOfProcesses() { processThreadHandler(); }
void AutoMode::processThreadHandler() {
	std::vector<std::thread> processThreads;
	for (int processID = 0; processID < banker->getNumProcesses(); processID++) 		{ //start process in own threads
		std::thread th(&pt::aProcessThread, std::move(processID), banker);
		processThreads.push_back(std::move(th));
	}

	for (auto& th : processThreads)
		th.join();
}

#endif // !AUTOMODE_H