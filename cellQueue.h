#include <iostream>
using namespace std;

template <typename T>
class node {
public:
	T dataR;
	T dataC;
	node* next;
	node() : dataR(0), dataC(0), next(NULL) {}
};

template <typename T>
class cellQueue {
	node<T>* front;
	node<T>* rear;
public:
	cellQueue() : front(NULL), rear(NULL) {}

	bool isEmpty() {
		if (rear) return false;
		return true;
	}

	bool Front(T& r, T& c) {
		if (!isEmpty()) {
			r = front->dataR;
			c = front->dataC;
			return true;
		}
		return false;
	}

	void enqueue(T const r, T const c) {
		node<T>* n = new node<T>;
		n->dataR = r;
		n->dataC = c;
		n->next = NULL;

		if (isEmpty()) {
			rear = n;
			front = n;
			rear->next = NULL;
			front->next = NULL;
		}
		else {
			rear->next = n;
			rear = n;
		}
	}

	void dequeue() {
		node<T>* temp = front;
		if (front) {
			front = front->next;

			if (!front) rear = NULL;

			delete temp;
		}
	}

	void Print() const {
		node<T>* curr = front;
		while (curr != NULL) {
			cout << curr->dataR << " " << curr->dataC << "\t";
			curr = curr->next;
		}
		cout << endl;
	}

	~cellQueue() {
		if (front) {
			node<T>* curr = front;
			while (front) {
				front = front->next;
				delete curr;
				curr = front;
			}
			front = NULL;
			rear = NULL;
		}
	}
};