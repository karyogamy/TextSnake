#include "snakeMain.h"

snakeDot* initSnake(int x, int y, int v) {
	snakeDot* pHead = (snakeDot*) malloc (sizeof(snakeDot));
	pHead->x = x;
	pHead->y = y;
	pHead->vector = v;
	pHead->next = NULL;
	return pHead;
}

snakeDot* getTail(snakeDot* head) {
	if (head->next == NULL)
		return head;
	else
		return getTail(head->next);
}

snakeDot* getSecondLastTail(snakeDot* head) {
	if (head->next == NULL)
		return NULL;
	if (head->next->next == NULL)
		return head;
	else
		return getSecondLastTail(head->next);
}

void addSnakeDot(snakeDot* head) {
	snakeDot* tail = getTail(head);
	//vect is one of 0 (R), 1 (D), 2(L), 3(U).
	snakeDot* newTail;
	switch (tail->vector) {
		case 0:
			newTail = initSnake(tail->x-1, tail->y, tail->vector);
			tail->next = newTail;
			break;
		case 1:
			newTail = initSnake(tail->x, tail->y+1, tail->vector);
			tail->next = newTail;
			break;
		case 2:
			newTail = initSnake(tail->x+1, tail->y, tail->vector);
			tail->next = newTail;
			break;
		case 3:
			newTail = initSnake(tail->x, tail->y-1, tail->vector);
			tail->next = newTail;
			break;
	}
	return;
}

snakeDot* 	updateSnake(snakeDot* head, int vect) {
	//cout << head->x << head->y << head->vector << endl;
	//Add new head and delete tail is faster than recursively updating the list

	snakeDot* newHead = (snakeDot*)malloc(sizeof(snakeDot));
	memcpy(newHead, head, sizeof(snakeDot));

	if (head != NULL && newHead != NULL) {
		newHead->vector = vect;
		switch(vect) {
			case 0:
				newHead->x++;
				break;
			case 1:
				newHead->y--;
				break;
			case 2:
				newHead->x--;
				break;
			case 3:
				newHead->y++;
				break;
		}
		newHead->next = head;
		snakeDot* newTail;
		if ((newTail = getSecondLastTail(newHead)) != NULL)
			newTail->next = NULL;
		//updateSnake(head->next, vect);
	}
	return newHead;
}

bool checkSelfCollision (snakeDot* head, snakeDot* obj) {
	if (head == NULL || obj == NULL)
		return false;
	if (head->x == obj->x && head->y == obj->y)
		return true;
	else
		checkSelfCollision (head, obj->next);
	return false;
}

void printSnake (snakeDot* head) {
	if (head != NULL) {
		cout << "Head: (" << head->x << " ," << head->y << ")" << endl;
	}
	int count = 0;
	while (head->next != NULL) {
		head = head->next;
		cout << "Body " << ++count << ": (" << head->x << " ," << head->y << ")" << endl;
	}
	return;
}

bool checkBorderCollision(snakeDot* head, int xMax, int yMax) {
	if (xMax == 0 && yMax == 0)
		return false;
	
	if (xMax > 0 && ((head->x < 0) || (head->x >= xMax)))
		return true;

	if (yMax > 0 && ((head->y < 0) || (head->y >= yMax)))
		return true;

	return false;
}

bool checkFoodCollision (snakeDot* head, Food food) {
	if (head == NULL)
		return false;
	if (head->x == food.x && head->y == food.y)
		return true;
	return false;
	//else -- if head doesn't hit, then body won't hit
	//	checkFoodCollision (head->next, food);
	//return false; -- unreachable code
}

Food genRandFood(int xMax, int yMax) {
	int randXSign, randYSign;
	int randX, randY;
	if (xMax == 0) {
		randX = rand() % 10;
		randXSign = rand() % 100;
		if (randXSign >= 50)
			randX *= -1;
	} else {
		randX = rand() % xMax;
	}

	if (yMax == 0) {
		randY = rand() % 10;
		randYSign = rand() % 100;
		if (randYSign >= 50)
			randY *= -1;
	} else {
		randY = rand() % yMax;
	}

	Food food;
	food.x = randX;
	food.y = randY;
	return food;
}

void main() {
	//Setup variables
	snakeDot* headSnake;
	bool isFoodOnMap = false;
	int vect = 0; //vect is one of 0 (R), 1 (D), 2(L), 3(U).
	Food food;
	int sleepTime;
	int xMax, yMax;

	//Get user-defined border dimensions
	cout << "Please enter the dimension for the border: " << endl;
	cout << "Enter dimension of X (0 if no border on x-axis): ";
	cin >> xMax;

	cout << "Enter dimension of Y (0 if no border on y-axis): ";
	cin >> yMax;

	//Make sure x and y max are valid values
	if (xMax < 0 || yMax < 0) {
		xMax = 0;
		yMax = 0;
	}

	//Speed of the game 
	cout << "Difficulty [500-2000]: ";
	cin >> sleepTime;
	cout << "Difficulty is " << sleepTime << endl << "Game Starting..." << endl;

	//Make sure sleep gets a valid value
	if (sleepTime < 500 || sleepTime > 2000) {
		sleepTime = 1000;
	}
	Sleep(1000);

	//Do setup
	int exitState = 0;
	srand((unsigned int) time(NULL));
	headSnake = initSnake(xMax/2, yMax/2, 0);

	//Game running - loop until ESC key pressed
	while (!exitState) {
		headSnake = updateSnake(headSnake, vect);

		if (checkBorderCollision(headSnake, xMax, yMax)) {
			cout << "Border Collision. Game Over, exiting..." << endl;
			return;
		}

		if (checkSelfCollision(headSnake, headSnake->next)) {
			cout << "Self Collision. Game Over, exiting..." << endl;
			return;
		}
		
		if (!isFoodOnMap) {
			food = genRandFood(xMax, yMax);
			isFoodOnMap = true;
		}

		cout << "Food: (" << food.x << " ," << food.y << ")" << endl;

		if (checkFoodCollision(headSnake, food)) {
			isFoodOnMap = false;
			addSnakeDot(headSnake);
		}

		if (GetAsyncKeyState(VK_UP)) {
			if (headSnake->next == NULL || vect != 1)
				vect = 3;
		} else if (GetAsyncKeyState(VK_DOWN)) {
			if (headSnake->next == NULL || vect != 3)
				vect = 1;
		} else if (GetAsyncKeyState(VK_LEFT)) {
			if (headSnake->next == NULL || vect != 0)
				vect = 2;
		} else if (GetAsyncKeyState(VK_RIGHT)) {
			if (headSnake->next == NULL || vect != 2)
				vect = 0;
		} else if (GetAsyncKeyState(VK_ESCAPE)) {
			cout << "exiting..." << endl;
			exitState = 1;
		}

		printSnake(headSnake);

		cout << endl;
		switch (vect) {
		case 0:
			cout << "RIGHT" << endl;
			break;
		case 1:
			cout << "DOWN" << endl;
			break;
		case 2:
			cout << "LEFT" << endl;
			break;
		case 3:
			cout << "UP" << endl;
			break;
		}
		cout << endl;

		Sleep(sleepTime);
	}

	return;
}

