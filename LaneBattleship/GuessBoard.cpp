#include <array>
#include <string>

using namespace std;

#pragma once


enum class GuessBoardValues
{
	Unknown,
	Miss,
	Hit,
	NearMissUnfound,
	NearMissFound
};


class GuessBoard
{
private:
	//int m_boardWidth;
	//int m_boardHeight;
	GuessBoardValues Board[8][8];

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
				Board[j][i] = GuessBoardValues::Unknown;
			}
		}
	}

	

public:
	GuessBoard()
	{
		
		ClearBoard();


	}

	std::string GuessDisplay()
	{
		std::string result = "";

		for (size_t i = 0; i < 8; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				result += GuessBoardValuesToString(Board[j][i]);
				
			}
			result += "\n";
		}

		return result;
	}

	string GuessBoardValuesToString(GuessBoardValues val)
	{
		switch (val)
		{
		case GuessBoardValues::Unknown:
			return "~";
		case GuessBoardValues::Miss:
			return "O";
		case GuessBoardValues::Hit:
			return "X";
		case GuessBoardValues::NearMissFound:
			return "*";
		case GuessBoardValues::NearMissUnfound:
			return "#";
		default:
			return "~";
		}
	}

	void Write(int x, int y, GuessBoardValues val)
	{
		Board[x][y] = val;
	}

	GuessBoardValues Check(int x, int y)
	{
		return Board[x][y];
	}

};

