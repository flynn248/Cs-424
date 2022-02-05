//Program 2
//Compsci 424
//Developed by Shane Flynn

/*RACE CONDITION RESULTS
Specific values in combination resulted in race conditions within 90 seconds:
	1. n = 15, k = 5, t = 1
	2. n = 30, k = 10, t = 2
	3. n = 30, k = 5, t = 2
	4. n = 50, k = 5, t = 5
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <ctime>

int fullSlots = 0;
int emptySlots;
std::mutex bufferAccess;

void mainThread();
void producer(const int, const int, const int, int*, bool*);
void consumer(const int, const int, const int, int*, bool*);
int getRandomNumber(const int);

int main() {
	std::thread mainT(&mainThread);
	mainT.join();
	return 0;
}

void mainThread() {
	const int n = 100; //size of buffer
	const int k = 75; //max burst length
	const int t = 5; //max number of seconds each thread will sleep between bursts
	int buffer[n] = {};
	bool *continueProcessing = new bool(true); //let threads know to continue or stop
	emptySlots = n;

	std::thread prod(&producer, std::ref(n), std::ref(k), std::ref(t), buffer, continueProcessing);
	std::thread cons(&consumer, std::ref(n), std::ref(k), std::ref(t), buffer, continueProcessing);

	std::cout << "Main thread sleeping for 90 seconds..." << std::endl;
	std::cout << "Displaying the buffer as the consumer finishes its burst:" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(90)); //Wait 90 seconds
	*continueProcessing = false;
	std::cout << "Sending Termination Signal" << std::endl;
	std::cout << "Producer and Consumer are finishing last tasks before terminating." << std::endl;
	prod.join();
	cons.join();
	delete continueProcessing;
	continueProcessing = nullptr;
}

void producer(const int n, const int k, const int t, int *buffer, bool *continueProcessing) {
	int next_in = 0;

	while (*continueProcessing) {

		int k1 = getRandomNumber(k);

		for (int i = 0; i < k1; i++) {
			
			emptySlots--;
			while (emptySlots <= 0 && *continueProcessing) 	{ //P(empty)
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}

			bufferAccess.lock(); //P(mutex)
			buffer[(next_in + i) % n] += 1;
			bufferAccess.unlock(); //V(mutex)
			fullSlots++; //V(full)
		}

		next_in = (next_in + k1) % n;

		int t1 = getRandomNumber(t);

		std::this_thread::sleep_for(std::chrono::seconds(t1));
	}
}

void consumer(const int n, const int k, const int t, int* buffer, bool *continueProcessing) {
	int next_out = 0;

	while (*continueProcessing) 	{
		int t2 = getRandomNumber(t);
		std::this_thread::sleep_for(std::chrono::seconds(t2));
		int k2 = getRandomNumber(k);

		for (int i = 0; i < k2; i++) {
			fullSlots--;
			while (fullSlots <= 0 && *continueProcessing) 	{ //P(full)
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}

			bufferAccess.lock(); //P(mutex)
			int data = buffer[(next_out + i) % n];

			if (data > 1) {
				std::cout << "Race Condition!!" << std::endl;
				bufferAccess.unlock();
				return;
			}

			buffer[(next_out + i) % n] = 0;
			bufferAccess.unlock(); //V(mutex)
			emptySlots++; //V(empty)
		}
		for (int i = 0; i < n; i++) 		{ //display purposes
			std::cout << buffer[i] << "";
		}
		std::cout << std::endl;

		next_out = (next_out + k2) % n;
	}
}

int getRandomNumber(const int r) { //get random number between 0 < ri <= r
	srand(time(0)); //seed to get a random number
	return (rand() % r) + 1;
}