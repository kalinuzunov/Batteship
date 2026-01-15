#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

const char WATER = '~';
const char HIT = 'X';
const char MISS = 'O';
const char SHIP = '#';
const char SEPARATION = '|';
const char EMPTY_SPACE = ' ';

const int MAX_FIELD_ROWS = 16;
const int MAX_FIELD_LENGTH = 32;
const int AMOUNT_OF_SHIPS = 10;
const int AMOUNT_OF_SHIPS_TILES = 1 * 4 + 2 * 3 + 3 * 2 + 4 * 1;
const int SIZE_OF_TEXT = 100;

const int COLOR_WATER = 9;
const int COLOR_SHIP = 10;
const int COLOR_HIT = 12;
const int COLOR_MISS = 8;
const int COLOR_SEPARATION = 14;
const int COLOR_TEXT = 7;

void clearConsole()
{
	system("cls");
}

void setColor(int color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void clearInputStream()
{
	std::cin.clear();
	std::cin.ignore(10000, '\n');
}

bool isNumber(char sym)
{
	return sym <= '9' && sym >= '0';
}

bool areAllDigits(char* text)
{
	if (text == nullptr)return false;
	while (*text)
	{
		if (!isNumber(*text))
		{
			return false;
		}
		text++;
	}
	return true;
}

int charToInt(char sym)
{
	return sym - '0';
}

int stringToInt(char* text)
{
	if (text == nullptr || !areAllDigits(text)) return 0;
	int result = 0;
	while (*text)
	{
		result = result * 10 + charToInt(*text);
		text++;
	}
	return result;
}

int getValidInt()
{
	char buffer[SIZE_OF_TEXT];
	if (!(std::cin >> buffer))
	{

		return -1;
	}

	if (areAllDigits(buffer))
	{
		return stringToInt(buffer);
	}
	else
	{

		return -1;
	}
}

void createBattleField(char field[MAX_FIELD_ROWS][MAX_FIELD_LENGTH], int n)
{
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 1; j <= (n * 2 + 1); ++j)
		{
			if (j == n + 1) field[i][j] = SEPARATION;
			else field[i][j] = WATER;
		}
	}
}

int myStrLen(const char* text)
{
	if (!text)
		return 0;
	int len = 0;
	while (*text)
	{
		len++;
		text++;
	}
	return len;
}

void printHeaders(int n)
{
	std::cout << "   ";

	int width = n * 2;

	int textLenComputer = myStrLen("COMPUTER");
	int spacesComputer = (width - textLenComputer) / 2;

	for (int k = 0; k < spacesComputer; ++k) std::cout << EMPTY_SPACE;
	std::cout << "COMPUTER";
	for (int k = 0; k < spacesComputer; ++k) std::cout << EMPTY_SPACE;
	if ((width - textLenComputer) % 2 != 0) std::cout << EMPTY_SPACE;

	std::cout << "  ";
	int textLenPlayer = myStrLen("PLAYER");
	int spacesPlayer = (width - textLenPlayer) / 2;

	for (int k = 0; k < spacesPlayer; ++k) std::cout << " ";
	std::cout << "PLAYER";

	std::cout << std::endl;
}

void printNumbers(int n)
{
	std::cout << "   ";
	for (int j = 1; j <= n; ++j) std::cout << j / 10 << " ";
	std::cout << "  ";
	for (int j = 1; j <= n; ++j) std::cout << j / 10 << " ";
	std::cout << std::endl;

	std::cout << "   ";
	for (int j = 1; j <= n; ++j) std::cout << j % 10 << " ";
	std::cout << "  ";
	for (int j = 1; j <= n; ++j) std::cout << j % 10 << " ";
	std::cout << std::endl;
}

void printBattleField(char field[MAX_FIELD_ROWS][MAX_FIELD_LENGTH], int n)
{
	clearConsole();
	setColor(COLOR_TEXT);
	printHeaders(n);
	printNumbers(n);
	int middle = n + 1;
	for (int i = 1; i <= n; ++i)
	{
		setColor(COLOR_TEXT);
		if (i < 10) std::cout << "0" << i << " ";
		else std::cout << i << " ";

		for (int j = 1; j <= (n * 2 + 1); ++j)
		{
			char currentSym = field[i][j];
			if (currentSym == WATER) setColor(COLOR_WATER);
			else if (currentSym == HIT) setColor(COLOR_HIT);
			else if (currentSym == MISS) setColor(COLOR_MISS);
			else if (currentSym == SEPARATION) setColor(COLOR_SEPARATION);
			else if (currentSym == SHIP)
			{
				if (j < middle) setColor(COLOR_WATER);
				else setColor(COLOR_SHIP);
			}
			if (j < middle && field[i][j] == SHIP)
			{
				std::cout << WATER;
			}
			else
			{
				std::cout << field[i][j];
			}
			setColor(COLOR_TEXT);
			std::cout << EMPTY_SPACE;
		}
		if (i < 10) std::cout << "0" << i;
		else std::cout << i;

		std::cout << std::endl;
	}
}

void getValidCoordinates(int& row, int& col, int n) {
	while (true) {
		row = getValidInt();
		col = getValidInt();

		if (row >= 1 && row <= n && col >= 1 && col <= n) {
			break;
		}
		std::cout << "Invalid input! Please enter two numbers between 1 and " << n << ": ";

		clearInputStream();
	}
}

bool areShipsNotDirectlyNextToEachOther(char field[][MAX_FIELD_LENGTH], int row, int col, char direction, int k, int n)
{
	for (int i = 0; i < k; ++i)
	{
		int Row = (direction == 'v') ? (row + i) : row;
		int Col = (direction == 'h') ? (col + i) : col;

		if (Row - 1 >= 1 && field[Row - 1][Col] == SHIP)
		{
			return false;
		}
		if (Row + 1 <= n && field[Row + 1][Col] == SHIP)
		{
			return false;
		}
		if (Col - 1 >= 1 && field[Row][Col - 1] == SHIP && field[Row][Col - 1] != SEPARATION)
		{
			return false;
		}
		if (Col + 1 < MAX_FIELD_LENGTH && field[Row][Col + 1] == SHIP && field[Row][Col + 1] != SEPARATION)
		{
			return false;
		}
	}
	return true;
}

bool isItValidPlaceToPutShipWithLengthOfK(char field[][MAX_FIELD_LENGTH], int row, int col, char direction, int k, int n)
{
	int middle = n + 1;
	if (direction == 'h')
	{
		if (col + k - 1 > n)
		{
			return false;
		}
		for (int i = 0; i < k; ++i)
		{
			if (field[row][col + i + middle] != WATER)
			{
				return false;
			}
		}
	}
	else if (direction == 'v')
	{
		if (row + k - 1 > n)
		{
			return false;
		}
		for (int i = 0; i < k; ++i)
		{
			if (field[row + i][col + middle] != WATER)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	if (!areShipsNotDirectlyNextToEachOther(field, row, col + middle, direction, k, n))
	{
		return false;
	}

	return true;
}

bool areValidNumbersForRowsAndCols(int row, int col, int n)
{
	if (row < 1 || row > n || col < 1 || col > n)
		return false;
	return true;
}

void printPlacementInstructions() {
	std::cout << "================================================================" << std::endl;
	std::cout << "                   SHIP PLACEMENT INSTRUCTIONS                  " << std::endl;
	std::cout << "================================================================" << std::endl;
	std::cout << "* To place a ship, enter starting coordinates (Row and Col).   *" << std::endl;
	std::cout << "* Then choose direction: 'h' (horizontal) or 'v' (vertical).   *" << std::endl;
	std::cout << "* 'h' - will place your ship to the right and 'v' - downwards. *" << std::endl;
	std::cout << "* The rest of the ship will be placed automatically.           *" << std::endl;
	std::cout << "* RULE: Ships cannot be directly next to each other!           *" << std::endl;
	std::cout << "================================================================" << std::endl;
	std::cout << "Press any key to start placing your ships..." << std::endl;
	system("pause");
}

void manualPlacingOfShips(char field[][MAX_FIELD_LENGTH], int n)
{
	clearConsole();
	printPlacementInstructions();

	int sizes[AMOUNT_OF_SHIPS] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	int middle = n + 1;

	for (int i = 0; i < AMOUNT_OF_SHIPS; ++i)
	{
		printBattleField(field, n);
		int row, col;
		char direction = 'h';
		bool placed = false;

		while (!placed)
		{
			std::cout << std::endl << "Placing ship of size " << sizes[i] << "(" << 10 - i << " ships left)" << std::endl;
			std::cout << "Enter starting coordinates (row and col): ";

			getValidCoordinates(row, col, n);
			clearInputStream();

			if (sizes[i] > 1) {
				std::cout << "Choose direction - horizontal(h) or vertical(v): ";
				std::cin >> direction;
				clearInputStream();

				while (direction != 'h' && direction != 'v')
				{
					std::cout << "Invalid direction! Use 'h' or 'v': ";
					clearInputStream();
					std::cin >> direction;
				}

			}
			else
			{
				direction = 'h';
			}

			if (isItValidPlaceToPutShipWithLengthOfK(field, row, col, direction, sizes[i], n))
			{
				if (direction == 'h') {
					for (int j = 0; j < sizes[i]; ++j)
						field[row][col + middle + j] = SHIP;
				}
				else {
					for (int j = 0; j < sizes[i]; ++j)
						field[row + j][col + middle] = SHIP;
				}
				placed = true;
			}
			else {
				std::cout << "Error: You can't place it there! (Out of bounds or next to another ship)" << std::endl;
				std::cout << "Try again for this ship." << std::endl;
				Sleep(1500);
			}

		}
	}

	printBattleField(field, n);
	std::cout << "All ships placed! Get ready for battle...";
	Sleep(2000);
}

bool isItValidPlaceForAutomatic(char field[][MAX_FIELD_LENGTH], int row, int col, char direction, int k, int n, bool isPlayer)
{
	int startCol = isPlayer ? (n + 1 + col) : col;
	int limitCol = isPlayer ? (2 * n + 1) : n;

	if (direction == 'h')
	{
		if (startCol + k - 1 > limitCol)
		{
			return false;
		}
		for (int i = 0; i < k; ++i)
		{
			if (field[row][startCol + i] != WATER)
			{
				return false;
			}
		}
	}
	else if (direction == 'v')
	{
		if (row + k - 1 > n)
		{
			return false;
		}
		for (int i = 0; i < k; ++i)
		{
			if (field[row + i][startCol] != WATER)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	if (!areShipsNotDirectlyNextToEachOther(field, row, startCol, direction, k, n))
	{
		return false;
	}

	return true;
}

void automaticPlacing(char field[][MAX_FIELD_LENGTH], int size, bool isPlayer)
{
	int sizes[AMOUNT_OF_SHIPS] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	int middle = size + 1;

	for (int i = 0; i < AMOUNT_OF_SHIPS; ++i)
	{
		bool placed = false;
		while (!placed)
		{
			int row = (rand() % size) + 1;
			int col = (rand() % size) + 1;
			char direction = (rand() % 2 == 0) ? 'h' : 'v';

			if (isItValidPlaceForAutomatic(field, row, col, direction, sizes[i], size, isPlayer))
			{
				int startCol = isPlayer ? (middle + col) : col;

				if (direction == 'h')
				{
					for (int j = 0; j < sizes[i]; ++j)
						field[row][startCol + j] = SHIP;
				}
				else
				{
					for (int j = 0; j < sizes[i]; ++j)
						field[row + j][startCol] = SHIP;
				}
				placed = true;
			}
		}
	}
}

void startOrLoadGame()
{
	int k;
	std::cout << "Choose to start a new game or to replay old one:" << std::endl;
	std::cout << "1. Start new game" << std::endl;
	std::cout << "2. Load old game" << std::endl;
	k = getValidInt();
	while (k != 1 && k != 2)
	{
		std::cout << "Invalid input! Choose 1 or 2: ";
		k = getValidInt();
	}
	clearInputStream();
	switch (k)
	{
	case 1:
		Sleep(100);
		clearConsole(); return; break;
	case 2:break;
	}
}

void chooseDifficulty(int& n)
{
	std::cout << "Choose Difficulty:" << std::endl;
	std::cout << "1. Calm Waters (field 10x10)  " << std::endl;
	std::cout << "2. Rough Seas (field 12x12) " << std::endl;
	std::cout << "3. Storm of Steel (field 15x15)  " << std::endl;
	n = getValidInt();
	while (n != 1 && n != 2 && n != 3)
	{
		std::cout << "Invalid input! Choose one of the presented options:" << std::endl;
		n = getValidInt();
	}
	clearInputStream();
	switch (n)
	{
	case 1: n = 10; break;
	case 2: n = 12; break;
	case 3: n = 15; break;
	}
	Sleep(100);
	clearConsole();
}

void chooseManualOrAutomaticPlacedShips(char field[][MAX_FIELD_LENGTH], int size)
{
	int k;
	std::cout << "Choose how to place your ships:" << std::endl;
	std::cout << "1. Manual placing" << std::endl;
	std::cout << "2. Automatic placing" << std::endl;
	k = getValidInt();

	while (k != 1 && k != 2)
	{
		std::cout << "Invalid input! Choose 1 or 2: ";
		k = getValidInt();
	}
	clearInputStream();
	switch (k)
	{
	case 1:
		manualPlacingOfShips(field, size);
		break;
	case 2:
	{
		automaticPlacing(field, size, true);
		printBattleField(field, size);
		std::cout << "Ships placed automatically!";
		break;
	}
	}

}

bool isAShipSunk(char field[][MAX_FIELD_LENGTH], int row, int col, int n)
{
	int tempCol = col - 1;
	while (tempCol >= 1 && field[row][tempCol] != WATER && field[row][tempCol] != MISS && field[row][tempCol] != SEPARATION)
	{
		if (field[row][tempCol] == SHIP)
			return false;
		tempCol--;
	}
	tempCol = col + 1;
	while (tempCol <= (2 * n + 1) && field[row][tempCol] != WATER && field[row][tempCol] != MISS && field[row][tempCol] != SEPARATION)
	{
		if (field[row][tempCol] == SHIP)
			return false;
		tempCol++;
	}
	int tempRow = row - 1;
	while (tempRow >= 1 && field[tempRow][col] != WATER && field[tempRow][col] != MISS)
	{
		if (field[tempRow][col] == SHIP)
			return false;
		tempRow--;
	}
	tempRow = row + 1;
	while (tempRow <= n && field[tempRow][col] != WATER && field[tempRow][col] != MISS)
	{
		if (field[tempRow][col] == SHIP)
			return false;
		tempRow++;
	}
	return true;
}

bool isThisTileAlreadyShot(char field[][MAX_FIELD_LENGTH], int row, int col)
{
	return field[row][col] == HIT || field[row][col] == MISS;
}

bool canAttackThere(char field[][MAX_FIELD_LENGTH], int n, int row, int col)
{
	int middle = n + 1;
	if (row > n || row < 1)
	{
		return false;
	}
	if (col > middle || col < 1)
	{
		return false;
	}
	return true;
}

bool areAllShipTilesHit(int k)
{
	return k == AMOUNT_OF_SHIPS_TILES;
}

void playerAttack(char field[][MAX_FIELD_LENGTH], int n, int& amountOfHitTiles)
{
	bool isHit = true;
	while (isHit) {
		int row, col;
		printBattleField(field, n);

		std::cout << "Choose where to shoot (row and col): ";
		getValidCoordinates(row, col, n);

		if (isThisTileAlreadyShot(field, row, col)) {
			std::cout << "You already shot there! Try again." << std::endl;
			Sleep(1500);
			continue;
		}

		if (field[row][col] == SHIP) {
			field[row][col] = HIT;
			printBattleField(field, n);
			if (isAShipSunk(field, row, col, n)) {
				std::cout << "BOOM! You SUNK an enemy ship!" << std::endl;
				Sleep(1500);
			}
			amountOfHitTiles++;
			if (areAllShipTilesHit(amountOfHitTiles))
				return;

			std::cout << "YOU HIT! Shoot again." << std::endl;
			Sleep(1500);

		}
		else {
			field[row][col] = MISS;
			printBattleField(field, n);
			std::cout << "MISS! Computer's turn." << std::endl;
			Sleep(1500);
			isHit = false;
			clearInputStream();
		}
	}
}

bool canAttackThereForComputer(char field[][MAX_FIELD_LENGTH], int n, int row, int col)
{
	int middle = n + 1;
	if (row > n || row < 1)
	{
		return false;
	}
	if (col < middle || col > MAX_FIELD_LENGTH - 1)
	{
		return false;
	}
	return true;
}

void computerAttack(char field[][MAX_FIELD_LENGTH], int n, int& amountOfHitTiles)
{
	int middle = n + 1;
	int row, col;
	bool isHit = true;
	while (isHit)
	{
		row = (rand() % n) + 1;
		col = (rand() % n) + 1;
		col += middle;
		while (isThisTileAlreadyShot(field, row, col) || !canAttackThereForComputer(field, n, row, col))
		{
			row = (rand() % n) + 1;
			col = (rand() % n) + 1;
			col += middle;
		}
		std::cout << "Computer is thinking...";
		Sleep(1500);
		if (field[row][col] == SHIP)
		{
			field[row][col] = HIT;
			amountOfHitTiles++;
			if (areAllShipTilesHit(amountOfHitTiles)) return;
			printBattleField(field, n);
			isHit = true;
			std::cout << "Computer hit your ship at:" << row << EMPTY_SPACE << col - middle << '!' << std::endl;
			Sleep(1500);
			if (isAShipSunk(field, row, col, n))
			{
				std::cout << "BOOM! Computer just SUNK one of your ships!" << std::endl;
				Sleep(1500);
			}
			std::cout << "Computer thinking for its next shot...";
			Sleep(1500);
		}
		else
		{
			field[row][col] = MISS;
			printBattleField(field, n);
			isHit = false;
			std::cout << "Computer missed your ships at:" << row << EMPTY_SPACE << col - middle << '!' << std::endl;
			Sleep(1500);
		}
	}
}

void attacking(char field[][MAX_FIELD_LENGTH], int n)
{
	int amountOfHitTilesPlayer = 0;
	int amountOfHitTilesComputer = 0;
	while (true)
	{
		playerAttack(field, n, amountOfHitTilesPlayer);
		if (areAllShipTilesHit(amountOfHitTilesPlayer))
		{
			std::cout << std::endl << "CONGRATULATIONS YOU HAVE SUCCESSFULLY SUNK ALL OF THE ENEMY'S SHIPS" << std::endl << "YOU WIN!!!";
			break;
		}
		computerAttack(field, n, amountOfHitTilesComputer);
		if (areAllShipTilesHit(amountOfHitTilesComputer))
		{
			std::cout << std::endl << "YOUR OPPONENT HAS SUNK ALL YOUR SHIPS" << std::endl << "YOU LOSE! BETTER LUCK NEXT TIME!";
			break;
		}
	}
}

void startGame()
{
	int n;
	char field[MAX_FIELD_ROWS][MAX_FIELD_LENGTH];
	startOrLoadGame();
	chooseDifficulty(n);
	createBattleField(field, n);
	automaticPlacing(field, n, false);
	chooseManualOrAutomaticPlacedShips(field, n);
	attacking(field, n);
}

int main()
{
	srand((unsigned int)time(0));
	startGame();
	return 0;
}