// LaneBattleship.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "ShipBoard.cpp"
#include "GuessBoard.cpp"
#include "ConsoleUtils.h"
#include "Guesser.cpp"

using namespace ConsoleUtils;

#pragma once

ShipBoard* ptShipBoard;
ShipBoard shipBoard;

GuessBoard* ptGuessBoard;
GuessBoard guessBoard;

Guesser* ptGary;
//Guesser gary;

void DisplayBoards();

void GameStep(Guesser& gary);

void EndGame();

int main()
{
    
    ptShipBoard = new ShipBoard();
    shipBoard = *ptShipBoard;

    ptGuessBoard = new GuessBoard();
    guessBoard = *ptGuessBoard;

    ptGary = new Guesser(shipBoard, guessBoard);
    //gary = *ptGary;

    Print("Welcome to Lane's Battleship Game");

    DisplayBoards();

    WaitForKeyPress();

    for (size_t i = 0; i < 100; i++) // 100 loop length is arbritary, but thats enough to go through all spaces and even have wasted turns
    {
        Print("Turn: " + std::to_string(i + 1));
        if (shipBoard.GetNumberOfShips() > 0) //if there is still ships to be found
        {
            GameStep(*ptGary);
        }
        else
        {
            Print("All ships found!");
            break;
        }
    }
    //display final game boards;
    DisplayBoards();
    
    EndGame();
}

void DisplayBoards()
{
    Print("Ship Board: ");

    Print(shipBoard.ShipDisplay());

    Print("Ships: " + to_string(shipBoard.GetNumberOfShips()));
    Print();

    Print("Guess Board: ");

    Print(guessBoard.GuessDisplay());
}

void GameStep(Guesser& gary)
{
    
    Print(gary.Step());

    //WaitForKeyPress();

    DisplayBoards();

    WaitForKeyPress();
}

void EndGame()
{
    delete ptShipBoard;
    delete ptGuessBoard;
    delete ptGary;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
