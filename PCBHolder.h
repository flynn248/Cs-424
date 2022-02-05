//Class to hold the two versions of PCB

#include "Version1PCB.h"
#include "Version2PCB.h"
#include <iostream>
#ifndef PCBHOLDER_H
#define PCBHOLDER_H

class PCBHolder{
private:
	const int SIZE = 10;

public:
	Version1PCB** v1 = new Version1PCB * [SIZE];
	Version2PCB** v2 = new Version2PCB * [SIZE];
	
	PCBHolder() {
		v1[0] = new Version1PCB(0);
		v2[0] = new Version2PCB(0);
		for (int i = 1; i < SIZE; i++) {
			v1[i] = NULL;
			v2[i] = NULL;
		}
	}

	~PCBHolder() {

		for (int i = 0; i < SIZE; i++) {
			delete v1[i];
			delete v2[i];
		}

		delete[] v1;
		delete[] v2;
	}

	template<typename T>
	int firstAvailableIndex(T** PCBarray, const int SIZE) //find first available index
	{
		for (int i = 0; i < SIZE; i++)
			if (!PCBarray[i])
				return i;
	}

	void createV1(const int parentIndex) //create child at parent index
	{
		int childIndex = firstAvailableIndex(v1, SIZE);
		v1[parentIndex]->getChildren().insertAtEnd(childIndex);
		v1[childIndex] = new Version1PCB(parentIndex);
	}

	void destroyV1(const int index) {
		destroyV1(index, index);
	}

	void destroyV1(const int index, const int almightyParent) {
		do //finds and deallocates all of the children/ decendents from memory of given parent node
		{
			if (v1[index]->getChildren().size == 0) {
				if (index != almightyParent) {
					v1[v1[index]->getParentIndex()]->getChildren().deleteHead(); //delete node off of parent's children linked list 
					delete v1[index];
					v1[index] = NULL;
					break;
				}
			}
			else
				destroyV1(v1[index]->getFirstChild()->value, almightyParent);

		} while (v1[index]->getChildren().size > 0);
	}

	void createV2(const int parentIndex) //create a new child of parent PCB
	{
		int childIndex = firstAvailableIndex(v2, SIZE);
		v2[childIndex] = new Version2PCB(parentIndex);
		int firstKid = v2[parentIndex]->getFirstChild();

		if (firstKid == -1)
			v2[parentIndex]->setFirstChild(childIndex);
		else
			setYoungestSib(firstKid, childIndex);
	}

	void setYoungestSib(const int olderChild, const int youngestChild) {
		if (v2[olderChild]->getYoungSib() != -1) //Find youngest sibling in bunch
		{
			setYoungestSib(v2[olderChild]->getYoungSib(), youngestChild);
		}
		else if (v2[olderChild]->getOlderSib() != -1) //If older child is not first child
		{
			v2[olderChild]->setYoungSib(youngestChild);
			v2[youngestChild]->setOlderSib(olderChild);
		}
		else if (v2[olderChild]->getOlderSib() == -1) //if older child is First child
		{
			v2[olderChild]->setYoungSib(youngestChild);
		}
	}

	void destroyV2(const int index) {
		destroyV2(index, index);
	}

	void destroyV2(const int index, const int almightyParent) //destroys all children/ decendents recursively
	{
		do {
			if (v2[index]->getFirstChild() != -1) { //Checks for children
				destroyV2(v2[index]->getFirstChild(), almightyParent);
				v2[index]->setFirstChild(-1);
			}
			else if (v2[index]->getYoungSib() != -1) { //Check for younger siblings
				destroyV2(v2[index]->getYoungSib(), almightyParent);
				v2[index]->setYoungSib(-1);
			}
			else if (index != almightyParent) { //If not parent that origionally called function
				delete v2[index];
				v2[index] = NULL;
				break;
			}
			else
				break;
		} while (true);
	}
};
#endif