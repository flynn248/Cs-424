//Version 1 of the PCB
//Uses linked lists to keep track of children PCB

#ifndef VERSION1PCB_H
#define VERSION1PCB_H
#include "LinkedList.h"

class Version1PCB
{
private:
	int parent = NULL;
	LinkedList children;

public:

	Version1PCB(){}

	Version1PCB(const int p) {
		parent = p;
	}

	ListNode *getFirstChild()const {
		return children.head;
	}

	int getParentIndex()const {
		return parent;
	}

	LinkedList& getChildren() {
		return children;
	}
};
#endif