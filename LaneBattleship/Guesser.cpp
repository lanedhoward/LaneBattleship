#include "ShipBoard.cpp"
#include "GuessBoard.cpp"

#pragma once

enum class GuesserState
{
	Search,
	Hunt
};

class Guesser
{
public:
	ShipBoard& shipBoard;
	GuessBoard& guessBoard;

	GuesserState state;

	int huntSpotX = -1;
	int huntSpotY = -1;

	int huntSpots = 0; // number of near misses (unfound) we know about
	
	//https://stackoverflow.com/questions/1711990/what-is-this-weird-colon-member-syntax-in-the-constructor
	Guesser(ShipBoard & _shipBoard, GuessBoard & _guessBoard) : shipBoard(_shipBoard), guessBoard(_guessBoard)
	{
		state = GuesserState::Search;
	}

	GuessBoardValues Guess(int x, int y)
	{
		//check the ship board- if it is a ship, return ship
		ShipBoardValues guessedSpot = shipBoard.Check(x,y);

		if (guessedSpot == ShipBoardValues::Ship)
		{
			shipBoard.Write(x, y, ShipBoardValues::Destroyed);
			guessBoard.Write(x, y, GuessBoardValues::Hit);
			return GuessBoardValues::Hit;
		}

		//if it is not a ship, check around it to see if it is a near miss
		bool foundNearShip = false;
		if (shipBoard.Check(x + 1, y) == ShipBoardValues::Ship) foundNearShip = true;
		if (shipBoard.Check(x + 1, y + 1) == ShipBoardValues::Ship) foundNearShip = true;
		if (shipBoard.Check(x, y + 1) == ShipBoardValues::Ship) foundNearShip = true;
		if (shipBoard.Check(x - 1, y + 1) == ShipBoardValues::Ship) foundNearShip = true;
		if (shipBoard.Check(x - 1, y) == ShipBoardValues::Ship) foundNearShip = true;
		if (shipBoard.Check(x - 1, y - 1) == ShipBoardValues::Ship) foundNearShip = true;
		if (shipBoard.Check(x, y - 1) == ShipBoardValues::Ship) foundNearShip = true;
		if (shipBoard.Check(x + 1, y - 1) == ShipBoardValues::Ship) foundNearShip = true;

		if (foundNearShip)
		{
			shipBoard.Write(x, y, ShipBoardValues::Guessed);
			guessBoard.Write(x, y, GuessBoardValues::NearMissUnfound);
			return GuessBoardValues::NearMissUnfound;
		}

		//if its not a ship and not a near miss, return miss
		shipBoard.Write(x, y, ShipBoardValues::Guessed);
		guessBoard.Write(x, y, GuessBoardValues::Miss);
		return GuessBoardValues::Miss;
	}

	string Step()
	{
		//step will be 1 turn, 1 pass thru the ai, etc.
		//step is what gets looped to play the game
		//will return a string of what it did.

		string result = "";

		//first determine what state we should be in
		if (huntSpots > 0)
		{
			state = GuesserState::Hunt;
		}
		else
		{
			state = GuesserState::Search;
		}

		//do state logic
		//state machine
		switch (state)
		{
			case GuesserState::Search:
			{
				result += "State: Search. ";

				//find a random unknown square
				bool foundUnknown = false;

				int guessX = 0;
				int guessY = 0;

				int attemptsToFindUnkown = 0;

				while (!foundUnknown)
				{
					attemptsToFindUnkown += 1;
					int rx = rand() % 8;
					int ry = rand() % 8;

					if (guessBoard.Check(rx, ry) == GuessBoardValues::Unknown)
					{

						guessX = rx;
						guessY = ry;
						foundUnknown = true;

						result += "Guessed (" + std::to_string(guessX) + "," + std::to_string(guessY) + "), found in " + std::to_string(attemptsToFindUnkown) + "attempts. ";
					}

					//actually guess 
					GuessBoardValues guess = Guess(guessX, guessY);

					//if its a near miss, save that
					if (guess == GuessBoardValues::NearMissUnfound)
					{
						huntSpots += 1;
						if (huntSpotX == -1 && huntSpotY == -1)
						{
							huntSpotX = guessX;
							huntSpotY = guessY;
						}
					}

					result += "Result: " + guessBoard.GuessBoardValuesToString(guess);

				}


				break;
			}
			case GuesserState::Hunt:
				break;
		}

		return result;

	}

};