#include <iostream>

const char WATER = '~';
const char HIT = 'X';
const char MISS = 'O';
const char SHIP = '#';
const char SEPARATION = '|';
const char EMPTY_SPACE = ' ';
const int MAX_FIELD_ROWS = 16;
const int MAX_FIELD_LENGHT = 32;
const int AMOUNT_OF_SHIPS = 10;

void clearConsole()
{
	system("cls");
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

	for (int i = 1; i <= n; ++i)
	{
		if (i < 10) std::cout << "0" << i << " ";
		else std::cout << i << " ";

		for (int j = 1; j <= (n * 2 + 1); ++j)
		{
			std::cout << field[i][j] << " ";
		}

		if (i < 10) std::cout << "0" << i;
		else std::cout << i;

		std::cout << std::endl;
	}
}

bool isItValidPlaceToPutShipWiyhLenghtOfK(char field[][MAX_FIELD_LENGHT], int row, int coll, char direction, int k,int n)
{
	int middle = n + 1;
	if (direction == 'h')
	{
		if (coll + k > n+1)
			return false;
		for (int i = 0; i < k; ++i)
		{
			if (field[row][coll + i+middle] != WATER)
				return false;
		}
		return true;
	}
	else if (direction == 'v')
	{
		if (row + k > n+1)
			return false;
		for (int i = 0; i < k; ++i)
		{
			if (field[row + i][coll+middle] != WATER)
				return false;
		}
		return true;
	}
	else return false;
}

bool areValidNumbersForRowsAndColls(int row, int coll)
{
	if (row < 1 || row>15 || coll < 1 || coll>15)
		return false;
	return true;
}

void manulalPlacingOfShips(char field[][MAX_FIELD_LENGHT],int size)
{
	int middle = size + 1;
	int sizes[AMOUNT_OF_SHIPS] = {4,3,3,2,2,2,1,1,1,1};
	for (int i = 0; i < AMOUNT_OF_SHIPS; ++i)
	{
		int row, coll;
		char direction;
		bool allShipsPlaced = false;
		while (!allShipsPlaced)
		{
			std::cout << "Place your next ship which is " << sizes[i] << " tiles long.(ships left to place:"<<10-i<<")"<<std::endl<<"To place your ship first input the starting coordinats and after that choose it to be horizontal(h) or vertical(h)."<<std::endl<<"The game will place the rest of your ship. Input your coordinats:";
			std::cin >> row >> coll;
			while (!areValidNumbersForRowsAndColls(row, coll))
			{
				std::cout << "Invalid coordinats please input new ones:";
				std::cin >> row >> coll;
			}
			if (sizes[i] != 1)
			{
				std::cout << "Choosee horizontal or vertica(h or v):"<<std::endl;
				std::cin >> direction;
				while (direction != 'h' && direction != 'v')
				{
					std::cout << "Invalid direction please input new one:";
					std::cin >> direction;
				}
				while (!isItValidPlaceToPutShipWiyhLenghtOfK(field, row, coll, direction, sizes[i],size))
				{
					std::cout << "You can not place ship of that lenght there!" << std::endl;
					std::cout << "Please enter new values for row coll:";
					std::cin >> row >> coll;
					while (!areValidNumbersForRowsAndColls(row, coll))
					{
						std::cout << "Invalid coordinats please input new ones:";
						std::cin >> row >> coll;
					}
					std::cout << "Please enter new dirction:";
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
			{
				field[row][coll + middle] = SHIP;
			}
			printBattleField(field,size);
			allShipsPlaced = true;
		}
	}
}

int main()
{
	int n;
	std::cin >> n;
	char field[MAX_FIELD_ROWS][MAX_FIELD_LENGHT];

	createBattleField(field,n);
	printBattleField(field,n);
	manulalPlacingOfShips(field, n);

	return 0;
}