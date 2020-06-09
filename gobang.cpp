#include <iostream>
#include <string>
#include "gobang.h"
#include <cmath>
#include <time.h>
using namespace std;
bool gobang::gameEnds(state *s)
{
    for (int i = 0; i < boardSize; i++)
    {
        string lineHorizantal = s->board[i];
        string lineVertical = "";
        for (int j = 0; j < boardSize; j++)
        {
            lineVertical += s->board[j][i];
        }
        if (countPattern("bbbbb", lineHorizantal) >= 1 || countPattern("bbbbb", lineVertical) >= 1)
        {
            return true;
        }
        if (countPattern("wwwww", lineHorizantal) >= 1 || countPattern("wwwww", lineVertical) >= 1)
        {
            return true;
        }
    }

    //diagonal
    for (int i = 0; i < boardSize; i++)
    {
        string line = "", line2 = "", line3 = "", line4 = "";
        int diagTmp = i;
        for (int j = 0; j < boardSize; j++)
        {
            if (diagTmp >= boardSize)
                break;
            line += s->board[j][diagTmp];
            line2 += s->board[diagTmp][j];
            line3 += s->board[diagTmp][boardSize - j - 1];
            line4 += s->board[diagTmp - i][boardSize - j - i - 1];
            diagTmp++;
        }
        if (countPattern("bbbbb", line) >= 1 || countPattern("bbbbb", line2) >= 1 || countPattern("bbbbb", line3) >= 1 || countPattern("bbbbb", line4) >= 1)
        {
            return true;
        }
        if (countPattern("aaaaa", line) >= 1 || countPattern("aaaaa", line2) >= 1 || countPattern("aaaaa", line3) >= 1 || countPattern("aaaaa", line4) >= 1)
        {
            return true;
        }
    }
    return false;
}

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

bool gobang::isTie(state *s)
{
    for (int i = 0; i < boardSize; ++i)
    {
        for (int j = 0; j < boardSize; ++j)
        {
            if (currentState.board[i][j] == 'w' || currentState.board[i][j] == 'b')
            {
                return false;
            }
        }
    }
    return true;
}

gobang::gobang(int boardSize, char AIPiece)
{
    //initilazies the boardSize and currentBoard
    this->boardSize = boardSize;
    this->AIPiece = AIPiece;
    depth = 2;
    this->HumanPiece = ('b' == AIPiece) ? 'w' : 'b';
    currentState.board = new char *[boardSize];
    for (int i = 0; i < boardSize; ++i)
    {
        currentState.board[i] = new char[boardSize];
        for (int j = 0; j < boardSize; ++j)
        {
            currentState.board[i][j] = 'X';
        }
    }
}

void gobang::HumanTurn()
{
    string move;
    cin >> move;
    int col = move[0] - 'a';
    int row = stoi(move.substr(1));
    row--;

    if (row >= boardSize || col >= boardSize || row < 0 || currentState.board[row][col] != 'X')
    {
        cout << "Invalid Move" << endl;
        HumanTurn();
    }
    else
    {
        currentState.board[row][col] = HumanPiece;
        cout << "Move Played: " << move << endl;
        return;
    }
}

int gobang::countPattern(string pattern, string line)
{
    size_t found = line.find(pattern);
    int count = 0;
    while (found != string::npos && found < line.size())
    {
        count++;
        found = line.find(pattern, found + pattern.size());
    }
    return count;
}

double gobang::horizantal(state *s, bool isMaximizingPlayer)
{
    double score = 0;
    for (int i = 0; i < boardSize; i++)
    {
        string line = s->board[i];
        if (AIPiece == 'w' && isMaximizingPlayer || AIPiece == 'b' && !isMaximizingPlayer)
        {
            // cout<< "black score"<< endl;
            score += 1000000 * (countPattern("bbbbb", line));
            score += 500000 * (countPattern("XbbbbX", line));                                                          //the straight four                                                                                                                    //the straight four
            score += 50000 * (countPattern("Xbbbbw", line) + countPattern("wbbbbX", line));                            //the four in a row
            score += 5000 * (countPattern("XbbbX", line));                                                             // the three in a row                                                                                                               //the three
            score += 5000 * (countPattern("XbbXbX", line) + countPattern("XbXbbX", line));                             // the broken three                                                                                                            //the broken three
            score += 500 * (countPattern("XXbbXX", line) + countPattern("bbXXX", line) + countPattern("XXXbb", line)); // two in a row
            score += 100 * (countPattern("XXbXX", line) + countPattern("bXXXX", line) + countPattern("XXXXb", line));  //ones                                                                                                                  //the two
        }

        if (AIPiece == 'b' && isMaximizingPlayer || AIPiece == 'w' && !isMaximizingPlayer)
        {
            // cout<< "White score"<< endl;
            score += 1000000 * (countPattern("wwwww", line));                                                          //win
            score += 500000 * (countPattern("XwwwwX", line));                                                          //the straight four
            score += 50000 * (countPattern("Xwwwwb", line) + countPattern("bwwwwX", line));                            //the four in a row
            score += 5000 * (countPattern("XwwwX", line));                                                             //the three in a row
            score += 5000 * (countPattern("XwXwwX", line) + countPattern("XwwXwX", line));                             //the broken three
            score += 500 * (countPattern("XXwwXX", line) + countPattern("wwXXX", line) + countPattern("XXXww", line)); // two in a row
            score += 100 * (countPattern("XXwXX", line) + countPattern("wXXXX", line) + countPattern("XXXXw", line));  //ones
        }
    }
    return score;
}

double gobang::vertical(state *s, bool isMaximizingPlayer)
{
    double score = 0;
    for (int i = 0; i < boardSize; i++)
    {
        string line = "";
        for (int j = 0; j < boardSize; j++)
        {
            line += s->board[j][i];
        }
        if (AIPiece == 'w' && isMaximizingPlayer || AIPiece == 'b' && !isMaximizingPlayer)
        {
            // cout<< "black score"<< endl;
            score += 1000000 * (countPattern("bbbbb", line));
            score += 500000 * (countPattern("XbbbbX", line));                                                          //the straight four                                                                                                                    //the straight four
            score += 50000 * (countPattern("Xbbbbw", line) + countPattern("wbbbbX", line));                            //the four in a row
            score += 5000 * (countPattern("XbbbX", line));                                                             // the three in a row                                                                                                               //the three
            score += 5000 * (countPattern("XbbXbX", line) + countPattern("XbXbbX", line));                             // the broken three                                                                                                            //the broken three
            score += 500 * (countPattern("XXbbXX", line) + countPattern("bbXXX", line) + countPattern("XXXbb", line)); // two in a row
            score += 100 * (countPattern("XXbXX", line) + countPattern("bXXXX", line) + countPattern("XXXXb", line));  //ones                                                                                                                  //the two
        }

        if (AIPiece == 'b' && isMaximizingPlayer || AIPiece == 'w' && !isMaximizingPlayer)
        {
            //cout<< "White score"<< endl;
            score += 1000000 * (countPattern("wwwww", line));                                                          //win
            score += 500000 * (countPattern("XwwwwX", line));                                                          //the straight four
            score += 50000 * (countPattern("Xwwwwb", line) + countPattern("bwwwwX", line));                            //the four in a row
            score += 5000 * (countPattern("XwwwX", line));                                                             //the three in a row
            score += 5000 * (countPattern("XwXwwX", line) + countPattern("XwwXwX", line));                             //the broken three
            score += 500 * (countPattern("XXwwXX", line) + countPattern("wwXXX", line) + countPattern("XXXww", line)); // two in a row
            score += 100 * (countPattern("XXwXX", line) + countPattern("wXXXX", line) + countPattern("XXXXw", line));  //ones
        }
    }
    return score;
}

double gobang::diagonal(state *s, bool isMaximizingPlayer)
{
    double score = 0;

    //right diag
    for (int i = 0; i < boardSize; i++)
    {
        string line1 = "", line2 = "", line3 = "", line4 = "";
        int diagTmp = i;
        for (int j = 0; j < boardSize; j++)
        {
            if (diagTmp >= boardSize)
                break;
            line1 += s->board[j][diagTmp];
            line2 += s->board[diagTmp][j];
            line3 += s->board[diagTmp][boardSize - j - 1];
            line4 += s->board[diagTmp - i][boardSize - j - i - 1];
            diagTmp++;
        }

        if (line1.size() == line2.size() && line1.size() == boardSize)
            line2 = "";
        if (line3.size() == line4.size() && line1.size() == boardSize)
            line4 = "";

        string line = line1 + "-" + line2 + "-" + line3 + "-" + line4;

        if (AIPiece == 'w' && isMaximizingPlayer || AIPiece == 'b' && !isMaximizingPlayer)
        {
            //cout<< "black score"<< endl;
            score += 1000000 * (countPattern("bbbbb", line));
            score += 500000 * (countPattern("XbbbbX", line));                                                          //the straight four                                                                                                                    //the straight four
            score += 50000 * (countPattern("Xbbbbw", line) + countPattern("wbbbbX", line));                            //the four in a row
            score += 5000 * (countPattern("XbbbX", line));                                                             // the three in a row                                                                                                               //the three
            score += 5000 * (countPattern("XbbXbX", line) + countPattern("XbXbbX", line));                             // the broken three                                                                                                            //the broken three
            score += 500 * (countPattern("XXbbXX", line) + countPattern("bbXXX", line) + countPattern("XXXbb", line)); // two in a row
            score += 100 * (countPattern("XXbXX", line) + countPattern("bXXXX", line) + countPattern("XXXXb", line));  //ones                                                                                                                  //the two
        }

        if (AIPiece == 'b' && isMaximizingPlayer || AIPiece == 'w' && !isMaximizingPlayer)
        {
            //cout<< "White score"<< endl;
            score += 1000000 * (countPattern("wwwww", line));                                                          //win
            score += 500000 * (countPattern("XwwwwX", line));                                                          //the straight four
            score += 50000 * (countPattern("Xwwwwb", line) + countPattern("bwwwwX", line));                            //the four in a row
            score += 5000 * (countPattern("XwwwX", line));                                                             //the three in a row
            score += 5000 * (countPattern("XwXwwX", line) + countPattern("XwwXwX", line));                             //the broken three
            score += 500 * (countPattern("XXwwXX", line) + countPattern("wwXXX", line) + countPattern("XXXww", line)); // two in a row
            score += 100 * (countPattern("XXwXX", line) + countPattern("wXXXX", line) + countPattern("XXXXw", line));  //ones
        }
    }
    return score;
}

bool gobang::hasPiecesNearThem(state *s, int row, int col)
{
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            if (row + i < boardSize && row + i >= 0 && col + j < boardSize && col + j >= 0)
            {
                if (s->board[row + i][col + j] != 'X')
                    return true;
            }
        }
    }
    return false;
}

double gobang::evalFunc(state *s, int depth, bool isMaximizingPlayer)
{
    int score = 0;
    score = diagonal(s, isMaximizingPlayer) + vertical(s, isMaximizingPlayer) + horizantal(s, isMaximizingPlayer);
    if (isMaximizingPlayer)
        return -score;
    return score;
}

string gobang::AIOptimalMove(state *s)
{
    double alpha = -1000000000000;
    double beta = 1000000000000;
    double max = -1000000000000;
    string s1 = "";
    char col;
    int row;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (s->board[i][j] == 'X' && hasPiecesNearThem(s, i, j))
            {
                s->board[i][j] = AIPiece;
                double evaluated = minmax(s, 1, false, alpha, beta);
                //cout<< i << " " << j << " Evaluated: " << evaluated << endl;
                if (evaluated > max)
                {
                    col = 'a' + j;
                    row = i + 1;
                    max = evaluated;
                }
                s->board[i][j] = 'X';
            }
        }
    }
    cout << col << " " << row << endl;
    s1 += col;
    s1 += to_string(row);
    return s1;
}

double gobang::minmax(state *s, int depth, bool isMaximizingPlayer, double &alpha, double &beta)
{
    if (isTie(s))
        return 0;
    double pointForCurrentState = evalFunc(s, depth, isMaximizingPlayer);
    if (depth == this->depth)
    {
        return pointForCurrentState;
    }
    if (gameEnds(s))
        return pointForCurrentState;

    if (isMaximizingPlayer)
    {
        double MaxValue = -1000000000000;
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (s->board[i][j] == 'X' && hasPiecesNearThem(s, i, j))
                {
                    s->board[i][j] = AIPiece;
                    MaxValue = max(MaxValue, minmax(s, depth + 1, !isMaximizingPlayer, alpha, beta));
                    s->board[i][j] = 'X';
                    alpha = max(alpha, MaxValue);
                    if (alpha >= beta)
                        break;
                }
            }
        }
        return MaxValue;
    }
    else
    {
        double MinValue = 1000000000000;
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (s->board[i][j] == 'X' && hasPiecesNearThem(s, i, j))
                {
                    s->board[i][j] = HumanPiece;
                    MinValue = min(MinValue, minmax(s, depth + 1, !isMaximizingPlayer, alpha, beta));
                    s->board[i][j] = 'X';
                    beta = min(beta, MinValue);
                    if (alpha >= beta)
                        break;
                }
            }
        }
        return MinValue;
    }
}

void gobang::startGame()
{
    if (AIPiece == 'b')
    {
        string move = "";
        int row;
        int col;
        //random first move for the AI
        srand(time(0));
        row = rand() % boardSize;
        col = rand() % boardSize;

        currentState.board[row][col] = AIPiece;
        char column = 'a' + col;
        char roww = '1' + row;
        move += column;
        move += roww;
        cout << "Move played: " << move << endl;
    }

    while (true)
    {
        HumanTurn();
        //printBoard();
        string move = AIOptimalMove(&currentState);
        int col = move[0] - 'a';
        int row = stoi(move.substr(1));
        row--;
        //cout<< col << " " << row<< endl;
        currentState.board[row][col] = AIPiece;
        cout << "Move played: " << move << endl;
        //printBoard();
    }
}

int main(int argc, char **argv)
{
    int boardSize = 11;
    char AIPiece = 'w';
    for (int i = 1; i < argc; i++)
    {
        //  cout<< argv[i] << " ";
        if (string(argv[i]) == "-l")
            AIPiece = 'b';
        if (string(argv[i]) == "-n")
            boardSize = atoi(argv[i + 1]);
    }
    gobang g(boardSize, AIPiece);
    //cout << boardSize<< endl;
    //cout<< AIPiece<< endl;
    g.startGame();
}