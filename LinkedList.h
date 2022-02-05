#include "ListNode.h"

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

class LinkedList {
public:
	ListNode* head, * tail;
	int size;

	LinkedList() {
		head = tail = NULL;
		size = 0;
	}

	void insertAtEnd(int val) { //insert new node at end
		ListNode* newNode = new ListNode(val);
		if (size == 0)
			head = tail = newNode;
		else {
			tail->next = newNode;
			tail = newNode;
		}
		size++;
	}

	void deleteHead() { //delete the head of the linked list
		if (size == 0) {
			std::cout << "Cannot delete an empty List!" << std::endl;
		}
		else if (size == 1) {
			delete head;
			head = tail = NULL;
			size--;
		}
		else {
			ListNode* temp = head;
			head = head->next;
			delete temp;
			size--;
		}
	}
};
#endif // !LINKEDLIST_H