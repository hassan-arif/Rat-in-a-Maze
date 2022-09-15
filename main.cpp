#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#include "cellStack.h"		//stack based sol
#include "cellQueue.h"		//queue based sol

using namespace std;

void SetColorAndBackground(int ForgC, int BackC);	//for onspot coloring
//default->(15,0)		source/path->(14,0)		freespace->(6,0)		destination->(10,0)		obstacle->(1,0)		newobstacle->(4,0)
//bright white, black	light yellow, black		yellow, black			light green, black		blue, black			red, black


void dimensionEntry(int&, int&);
char** set1(int, int);			//designing char datatype maze from scatch
int** set2(char**, int, int);	//converting char maze to int maze for queue based implementation

void print(char**, int, int);		//for backtracking
void print(int**, int, int, int, int);		//for lee's algorithm
int** findCoordinatesOfShortestPath(int** array, int r, int c, int rS, int cS, int k);

template <typename t>
t** deepcopy(t**, int, int);	//deep-copy of 2d dynamic array

template <typename t>
void del(t**, int);				//deletion of 2d dynamic array to free memory in heap

void sourceEntry(int&, int&, char**, int, int);
void destinationEntry(int&, int&, int, int, char**, int, int);

bool backtracking(char**, int, int, int, int, int, int);
void leesalgo(int**, int, int, int, int, int, int);
int countStack(char**, int, int);
int countQueue(int**, int, int);

//------------------------------------------------MAIN FUNCTION------------------------------------------------

int main() {
	srand(time(0));

	cout << "\t\t\t\t\tProblem 1: Rat in a Maze\n\n";

	cout << "please enter the dimensions of the Maze...\n\n";
	int r = 0, c = 0;
	dimensionEntry(r, c);
	cout << endl;

	char** maze1 = set1(r, c);			//for stackbased sol: backtracking
	print(maze1, r, c);					//obstacle: x		visited-obstacle: v		free-space: z		path: o		goal: g
	cout << endl;

	int** maze2 = set2(maze1, r, c);	//for queuebased sol: lee's algo
										//obstacle: -2, printed x		free space:	-1, printed z
										//goal: 0, printed g			source: -3

	int rS = 0, cS = 0, rD = 0, cD = 0;
	char** maze1Backup = deepcopy(maze1, r, c);

	while (true) {
		cout << "please enter the coordinates of Source cell...\n\n";
		sourceEntry(rS, cS, maze1, r, c);
		cout << endl;

		cout << "please enter the coordinates of Destination cell...\n\n";
		destinationEntry(rD, cD, rS, cS, maze1, r, c);
		cout << endl;

		if (backtracking(maze1, r, c, rS, cS, rD, cD)) {	//backtracking on maze1
			maze2[rS][cS] = -3;
			maze2[rD][cD] = 0;
			leesalgo(maze2, r, c, rS, cS, rD, cD);			//lee's algorithm on maze2
			break;
		}

		cout << "sadly, there's no direct way between source and destination cell.\n";
		cout << "we can resolve such conflict only by changing the source and destination cells.\n\n";

		del(maze1, r);
		maze1 = deepcopy(maze1Backup, r, c);
		rS = 0; cS = 0; rD = 0; cD = 0;
	}

	cout << "\"Stack Based Solution: Backtracking\"\n\n";
	print(maze1, r, c);
	cout << endl;
	Sleep(1000);

	cout << "\"Queue Based Solution: Lee's Algorithm\"\n\n";
	print(maze2, r, c, rS, cS);
	cout << endl;
	Sleep(1000);

	cout << "\"Comparison of Both Solutions (Efficiency)\"\n\n";
	int n1 = countStack(maze1, r, c), n2 = countQueue(maze2, r, c);
	cout << "Stack-Based Sol:\t" << n1 << " Steps";
	cout << "\nQueue-Based Sol:\t" << n2 << " Steps";
	if (n1 > n2) cout << "\nQueue-Based Solution is more efficient in terms of Time and Space Complexity.\n\n";
	else if (n1 < n2) cout << "\nStack-Based Solution is more efficient in terms of Time and Space Complexity.\n\n";
	else cout << "\nBoth Solutions are equally efficient in terms of Time and Space Complexity.\n\n";

	del(maze1, r);
	del(maze1Backup, r);
	del(maze2, r);

	system("pause");
	return 0;
}

void SetColorAndBackground(int ForgC, int BackC) {
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}

void dimensionEntry(int& r, int& c) {
	cout << "Rows    (>1):\t";
	cin >> r;
	while (r <= 1) {
		cout << "invalid row(s)! please enter again: ";
		cin >> r;
	}
	cout << "Columns (>1):\t";
	cin >> c;
	while (c <= 1) {
		cout << "invalid column(s)! please enter again: ";
		cin >> c;
	}
}

char** set1(int r, int c) {
	char** m = new char* [r];
	for (int i = 0; i < r; i++) {
		m[i] = new char[c];
		for (int j = 0; j < c; j++) m[i][j] = 'z';
	}
	int total = r * c, tempR = 0, tempC = 0;
	total /= 4;
	while (total > 0) {
		tempR = rand() % r;
		tempC = rand() % c;
		while (m[tempR][tempC] == 'x') {
			tempR = rand() % r;
			tempC = rand() % c;
		}
		m[tempR][tempC] = 'x';
		total--;
	}

	return m;
}

int** set2(char** m, int r, int c) {
	int** n = new int* [r];
	for (int i = 0; i < r; i++) {
		n[i] = new int[c];
		for (int j = 0; j < c; j++) {
			if (m[i][j] == 'z') n[i][j] = -1;			//free-space
			else if (m[i][j] == 'x') n[i][j] = -2;		//obstacle
		}
	}
	return n;
}

void print(char** array, int r, int c) {			//print function for backtracking
	cout << '\t';

	SetColorAndBackground(8, 0);
	for (int i = 0; i < c; i++) {
		cout << i;
		if (i > 9) cout << "   ";
		else cout << "    ";
	}
	SetColorAndBackground(15, 0);

	cout << "\n\n";
	for (int i = 0; i < r; i++) {
		SetColorAndBackground(8, 0);	//gray color for border
		cout << " " << i << "\t";
		SetColorAndBackground(15, 0);

		for (int j = 0; j < c; j++) {
			//default->(15,0)		source/path->(14,0)		freespace->(6,0)		destination->(10,0)		obstacle->(1,0)		newobstacle->(4,0)
			//bright white, black	light yellow, black		yellow, black			light green, black		blue, black			red, black

			if (array[i][j] == 'x') SetColorAndBackground(1, 1);			//obstacle
			else if (array[i][j] == 'z') SetColorAndBackground(14, 14);		//free-space
			else if (array[i][j] == 'g') SetColorAndBackground(10, 10);		//destination
			else if (array[i][j] == 'v') SetColorAndBackground(4, 4);		//visited path, now obstacle
			else if (array[i][j] == 'o') SetColorAndBackground(6, 6);		//path to destination

			cout << array[i][j] << " ";

			SetColorAndBackground(15, 0);
			cout << "   ";	//with double-space
		}
		SetColorAndBackground(8, 0);
		cout << "    " << i;
		cout << endl << endl;
	}
	cout << '\t';
	for (int i = 0; i < c; i++) {
		cout << i;
		if (i > 9) cout << "   ";
		else cout << "    ";
	}
	cout << endl;

	SetColorAndBackground(15, 0);
}

void print(int** array, int r, int c, int rS, int cS) {			//print function for lee's algorithm
																//obstacle: -2, printed x		free space:	-1, printed z
																//goal: 0, printed g			source: -3

	//default->(15,0)		source/path->(14,0)		freespace->(6,0)		destination->(10,0)		obstacle->(1,0)		newobstacle->(4,0)
	//bright white, black	light yellow, black		yellow, black			light green, black		blue, black			red, black

	int k = countQueue(array, r, c), l = 0;
	int** pathway = findCoordinatesOfShortestPath(array, r, c, rS, cS, k);

	cout << '\t';
	SetColorAndBackground(8, 0);		//gray color for border
	for (int i = 0; i < c; i++) {
		cout << i;
		if (i > 9) cout << "   ";
		else cout << "    ";
	}
	SetColorAndBackground(15, 0);

	cout << "\n\n";
	for (int i = 0; i < r; i++) {
		SetColorAndBackground(8, 0);
		cout << " " << i << "\t";
		SetColorAndBackground(15, 0);

		for (int j = 0; j < c; j++) {
			if (array[i][j] == -2) {
				SetColorAndBackground(1, 1);		//obstacle
				cout << "x ";
			}
			else if (array[i][j] == 0) {
				SetColorAndBackground(10, 10);		//destination
				cout << "g ";
			}
			else if (array[i][j] == -3) {
				SetColorAndBackground(0, 10);		//source
				cout << array[i][j] << " ";
			}
			else if (array[i][j] == -1) {
				SetColorAndBackground(14, 14);		//free-space				
				cout << "z ";
			}
			else if (array[i][j] > 0) {
				if (l < k && pathway[l][0] == i && pathway[l][1] == j) {
					SetColorAndBackground(0, 6);
					l++;
				}
				else SetColorAndBackground(0, 14);		//travelled space
				cout << array[i][j];
				if (array[i][j] < 10) cout << " ";	//need to set condition where one path is colored the same as source
			}

			SetColorAndBackground(15, 0);
			cout << "   ";
		}
		SetColorAndBackground(8, 0);
		cout << "    " << i;
		cout << endl << endl;
	}
	cout << '\t';
	for (int i = 0; i < c; i++) {
		cout << i;
		if (i > 9) cout << "   ";
		else cout << "    ";
	}
	cout << endl;

	SetColorAndBackground(15, 0);

	del(pathway, k);
}

int** findCoordinatesOfShortestPath(int** array, int r, int c, int rS, int cS, int k) {
	int** path = new int* [k];
	for (int i = 0; i < k; i++) path[i] = new int[2];
	path[0][0] = rS;
	path[0][1] = cS;

	int currR = rS, currC = cS, value = array[rS][cS];

	int d = 1;
	while (d < k) {
		if (currC + 1 < c && (array[currR][currC + 1] + 1) == value) {
			currC++;
			path[d][0] = currR;
			path[d][1] = currC;
			value = array[currR][currC];
			d++;
		}
		else if (currR + 1 < r && (array[currR + 1][currC] + 1) == value) {
			currR++;
			path[d][0] = currR;
			path[d][1] = currC;
			value = array[currR][currC];
			d++;
		}
		else if (currC - 1 >= 0 && (array[currR][currC - 1] + 1) == value) {
			currC--;
			path[d][0] = currR;
			path[d][1] = currC;
			value = array[currR][currC];
			d++;
		}
		else if (currR - 1 >= 0 && (array[currR - 1][currC] + 1) == value) {
			currR--;
			path[d][0] = currR;
			path[d][1] = currC;
			value = array[currR][currC];
			d++;
		}
	}

	//sorting
	int temporary = 0;
	for (int i = 0; i < k - 1; i++) {
		for (int j = i + 1; j < k; j++) {
			if (path[i][0] > path[j][0]) {
				temporary = path[i][0];
				path[i][0] = path[j][0];
				path[j][0] = temporary;

				temporary = path[i][1];
				path[i][1] = path[j][1];
				path[j][1] = temporary;
			}
			if (path[i][0] == path[j][0]) {
				if (path[i][1] > path[j][1]) {
					temporary = path[i][1];
					path[i][1] = path[j][1];
					path[j][1] = temporary;
				}
			}
		}
	}

	return path;
}

template <typename t>
t** deepcopy(t** m, int r, int c) {
	t** n = new t * [r];
	for (int i = 0; i < r; i++) {
		n[i] = new t[c];
		for (int j = 0; j < c; j++) n[i][j] = m[i][j];
	}
	return n;
}

template <typename t>
void del(t** m, int r) {
	for (int i = 0; i < r; i++) delete[] m[i];
	delete[]m;
	m = nullptr;
}

void sourceEntry(int& rS, int& cS, char** maze1, int r, int c) {
	while (true) {
		cout << "Row    (range: [0-" << r - 1 << "]):\t";
		cin >> rS;
		while (rS < 0 || rS >= r) {
			cout << "invalid row! please enter again: ";
			cin >> rS;
		}

		cout << "Column (range: [0-" << c - 1 << "]):\t";
		cin >> cS;
		while (cS < 0 || cS >= c) {
			cout << "invalid column! please enter again: ";
			cin >> cS;
		}

		if (maze1[rS][cS] == 'x') {
			cout << "source cell can't be an obstacle! please enter Valid coordinates of Source cell...\n";
			continue;
		}
		break;
	}
	maze1[rS][cS] = 'o';
}

void destinationEntry(int& rD, int& cD, int rS, int cS, char** maze1, int r, int c) {
	while (true) {
		cout << "Row    (range: [0-" << r - 1 << "]):\t";
		cin >> rD;
		while (rD < 0 || rD >= r) {
			cout << "invalid row! please enter again: ";
			cin >> rD;
		}

		cout << "Column (range: [0-" << c - 1 << "]):\t";
		cin >> cD;
		while (cD < 0 || cD >= c) {
			cout << "invalid column! please enter again: ";
			cin >> cD;
		}

		if (maze1[rD][cD] == 'x') {
			cout << "destination cell can't be an obstacle! please enter Valid coordinates of Destination cell...\n";
			continue;
		}
		else if (rS == rD && cS == cD) {
			cout << "destination cell can't be the same as the source cell! please enter different coordinates...\n";
			continue;
		}
		break;
	}
	maze1[rD][cD] = 'g';
}

bool backtracking(char** maze1, int r, int c, int rS, int cS, int rD, int cD) {
	cellStack<int> s1;	//obstacle: x		visited-obstacle: v		free-space: z		path: o		goal: g

	int currR = rS, currC = cS;
	s1.push(currR, currC);

	while (currR != rD || currC != cD) {

		if (currC + 1 < c) {
			if (maze1[currR][currC + 1] == 'z') {
				currC++;
				s1.push(currR, currC);
				maze1[currR][currC] = 'o';
				continue;
			}
			else if (maze1[currR][currC + 1] == 'g') return true;
		}
		if (currR + 1 < r) {
			if (maze1[currR + 1][currC] == 'z') {
				currR++;
				s1.push(currR, currC);
				maze1[currR][currC] = 'o';
				continue;
			}
			else if (maze1[currR + 1][currC] == 'g') return true;
		}
		if (currC - 1 >= 0) {
			if (maze1[currR][currC - 1] == 'z') {
				currC--;
				s1.push(currR, currC);
				maze1[currR][currC] = 'o';
				continue;
			}
			else if (maze1[currR][currC - 1] == 'g') return true;
		}
		if (currR - 1 >= 0) {
			if (maze1[currR - 1][currC] == 'z') {
				currR--;
				s1.push(currR, currC);
				maze1[currR][currC] = 'o';
				continue;
			}
			else if (maze1[currR - 1][currC] == 'g') return true;
		}

		maze1[currR][currC] = 'v';
		s1.pop();
		if (!s1.topValue(currR, currC)) break;
	}
	return false;
}

void leesalgo(int** maze2, int r, int c, int rS, int cS, int rD, int cD) {		//obstacle: -2, printed x		free space:	-1, printed z
																				//goal: 0, printed g			source: -3
	cellQueue<int> q1;
	q1.enqueue(rD, cD);

	int currR = rD, currC = cD, count = 1, newcount = 0, steps = 1;		//count: current queue's entries
	bool notfound = true;												//newcount: new entries generated by current queue's entries (X 4)
																		//steps: used to update pathway cells to +1 of previous one
	while (notfound) {
		while (count > 0) {
			q1.Front(currR, currC);
			q1.dequeue();

			if (currC + 1 < c) {
				if (maze2[currR][currC + 1] == -1) {
					q1.enqueue(currR, currC + 1);
					newcount++;
					maze2[currR][currC + 1] = steps;
				}
				else if (maze2[currR][currC + 1] == -3) {
					notfound = false;
					maze2[currR][currC + 1] = steps;
				}
			}
			if (currR + 1 < r) {
				if (maze2[currR + 1][currC] == -1) {
					q1.enqueue(currR + 1, currC);
					newcount++;
					maze2[currR + 1][currC] = steps;
				}
				else if (maze2[currR + 1][currC] == -3) {
					notfound = false;
					maze2[currR + 1][currC] = steps;
				}
			}
			if (currC - 1 >= 0) {
				if (maze2[currR][currC - 1] == -1) {
					q1.enqueue(currR, currC - 1);
					newcount++;
					maze2[currR][currC - 1] = steps;
				}
				else if (maze2[currR][currC - 1] == -3) {
					notfound = false;
					maze2[currR][currC - 1] = steps;
				}
			}
			if (currR - 1 >= 0) {
				if (maze2[currR - 1][currC] == -1) {
					q1.enqueue(currR - 1, currC);
					newcount++;
					maze2[currR - 1][currC] = steps;
				}
				else if (maze2[currR - 1][currC] == -3) {
					notfound = false;
					maze2[currR - 1][currC] = steps;
				}
			}
			count--;
		}

		count = newcount;
		newcount = 0;
		steps++;
	}
}

int countStack(char** array, int r, int c) {
	int count = 0;
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (array[i][j] == 'o') count++;
	return count;
}

int countQueue(int** array, int r, int c) {
	int count = 0;
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (count < array[i][j]) count = array[i][j];
	return count;
}