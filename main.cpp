//Program 4
//Developed by Shane Flynn
#include <iostream>
#include <random>
#include <thread>
//#include <memory>
#include <string>
#include <chrono>

const int REF_STR_SIZE = 20;
int inputPageFrames();
std::unique_ptr<int[]> makeRefString();
bool isAlreadyInFrame(std::unique_ptr<int[]>&, const int&, const int&);
int FIFO(const std::unique_ptr<int[]>&, const int&);
int LRU(const std::unique_ptr<int[]>&, const int&);
int OPT(const std::unique_ptr<int[]>&, const int&);

int main() {

	int pageFrames = inputPageFrames();
	std::unique_ptr<int[]> refString = makeRefString();
	
	std::cout << "FIFO page faults: " << FIFO(refString, pageFrames);
	std::cin.get();
	return 0;
}

int FIFO(const std::unique_ptr<int[]>& refString, const int& pageFrames){
	int pageFaults = 0;
	int framePointer = 0;
	std::unique_ptr<int[]> frames = std::make_unique<int[]>(pageFrames);

	for (int i = 0; i < pageFrames; i++)
		frames[i] = refString[i];

	for (int i = pageFrames; i < REF_STR_SIZE; i++) 		{
		if (!isAlreadyInFrame(frames, refString[i], pageFrames)) 	{
			frames[framePointer] = refString[i];
			framePointer = ++framePointer % pageFrames;
			pageFaults++;
		}
	}

	return pageFaults;
}
int LRU(const std::unique_ptr<int[]>& refString, const int& pageFrames) {
	int pageFaults = 0;
	std::unique_ptr<int[]> frames = std::make_unique<int[]>(pageFrames);

	for (int i = 0; i < pageFrames; i++)
		frames[i] = refString[i];

	return pageFaults;
}
int OPT(const std::unique_ptr<int[]>& refString, const int& pageFrames) {
	int pageFaults = 0;
	std::unique_ptr<int[]> frames = std::make_unique<int[]>(pageFrames);

	for (int i = 0; i < pageFrames; i++)
		frames[i] = refString[i];

	return pageFaults;
}

bool isAlreadyInFrame(std::unique_ptr<int[]>& frames, const int& pageNumb, const int& pageFrames) {
	for (int i = 0; i < pageFrames; i++) 		{
		if (frames[i] == pageNumb) 	{
			return true;
		}
	}
	return false;
}

int inputPageFrames() {
	while (true) 	{
		try 	{
			int tmp = 0;
			std::cout << "Enter the number of page frames in the system: ";
			std::cin >> tmp;
			if (tmp < 1 || tmp > 6) 	{
				throw std::exception("\nInvalid Page Frame #\n");
			}
			std::cin.ignore();
			std::cout << std::endl;
			return tmp;
		}
		catch (const std::exception& e) 	{
			std::cout << e.what()
				<< "The possible page frame # must be between 1 and 6\n";
			if (std::cin.fail()) 	{
				std::cin.clear();
				std::cin.ignore();
			}
		}

	}

}

std::unique_ptr<int[]> makeRefString() {
	std::unique_ptr<int[]> tmp = std::make_unique<int[]>(REF_STR_SIZE);
	std::cout << "Reference String: ";
	for (int i = 0; i < REF_STR_SIZE; i++) 		{
		srand(time(NULL));
		std::mt19937_64 rng(rand() + (uint64_t)i);
		std::uniform_int_distribution<int> generator(0, 9);
		tmp[i] = generator(rng);
		std::cout << tmp[i];
	}
	std::cout << std::endl;
	return tmp;
}