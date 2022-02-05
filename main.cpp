//Program Assignment 1
//Compare two methods of PCB allocation
//Developed by Shane Flynn
#include <iostream>
#include <iomanip>
#include <time.h>
#include "PCBHolder.h"

#define LOOPS 200000 //Number of test iterations for both v1 and v2

clock_t version1Performance(PCBHolder& PCB);
clock_t version2Performance(PCBHolder& PCB);

int main()
{
	PCBHolder PCB;

	std::cout << "Number of iterations: " << LOOPS << std::setprecision(3) << std::fixed << std::endl;

	clock_t v1Time = version1Performance(PCB);
	std::cout << "Version 1 took " << v1Time << " clicks [" << ((float)v1Time / CLOCKS_PER_SEC) << " seconds]\n";

	clock_t v2Time = version2Performance(PCB);
	std::cout << "Version 2 took " << v2Time << " clicks [" << ((float)v2Time / CLOCKS_PER_SEC) << " seconds]\n";

	std::cout << "Difference of V1-V2 is: " << v1Time - v2Time << " clicks [" << ((float)(v1Time - v2Time) / CLOCKS_PER_SEC) << " seconds]\n";

	return 0;
}

clock_t version1Performance(PCBHolder& PCB)//Test time using same steps as v2
{
	int index = -1;
	clock_t t;
	t = clock();

	for (int i = 0; i < LOOPS; i++)
	{
		PCB.createV1(0); //1
		PCB.createV1(0); //2
		PCB.createV1(0); //3
		PCB.createV1(0); //4
		PCB.createV1(0); //5
		PCB.createV1(0); //6
		PCB.createV1(1); //7
		PCB.createV1(2); //8
		PCB.createV1(1); //9

		PCB.destroyV1(0);
	}

	t = clock() - t;
	return t;
}
															
clock_t version2Performance(PCBHolder& PCB)//Test time using same steps as v1
{
	int index = -1;
	clock_t t;
	t = clock();

	for (int i = 0; i < LOOPS; i++) {
		PCB.createV2(0); //1
		PCB.createV2(0); //2
		PCB.createV2(0); //3
		PCB.createV2(0); //4
		PCB.createV2(0); //5
		PCB.createV2(0); //6
		PCB.createV2(1); //7
		PCB.createV2(2); //8
		PCB.createV2(1); //9

		PCB.destroyV2(0);
	}

	t = clock() - t;
	return t;
}