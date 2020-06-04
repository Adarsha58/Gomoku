#include <iostream>
#include <string>
#include "gobang.h"

using namespace std;
void gobang::printBoard()
{
    cout << "------------" << endl;
    for (int i = 0; i < boardSize; ++i)
    { // for each row
        for (int j = 0; j < boardSize; ++j)
        { // for each column
            cout << currentState.board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "------------" << endl;
}

gobang::gobang(int boardSize)
{
    //initilazies the boardSize and currentBoard
    this->boardSize = boardSize;
    currentState.board = new char *[boardSize];
    for (int i = 0; i < boardSize; ++i)
    {
        currentState.board[i] = new char[boardSize];
        for (int j = 0; j < boardSize; ++j)
        {
            currentState.board[i][j] = 'X';
        }
    }
    printBoard();
}

void gobang::HumanTurn(char piece)
{
    string move;
    cin >> move;
    int col = move[0] - 'a';
    int row = move[1] - '0';

    if (currentState.board[row][col] != 'X')
    {
        cout << "Invalid Move";
        HumanTurn(piece);
    }
    else
    {
        currentState.board[row][col] = piece;
        return;
    }
}

void gobang::startGame(char color)
{
    //dark indicates human player move first
    if (color == 'd')
    {
        HumanTurn(color);
        printBoard();
    }
}

int main(int argc, char **argv)
{
    if (argc > 3)
    {
        cerr << "Invalid Input" << endl;
    }
    if (argc == 3)
    {
        int boardSize = atoi(argv[1]);
        gobang game(boardSize);
        game.startGame('l');
    }
    else if (argc == 2)
    {
        if (argv[1] == "l")
        {
            gobang game;
            game.startGame('l');
        }
        else
        {
            int boardSize = atoi(argv[1]);
            gobang game(boardSize);
            game.startGame('d');
        }
    }
}