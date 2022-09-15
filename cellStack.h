#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Node {
public:
	T dataR;
	T dataC;
	Node* next;
	Node() : dataR(0), dataC(0), next(NULL) {}
};

template <typename T>
class cellStack {
	Node<T>* top;
public:
	cellStack() : top(NULL) {}

	bool isEmpty() {
		if (top) return false;
		return true;
	}

	bool isFull() {
		return false;
	}

	bool topValue(T& r, T& c) {
		if (!isEmpty()) {
			r = top->dataR;
			c = top->dataC;
			return true;
		}
		return false;
	}

	void push(T const& r, T const& c) {
		Node<T>* curr = new Node<T>;
		curr->dataR = r;
		curr->dataC = c;
		curr->next = top;
		top = curr;
	}

	void pop() {
		if (!isEmpty()) {
			Node<T>* curr = top;
			top = top->next;
			delete curr;
		}
	}

	~cellStack() {
		if (top) {
			Node<T>* curr = top;
			while (top) {
				top = top->next;
				delete curr;
				curr = top;
			}
		}
	}
};