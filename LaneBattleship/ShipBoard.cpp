#include <array>
#include <string>
#include <ctime>

using namespace std;

#pragma once

enum class ShipBoardValues
{
	Water,
	Ship,
	Destroyed,
	Guessed
};

class ShipBoard
{
	private:
		//int m_boardWidth;
		//int m_boardHeight;
		ShipBoardValues Board[8][8];

		int m_totalShips = 0;

		void ClearBoard()
		{
			// find length of 2d array code adapted from https://stackoverflow.com/questions/10274162/how-to-find-2d-array-size-in-c
			//m_boardHeight = sizeof(Board) / sizeof(Board[0]);
			//m_boardWidth = sizeof(Board[0]) / sizeof(ShipBoardValues);

			//initialize board
			for (size_t i = 0; i < 8; i++)
			{
				for (size_t j = 0; j < 8; j++)
				{
					Board[j][i] = ShipBoardValues::Water;
				}
			}
		}

		void PlaceShips()
		{
			ClearBoard();
			//randomly place ships
			m_totalShips = 0;

			//loop through each space
			for (size_t i = 0; i < 8; i++)
			{
				for (size_t j = 0; j < 8; j++)
				{

					int r = rand() % 10;
					
					if (r < 2) // ~20% chance of a ship?
					{
						Board[j][i] = ShipBoardValues::Ship;
						m_totalShips += 1;
					}
				}
			}
		}

	public:
		ShipBoard()
		{
			//random number seeding from https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
			srand((unsigned)time(0));

			PlaceShips();
			
			

		}

		std::string ShipDisplay() 
		{
			std::string result = "";
			
			for (size_t i = 0; i < 8; i++)
			{
				for (size_t j = 0; j < 8; j++)
				{
					switch (Board[j][i])
					{
					case ShipBoardValues::Water:
						result += "~";
						break;
					case ShipBoardValues::Ship:
						result += "S";
						break;
					case ShipBoardValues::Destroyed:
						result += "*";
						break;
					case ShipBoardValues::Guessed:
						result += "/";
						break;
					}
				}
				result += "\n";
			}

			return result;
		}

		int GetNumberOfShips()
		{
			return m_totalShips;
		}

		void Write(int x, int y, ShipBoardValues val)
		{
			if (val == ShipBoardValues::Destroyed) m_totalShips -= 1;

			Board[x][y] = val;
		}

		ShipBoardValues Check(int x, int y)
		{
			return Board[x][y];
		}

};

