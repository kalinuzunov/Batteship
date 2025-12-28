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
const int MAX_FIELD_LENGHT = 32;
const int AMOUNT_OF_SHIPS = 10;
const int AMOUNT_OF_SHIPS_TILES = 1 * 4 + 2 * 3 + 3 * 2 + 4 * 1;
const int SIZE_OF_TEXT = 100;

void clearConsole()
{
	system("cls");
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
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		return -1;
	}

	if (areAllDigits(buffer))
	{
		return stringToInt(buffer);
	}
	else
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		return -1;
	}
}

void createBattleField(char field[MAX_FIELD_ROWS][MAX_FIELD_LENGHT], int n)
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

void printBattleField(char field[MAX_FIELD_ROWS][MAX_FIELD_LENGHT], int n)
{
	clearConsole();
	printNumbers(n);
	int middle = n + 1;
	for (int i = 1; i <= n; ++i)
	{
		if (i < 10) std::cout << "0" << i << " ";
		else std::cout << i << " ";

		for (int j = 1; j <= (n * 2 + 1); ++j)
		{
			if (j < middle && field[i][j] == SHIP)
			{
				std::cout << WATER << EMPTY_SPACE;
			}
			else std::cout << field[i][j] << EMPTY_SPACE;
		}

		if (i < 10) std::cout << "0" << i;
		else std::cout << i;

		std::cout << std::endl;
	}
}

bool areShipsNotDirectlyNextToEachOther(char field[][MAX_FIELD_LENGHT], int row, int coll, char direction, int k, int n)
{
	for (int i = 0; i < k; ++i)
	{
		int Row = (direction == 'v') ? (row + i) : row;
		int Coll = (direction == 'h') ? (coll + i) : coll;

		if (Row - 1 >= 1 && field[Row - 1][Coll] == SHIP)
		{
			return false;
		}
		if (Row + 1 <= n && field[Row + 1][Coll] == SHIP)
		{
			return false;
		}
		if (Coll - 1 >= 1 && field[Row][Coll - 1] == SHIP && field[Row][Coll - 1] != SEPARATION)
		{
			return false;
		}
		if (Coll + 1 < MAX_FIELD_LENGHT && field[Row][Coll + 1] == SHIP && field[Row][Coll + 1] != SEPARATION)
		{
			return false;
		}
	}
	return true;
}

bool isItValidPlaceToPutShipWiyhLenghtOfK(char field[][MAX_FIELD_LENGHT], int row, int coll, char direction, int k, int n)
{
	int middle = n + 1;
	if (direction == 'h')
	{
		if (coll + k - 1 > n)
		{
			return false;
		}
		for (int i = 0; i < k; ++i)
		{
			if (field[row][coll + i + middle] != WATER)
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
			if (field[row + i][coll + middle] != WATER)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	if (!areShipsNotDirectlyNextToEachOther(field, row, coll + middle, direction, k, n))
	{
		return false;
	}

	return true;
}

bool areValidNumbersForRowsAndColls(int row, int coll, int n)
{
	if (row < 1 || row > n || coll < 1 || coll > n)
		return false;
	return true;
}

void manulalPlacingOfShips(char field[][MAX_FIELD_LENGHT], int n)
{
	printBattleField(field, n);
	int middle = n + 1;
	int sizes[AMOUNT_OF_SHIPS] = { 4,3,3,2,2,2,1,1,1,1 };
	for (int i = 0; i < AMOUNT_OF_SHIPS; ++i)
	{
		int row, coll;
		char direction;
		bool allShipsPlaced = false;
		while (!allShipsPlaced)
		{
			std::cout << "Place your next ship which is " << sizes[i] << " tiles long.(ships left to place:" << 10 - i << ")" << std::endl;
			std::cout << "To place your ship first input the starting coordinats and after that choose it to be horizontal(h) or vertical(v)." << std::endl;
			std::cout << "After you type in the coordinates of the first tile of your ship," << std::endl << "the rest of the ship wiil be placed autumaticly based of the direction " << std::endl;
			std::cout << "You can not place ships directly next to each other!" << std::endl;
			std::cout << "Input your coordinats:" << std::endl;
			row = getValidInt();
			coll = getValidInt();
			while (!areValidNumbersForRowsAndColls(row, coll, n))
			{
				std::cout << "Invalid coordinats please input new ones:";
				row = getValidInt();
				coll = getValidInt();
			}
			if (sizes[i] != 1)
			{
				std::cout << "Choosee horizontal or vertica(h or v):" << std::endl;
				std::cin >> direction;
				while (direction != 'h' && direction != 'v')
				{
					std::cout << "Invalid direction please input new one:";
					std::cin >> direction;
				}
				while (!isItValidPlaceToPutShipWiyhLenghtOfK(field, row, coll, direction, sizes[i], n))
				{
					std::cout << "You can not place ship of that lenght there!" << std::endl;
					std::cout << "Please enter new values for row coll:";
					row = getValidInt();
					coll = getValidInt();
					while (!areValidNumbersForRowsAndColls(row, coll, n))
					{
						std::cout << "Invalid coordinats please input new ones:";
						row = getValidInt();
						coll = getValidInt();
					}
					std::cout << "Please enter new dirction(h or v):";
					std::cin >> direction;
					while (direction != 'h' && direction != 'v')
					{
						std::cout << "Invalid direction please input new one:";
						std::cin >> direction;
					}
				}
				if (direction == 'h')
				{
					for (int j = 0; j < sizes[i]; ++j)
					{
						field[row][coll + middle + j] = SHIP;
					}
				}
				else if (direction == 'v')
				{
					for (int j = 0; j < sizes[i]; ++j)
					{
						field[row + j][coll + middle] = SHIP;
					}
				}
			}
			else if (sizes[i] == 1)
				while (!isItValidPlaceToPutShipWiyhLenghtOfK(field, row, coll, 'h', 1, n))
				{
					std::cout << "You cannot place a ship nex to each other! New Row and Col: ";
					row = getValidInt();
					coll = getValidInt();
					while (!areValidNumbersForRowsAndColls(row, coll, n)) {
						row = getValidInt();
						coll = getValidInt();
					}
				}
			field[row][coll + middle] = SHIP;
			printBattleField(field, n);
			allShipsPlaced = true;
		}
	}
}

bool isItValidPlaceForAutumatic(char field[][MAX_FIELD_LENGHT], int row, int coll, char direction, int k, int n, bool isPlayer)
{
	int startCol = isPlayer ? (n + 1 + coll) : coll;
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

void automaticPlacing(char field[][MAX_FIELD_LENGHT], int size, bool isPlayer)
{
	int sizes[AMOUNT_OF_SHIPS] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	int middle = size + 1;

	for (int i = 0; i < AMOUNT_OF_SHIPS; ++i)
	{
		bool placed = false;
		while (!placed)
		{
			int row = (rand() % size) + 1;
			int coll = (rand() % size) + 1;
			char direction = (rand() % 2 == 0) ? 'h' : 'v';

			if (isItValidPlaceForAutumatic(field, row, coll, direction, sizes[i], size, isPlayer))
			{
				int startCol = isPlayer ? (middle + coll) : coll;

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

void chooseDifficulty(int& n)
{
	std::cout << "Choose Difficulty:" << std::endl;
	std::cout << "1. Calm Waters (field 10x10)  " << std::endl;
	std::cout << "2. Rough Seas (field 12x12) " << std::endl;
	std::cout << "3. Storm of Steel (field 15x15)  " << std::endl;
	n = getValidInt();
	while (n != 1 && n != 2 && n != 3)
	{
		std::cout << "Invalid input! Choose one of the presentet optons:" << std::endl;
		n = getValidInt();
	}
	switch (n)
	{
	case 1: n = 10; break;
	case 2: n = 12; break;
	case 3: n = 15; break;
	}
	Sleep(200);
	clearConsole();
}

void chooseManualOrAutumaticPlacedShips(char field[][MAX_FIELD_LENGHT], int size)
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

	switch (k)
	{
	case 1:
		manulalPlacingOfShips(field, size);
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

bool isAShipSunk(char field[][MAX_FIELD_LENGHT], int row, int coll, int n)
{
	int tempColl = coll - 1;
	while (tempColl >= 1 && field[row][tempColl] != WATER && field[row][tempColl] != MISS && field[row][tempColl] != SEPARATION)
	{
		if (field[row][tempColl] == SHIP)
			return false;
		tempColl--;
	}
	tempColl = coll + 1;
	while (tempColl <= (2 * n + 1) && field[row][tempColl] != WATER && field[row][tempColl] != MISS && field[row][tempColl] != SEPARATION)
	{
		if (field[row][tempColl] == SHIP)
			return false;
		tempColl++;
	}
	int tempRow = row - 1;
	while (tempRow >= 1 && field[tempRow][coll] != WATER && field[tempRow][coll] != MISS)
	{
		if (field[tempRow][coll] == SHIP)
			return false;
		tempRow--;
	}
	tempRow = row + 1;
	while (tempRow <= n && field[tempRow][coll] != WATER && field[tempRow][coll] != MISS)
	{
		if (field[tempRow][coll] == SHIP)
			return false;
		tempRow++;
	}
	return true;
}

bool isThisTileAlreadyShooted(char field[][MAX_FIELD_LENGHT], int row, int coll)
{
	return field[row][coll] == HIT || field[row][coll] == MISS;
}

bool canAttakThere(char field[][MAX_FIELD_LENGHT], int n, int row, int coll)
{
	int middle = n + 1;
	if (row > n || row < 1)
	{
		return false;
	}
	if (coll > middle || coll < 1)
	{
		return false;
	}
	return true;
}

bool areAllShipTilesHitted(int k)
{
	return k == AMOUNT_OF_SHIPS_TILES;
}

void playerAttack(char field[][MAX_FIELD_LENGHT], int n, int& amountOfHittedTiles)
{
	bool isHit = true;
	while (isHit)
	{
		int row, coll;
		printBattleField(field, n);
		std::cout << "Choose where to shoot: ";
		row = getValidInt();
		coll = getValidInt();

		while (!canAttakThere(field, n, row, coll) || isThisTileAlreadyShooted(field, row, coll))
		{
			std::cout << "Invalid coordinates or already shot there! Try again: ";
			row = getValidInt();
			coll = getValidInt();
		}

		if (field[row][coll] == SHIP)
		{
			field[row][coll] = HIT;
			if (isAShipSunk(field, row, coll, n))
			{
				std::cout << "BOOM! You just SUNK an enemy ship!";
				Sleep(1500);
			}
			amountOfHittedTiles++;
			if (areAllShipTilesHitted(amountOfHittedTiles)) return;
			printBattleField(field, n);
			std::cout << "YOU HIT! Shoot again." << std::endl;
			Sleep(1500);
			isHit = true;
		}
		else
		{
			field[row][coll] = MISS;
			printBattleField(field, n);
			std::cout << "MISS! Computer's turn." << std::endl;
			Sleep(1500);
			isHit = false;
		}
	}
}

bool canAttakThereForComputer(char field[][MAX_FIELD_LENGHT], int n, int row, int coll)
{
	int middle = n + 1;
	if (row > n || row < 1)
	{
		return false;
	}
	if (coll < middle || coll > MAX_FIELD_LENGHT - 1)
	{
		return false;
	}
	return true;
}

void computerAttack(char field[][MAX_FIELD_LENGHT], int n, int& amountOfHittedTiles)
{
	int middle = n + 1;
	int row, coll;
	bool isHit = true;
	while (isHit)
	{
		row = (rand() % n) + 1;
		coll = (rand() % n) + 1;
		coll += middle;
		while (isThisTileAlreadyShooted(field, row, coll) || !canAttakThereForComputer(field, n, row, coll))
		{
			row = (rand() % n) + 1;
			coll = (rand() % n) + 1;
			coll += middle;
		}
		std::cout << "Computer is thinking...";
		Sleep(2000);
		if (field[row][coll] == SHIP)
		{
			field[row][coll] = HIT;
			amountOfHittedTiles++;
			if (areAllShipTilesHitted(amountOfHittedTiles)) return;
			printBattleField(field, n);
			isHit = true;
			std::cout << "Computer hit yout ship at:" << row << EMPTY_SPACE << coll - middle << '!' << std::endl;
			Sleep(2500);
			if (isAShipSunk(field, row, coll, n))
			{
				std::cout << "BOOM! Computer just SUNK one of your ships!" << std::endl;
				Sleep(1500);
			}
			std::cout << "Computer thinking for his next shot...";
			Sleep(1500);
		}
		else
		{
			field[row][coll] = MISS;
			printBattleField(field, n);
			isHit = false;
			std::cout << "Computer missed your ships at:" << row << EMPTY_SPACE << coll - middle << '!' << std::endl;
			Sleep(2500);
		}
	}
}

void attacking(char field[][MAX_FIELD_LENGHT], int n)
{
	int amountOfHittedTilesPlayer = 0;
	int amountOfHittedTilesComputer = 0;
	while (true)
	{
		playerAttack(field, n, amountOfHittedTilesPlayer);
		if (areAllShipTilesHitted(amountOfHittedTilesPlayer))
		{
			std::cout << "CONGRATULATIONS YOU HAVE SUCCESSFULLY SUNK ALL OF THE ENEMY'S SHIPS" << std::endl << "YOU WIN!!!";
			break;
		}
		computerAttack(field, n, amountOfHittedTilesComputer);
		if (areAllShipTilesHitted(amountOfHittedTilesComputer))
		{
			std::cout << "YOUR OPPONENT HAS SUNK ALL YOUR SHIPS" << std::endl << "YOU LOSE! BETTER LUCK NEXT TIME!";
			break;
		}
	}
}

void startGame()
{
	int n;
	char field[MAX_FIELD_ROWS][MAX_FIELD_LENGHT];
	chooseDifficulty(n);
	createBattleField(field, n);
	automaticPlacing(field, n, false);
	chooseManualOrAutumaticPlacedShips(field, n);
	attacking(field, n);
}

int main()
{
	srand((unsigned int)time(0));
	startGame();
	return 0;
}