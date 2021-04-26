//Program 4
//Developed by Shane Flynn
#include <iostream>
#include <random>
#include <thread>
#include <memory>
#include <queue>
#include <tuple>
const int REF_STR_SIZE = 20;
int inputPageFrames();
std::unique_ptr<int[]> makeRefString();
bool isAlreadyInFrame(std::unique_ptr<int[]>&, const int&, const int&);
int findFurthestPageIndex(const std::unique_ptr<int[]>&, const std::unique_ptr<int[]>&, const int&, const int&);
int FIFO(const std::unique_ptr<int[]>&, const int&);
int LRU(const std::unique_ptr<int[]>&, const int&);
int OPT(const std::unique_ptr<int[]>&, const int&);

int main() {

	int pageFrames = inputPageFrames();
	std::unique_ptr<int[]> refString = makeRefString();
	
	std::cout << "FIFO page faults: " << FIFO(refString, pageFrames) << std::endl;
	std::cout << "LRU page faults: " << LRU(refString, pageFrames) << std::endl;
	std::cout << "OPT page faults: " << OPT(refString, pageFrames);
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
	std::queue<std::tuple<int, int>> leastRefQueue;

	for (int i = 0; i < pageFrames; i++) {
		frames[i] = refString[i];
		leastRefQueue.emplace(refString[i], i);
	}

	for (int i = pageFrames; i < REF_STR_SIZE; i++) {
		if (isAlreadyInFrame(frames, refString[i], pageFrames)) {
			leastRefQueue.push(leastRefQueue.front());
			leastRefQueue.pop();
		}
		else 	{
			int index = std::get<1>(leastRefQueue.front());
			frames[index] = refString[i];
			leastRefQueue.emplace(refString[i], index);
			leastRefQueue.pop();
			pageFaults++;
		}
	}
	return pageFaults;
}
int OPT(const std::unique_ptr<int[]>& refString, const int& pageFrames) {
	int pageFaults = 0;
	std::unique_ptr<int[]> frames = std::make_unique<int[]>(pageFrames);

	for (int i = 0; i < pageFrames; i++)
		frames[i] = refString[i];

	for (int i = pageFrames; i < REF_STR_SIZE; i++) 		{

		if (!isAlreadyInFrame(frames, refString[i], pageFrames)) 	{
			frames[findFurthestPageIndex(refString, frames, i, pageFrames)] = refString[i];
			pageFaults++;
		}

	}
	return pageFaults;
}

int findFurthestPageIndex(const std::unique_ptr<int[]>& refString, const std::unique_ptr<int[]>& frames, const int& currPos, const int& pageFrames) {
	
	int firstRefArray[10] = {}; //hold when a page is first referenced
	int numFoundFrames = 0;

	for (int i = currPos; i < REF_STR_SIZE; i++) 		{
		if (numFoundFrames == pageFrames - 1)
			break;
		
		if (firstRefArray[refString[i]] == 0) {
			firstRefArray[refString[i]] = i;
			numFoundFrames++;
		}
	}

	int maxPageIndex = -1;
	int pageFrameIndex = -1;

	for (int i = 0; i < pageFrames; i++) {
		if (firstRefArray[frames[i]] == 0) 	{ //if frame # not discovered
			return i;
		}
		if (firstRefArray[frames[i]] > maxPageIndex) 	{
			maxPageIndex = firstRefArray[i];
			pageFrameIndex = i;
		}

	}

	return pageFrameIndex;
}
bool isAlreadyInFrame(std::unique_ptr<int[]>& frames, const int& pageNumb, const int& pageFrames) {
	for (int i = 0; i < pageFrames; i++) 
		if (frames[i] == pageNumb)
			return true;

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