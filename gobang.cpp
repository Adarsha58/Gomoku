#include <iostream>
#include <string>
#include "gobang.h"
#include <cmath>

using namespace std;
bool gobang::gameEnds(state* s){
    for(int i = 0; i < boardSize; i++){
        string lineHorizantal = s->board[i];
        string lineVertical = "";
        for(int j = 0; j < boardSize; j++){
             lineVertical += s->board[j][i];
        }
        if(countPattern("bbbbb", lineHorizantal) >= 1 || countPattern("bbbbb", lineVertical) >= 1){
            return true;
        }
        if(countPattern("wwwww", lineHorizantal) >= 1 || countPattern("wwwww", lineVertical) >= 1){
            return true;
        }
    }

    //diagonal
    for(int i = 0; i < boardSize; i++){
        string line = "", line2 = "", line3 = "", line4 = "";
        int diagTmp = i;
        for(int j = 0; j < boardSize; j++){
            if (diagTmp >= boardSize) break;
            line += s->board[j][diagTmp];
            line2 += s->board[diagTmp][j];
            line3 += s->board[diagTmp][boardSize-j-1];
            line4 += s->board[diagTmp-i][boardSize-j-i-1];
            diagTmp++;
        }
        if(countPattern("bbbbb", line) >= 1 || countPattern("bbbbb", line2) >= 1 || countPattern("bbbbb", line3) >= 1 || countPattern("bbbbb", line4) >= 1){
            return true;
        }
        if(countPattern("aaaaa", line) >= 1 || countPattern("aaaaa", line2) >= 1 || countPattern("aaaaa", line3) >= 1 || countPattern("aaaaa", line4) >= 1){
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

bool gobang::isTie(state* s){
    for (int i = 0; i < boardSize; ++i)
    { 
        for (int j = 0; j < boardSize; ++j)
        { 
           if(currentState.board[i][j] == 'w' || currentState.board[i][j] == 'b'){
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
    depth = 3;
    this->HumanPiece = ('b' == AIPiece) ? 'w':'b';
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
    if (row >=boardSize || col >= boardSize || row <0 || currentState.board[row][col] != 'X' )
    {
        cout << "Invalid Move"<<endl;
        HumanTurn();
    }
    else
    {
        currentState.board[row][col] = HumanPiece;
        cout<< "Move played: " << move<< endl;
        return;
    }
}

int gobang::countPattern(string pattern, string line){
    size_t found = line.find(pattern);
    int count = 0;
    while(found!= string::npos && found < line.size()){
        count ++;
        found = line.find(pattern, found + pattern.size());
    }
    return count;
}

int gobang::horizantal(state* s, bool isAITurn){
    int score = 0;
    int score2 = 0;

    for(int i = 0; i < boardSize; i++){
        string line = s->board[i];
        score += 1000000 * (countPattern("XbbbbX", line)); //the straight four
        score += 50000 * (countPattern("bbbbX", line) + countPattern("bbbXb", line) + countPattern("bbXbb", line) + countPattern("bXbbb", line) + countPattern("Xbbbb", line)); //the four
        score += 20000* (countPattern("XXbbbX", line) +  countPattern("XbbbXX", line) + countPattern("XbXbbX", line)); //the dangerous three
        score += 500* (countPattern("bbbX", line) +  countPattern("Xbbb", line) + countPattern("bbXb", line) + countPattern("bxbb", line));//normal three
        score += 500 * (countPattern("bbX", line) + countPattern("Xbb", line) + countPattern("bXb", line)); //the two
        score += 100 * countPattern("XbX", line); //the one

        score2 += 1000000 * (countPattern("XwwwwX", line)); //the straight four
        score2 += 50000 * (countPattern("wwwwX", line) + countPattern("wwwXw", line) + countPattern("wwXww", line) + countPattern("wXwww", line) + countPattern("Xwwww", line)); //the four
        score2 += 20000* (countPattern("XXwwwX", line) +  countPattern("XwwwXX", line) + countPattern("XwXwwX", line)); //the dangerous three
        score2 += 500* (countPattern("wwwX", line) +  countPattern("Xwww", line) + countPattern("wwXw", line) + countPattern("wxww", line));//normal three
        score2 += 500 * (countPattern("wwX", line) + countPattern("Xww", line) + countPattern("wXw", line)); //the two
        score2 += 100 * countPattern("XwX", line); //the one
    }
    
    if(AIPiece == 'b') return score - score2;
    return score2 - score; 
}

int gobang::vertical(state* s, bool isAITurn){
    int score = 0;
    int score2 = 0;
    for(int i = 0; i < boardSize; i++){
        string line = "";
        for(int j = 0; j < boardSize; j++){
             line += s->board[j][i];
        }
        score += 1000000 * (countPattern("XbbbbX", line)); //the straight four
        score += 50000 * (countPattern("bbbbX", line) + countPattern("bbbXb", line) + countPattern("bbXbb", line) + countPattern("bXbbb", line) + countPattern("Xbbbb", line)); //the four
        score += 20000* (countPattern("XXbbbX", line) +  countPattern("XbbbXX", line) + countPattern("XbXbbX", line)); //the dangerous three
        score += 500* (countPattern("bbbX", line) +  countPattern("Xbbb", line) + countPattern("bbXb", line) + countPattern("bxbb", line));//normal three
        score += 500 * (countPattern("bbX", line) + countPattern("Xbb", line) + countPattern("bXb", line)); //the two
        score += 100 * countPattern("XbX", line); //the one

        score2 += 1000000 * (countPattern("XwwwwX", line)); //the straight four
        score2 += 50000 * (countPattern("wwwwX", line) + countPattern("wwwXw", line) + countPattern("wwXww", line) + countPattern("wXwww", line) + countPattern("Xwwww", line)); //the four
        score2 += 20000* (countPattern("XXwwwX", line) +  countPattern("XwwwXX", line) + countPattern("XwXwwX", line)); //the dangerous three
        score2 += 500* (countPattern("wwwX", line) +  countPattern("Xwww", line) + countPattern("wwXw", line) + countPattern("wxww", line));//normal three
        score2 += 500 * (countPattern("wwX", line) + countPattern("Xww", line) + countPattern("wXw", line)); //the two
        score2 += 100 * countPattern("XwX", line); //the one

    }

    if(AIPiece == 'b') return score - score2;
    return score2 - score;
    
}

int gobang::diagonal(state* s, bool isAITurn){
    int score = 0;
    int score2 = 0;
    //right diag
    for(int i = 0; i < boardSize; i++){
        string line1 = "", line2 = "", line3 = "", line4 = "";
        int diagTmp = i;
        for(int j = 0; j < boardSize; j++){
            if (diagTmp >= boardSize) break;
            line1 += s->board[j][diagTmp];
            line2 += s->board[diagTmp][j];
            
            //cout<< j<< " " << diagTmp<< endl;
            line3 += s->board[diagTmp][boardSize-j-1];
            line4 += s->board[diagTmp-i][boardSize-j-i-1];
            diagTmp++;
        }

        if(line1.size() == line2.size() && line1.size() == boardSize) line2 = "";
        if(line3.size() == line4.size() && line1.size() == boardSize) line4 = "";
        if(line1.size() < 3) continue;

        string line = line1 + "-" + line2 + "-" + line3 + "-" + line4;
        score += 1000000 * (countPattern("XbbbbX", line)); //the straight four
        score += 50000 * (countPattern("bbbbX", line) + countPattern("bbbXb", line) + countPattern("bbXbb", line) + countPattern("bXbbb", line) + countPattern("Xbbbb", line)); //the four
        score += 20000* (countPattern("XXbbbX", line) +  countPattern("XbbbXX", line) + countPattern("XbXbbX", line)); //the dangerous three
        score += 500* (countPattern("bbbX", line) +  countPattern("Xbbb", line) + countPattern("bbXb", line) + countPattern("bXbb", line));//normal three
        score += 500 * (countPattern("bbX", line) + countPattern("Xbb", line) + countPattern("bXb", line)); //the two
        score += 100 * countPattern("XbX", line); //the one

        score2 += 1000000 * (countPattern("XwwwwX", line)); //the straight four
        score2 += 50000 * (countPattern("wwwwX", line) + countPattern("wwwXw", line) + countPattern("wwXww", line) + countPattern("wXwww", line) + countPattern("Xwwww", line)); //the four
        score2 += 20000* (countPattern("XXwwwX", line) +  countPattern("XwwwXX", line) + countPattern("XwXwwX", line)); //the dangerous three
        score2 += 500* (countPattern("wwwX", line) +  countPattern("Xwww", line) + countPattern("wwXw", line) + countPattern("wXww", line));//normal three
        score2 += 500 * (countPattern("wwX", line) + countPattern("Xww", line) + countPattern("wXw", line)); //the two
      score2 += 100 * countPattern("XwX", line); //the one
    }
    if(AIPiece == 'b') return score - score2;
    return score2 - score;
}

int gobang::evalFunc(state* s, int depth, bool isAIturn){
    char** tmp = s->board;
    int score = 0;
    if(gameEnds(s)){
        return 1000000000;
    }
    score = diagonal(s, isAIturn) + vertical(s, isAIturn) + horizantal(s, isAIturn);
    score = score - depth;
    return score;
}

string gobang::AIOptimalMove(state* s){
    int MaxValue = -1000000000;
    string s1 = "";
    char col;
    int row;
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            if(s->board[i][j] == 'X'){
                s->board[i][j] = AIPiece;
                int tmp = minmax(s, 1,false, MaxValue, -MaxValue);
               //cout << i << " "<< j << " :Evaluation: " << tmp << endl;
                if(tmp >= MaxValue){
                    col = 'a' + j;
                    row = i+1;
                    MaxValue = tmp;
                }
                s->board[i][j] = 'X';
            }
        }
    }
    s1 += col;
    s1 += to_string(row);
    return s1;
}

int gobang::minmax(state* s, int depth, bool isAIturn, int alpha, int beta){
    if (isTie(s)) return 0;
    int pointForCurrentState = evalFunc(s, depth, isAIturn);
    if (depth == this->depth)
        {
        //    cout << "Exited bitch " <<endl;
            return pointForCurrentState;
        }
    if (abs(pointForCurrentState) == 1000000000) return pointForCurrentState;
    if(isAIturn){
        int MaxValue = -1000000000;
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++){
                if(s->board[i][j] == 'X'
                ){
                    s->board[i][j] = AIPiece;
                    MaxValue = max(MaxValue, minmax(s, depth+1, !isAIturn, alpha, beta));
                    s->board[i][j] = 'X';
                    alpha = max(alpha, MaxValue);
                    if (alpha >= beta) break;
                }
            }
        }
        return MaxValue;
    } else{
        int MaxValue = 1000000000;
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++){
                if(s->board[i][j] == 'X' ){
                    s->board[i][j] = HumanPiece;
                    MaxValue = min(MaxValue, minmax(s,depth +1, !isAIturn, alpha , beta));
                    beta = min(beta, MaxValue);
                    s->board[i][j] = 'X';
                    if(alpha >= beta) break;
                }
            }
        }
        return MaxValue;
    }
}




void gobang::startGame()
{
    if(AIPiece == 'b'){
        string move = "e5";
        int col = move[0] - 'a';
        int row = stoi(move.substr(1));
        row--;
        currentState.board[row][col] = AIPiece;
        cout<< "Move played: " << move << endl;
        printBoard();
    }

    while(true){
        HumanTurn();
        printBoard();
        string move = AIOptimalMove(&currentState);
        int col = move[0] - 'a';
        int row = stoi(move.substr(1));
        row--;
        currentState.board[row][col] = AIPiece;
        printBoard();
        cout<< "Move played: " << move << endl;
    }
}






int main(int argc, char **argv)
{
    int boardSize = 11;
    char AIPiece = 'w';
    for(int i = 1; i < argc; i++){
      //  cout<< argv[i] << " ";
        if(string(argv[i]) == "-l") AIPiece = 'b';
        if(string(argv[i]) == "-n") boardSize = atoi(argv[i+1]);
    }
    gobang g(boardSize, AIPiece);
    //cout << boardSize<< endl;
    //cout<< AIPiece<< endl;
    g.startGame();
}