#ifndef LISTNODE_H
#define LISTNODE_H
#include <iostream>

class ListNode
{
public:
	ListNode* next;
	int value;

	ListNode(int val) {
		value = val;
		next = NULL;
	}
};

#endif // !LISTNODE_H