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

		huntSpotX = -1;
		huntSpotY = -1;

		huntSpots = 0;
	}

	/*
	//move assignment adapted from https://riptutorial.com/cplusplus/example/6993/move-assignment
	Guesser& operator= (Guesser&& other) {
		this->shipBoard = other.shipBoard;
		this->guessBoard = other.guessBoard;

		this->state = other.state;

		this->huntSpotX = other.huntSpotX;
		this->huntSpotY = other.huntSpotY;

		this->huntSpots = other.huntSpots;

		return *this;
	}
	*/
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

	bool IsValidGuess(int x, int y)
	{
		
		if (x < 0 || x > 7 || y < 0 || y > 7) return false; //if out of bounds of the array

		if (guessBoard.Check(x, y) != GuessBoardValues::Unknown) return false; //if we know the spot

		return true;
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

						result += "Guessed (" + std::to_string(guessX) + "," + std::to_string(guessY) + "), found in " + std::to_string(attemptsToFindUnkown) + " attempts. ";
					}
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

				result += "Result: " + guessBoard.GuessBoardValuesToStringLong(guess);

				break;
			}
			case GuesserState::Hunt:
				result += "State: Hunt. ";

				
				//first find hunting spot if we dont have one on lock
				if (huntSpotX == -1 || huntSpotY == -1)
				{
					//dont have a spot yet
					//go through the guess board and find a near miss unfound
					for (size_t i = 0; i < 8; i++)
					{
						for (size_t j = 0; j < 8; j++)
						{
							GuessBoardValues space = guessBoard.Check(j, i);
							if (space == GuessBoardValues::NearMissUnfound)
							{
								//found a hunting spot
								huntSpotX = j;
								huntSpotY = i;
							}

						}
					}
				}

				result += "Hunting around (" + std::to_string(huntSpotX) + "," + std::to_string(huntSpotY) + "). ";


				//then start checking the 8 surrounded slots
				//if thety are in the array, and haven't been guessed yet

				int guessX = huntSpotX;
				int guessY = huntSpotY;

				bool foundValidSpot = false;

				//loop through adjacent spaces code adapted from https://stackoverflow.com/questions/2035522/get-adjacent-elements-in-a-two-dimensional-array
				for (int dy = -1; dy <= 1; ++dy) {
					for (int dx = -1; dx <= 1; ++dx) {
						if (dx != 0 || dy != 0) { //not the center space
							if (IsValidGuess(huntSpotX + dx, huntSpotY + dy))
							{
								guessX = huntSpotX + dx;
								guessY = huntSpotY + dy;
								foundValidSpot = true;
							}
						}
					}
				}

				if (foundValidSpot == false)
				{
					result += "No valid adjacent spots found.";
					//assume that spot was found at some point
					guessBoard.Write(huntSpotX, huntSpotY, GuessBoardValues::NearMissFound);

					huntSpots -= 1;
					huntSpotX = -1;
					huntSpotY = -1;

					return result;

				}

				result += "Guessed (" + std::to_string(guessX) + "," + std::to_string(guessY) + "). ";

				//then guess 
				GuessBoardValues guess = Guess(guessX, guessY);

				//if its a hit, mark the hunted spot as Near Miss Found, reset hunting spot to -1,-1
				if (guess == GuessBoardValues::Hit)
				{
					guessBoard.Write(huntSpotX, huntSpotY, GuessBoardValues::NearMissFound);

					huntSpots -= 1;
					huntSpotX = -1;
					huntSpotY = -1;
				}
				//if its another near miss, add it to the hunting spots count
				if (guess == GuessBoardValues::NearMissUnfound)
				{
					huntSpots += 1;
				}

				result += "Result: " + guessBoard.GuessBoardValuesToStringLong(guess);
				

				break;
		}

		return result;

	}

};