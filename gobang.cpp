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
    depth = 2;
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

    if (row >=boardSize || col >= boardSize || currentState.board[row][col] != 'X')
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
    int score = 1;
    int score2 = 1;
    for(int i = 0; i < boardSize; i++){
        string line = s->board[i];
        score += 100000 * (countPattern("XbbbbX", line)); //the straight four
        score += 50000 * (countPattern("bbbbX", line) + countPattern("bbbXb", line) + countPattern("bbXbb", line) + countPattern("bXbbb", line) + countPattern("Xbbbb", line)); //the four
        score += 5000* (countPattern("XXbbbX", line) +  countPattern("XbbbXX", line)); //the three
        score += 5000 * (countPattern("XbXbbX", line)); //the broken three  

        score2 += 100000 * (countPattern("XwwwwX", line)); //the straight four
        score2 += 50000 * (countPattern("wwwwX", line) + countPattern("wwwXw", line) + countPattern("wwXww", line) + countPattern("wXwww", line) + countPattern("Xwwww", line)); //the four
        score2 += 5000* (countPattern("XXwwwX", line) +  countPattern("XwwwXX", line)); //the three
        score2 += 5000 * (countPattern("XwXwwX", line)); //the broken three
    }
    /*
    if(AIPiece == 'b'&& isAITurn) return 2* score - score2;
    if(AIPiece == 'b'&& !isAITurn) return score - 2*score2;
    if(AIPiece == 'w'&& isAITurn) return 2*score2 - score;
    if(AIPiece == 'w'&& !isAITurn) return score2 - 2*score;
    */
    if(AIPiece == 'b') return score - score2;
    return score2 - score; 
}

int gobang::vertical(state* s, bool isAITurn){
    int score = 1;
    int score2 = 1;
    for(int i = 0; i < boardSize; i++){
        string line = "";
        for(int j = 0; j < boardSize; j++){
             line += s->board[j][i];
        }
        score += 100000 * (countPattern("XbbbbX", line)); //the straight four
        score += 50000 * (countPattern("bbbbX", line) + countPattern("bbbXb", line) + countPattern("bbXbb", line) + countPattern("bXbbb", line) + countPattern("Xbbbb", line)); //the four
        score += 5000* (countPattern("XXbbbX", line) +  countPattern("XbbbXX", line)); //the three
        score += 5000 * countPattern("XbXbbX", line); //the broken three  

        score2 += 100000 * (countPattern("XwwwwX", line)); //the straight four
        score2 += 50000 * (countPattern("wwwwX", line) + countPattern("wwwXw", line) + countPattern("wwXww", line) + countPattern("wXwww", line) + countPattern("Xwwww", line)); //the four
        score2 += 5000* (countPattern("XXwwwX", line) +  countPattern("XwwwXX", line)); //the three
        score2 += 5000 * countPattern("XwXwwX", line); //the wroken three  

    }
    if(AIPiece == 'b') return score - score2;
    return score2 - score; 
}

int gobang::diagonal(state* s, bool isAITurn){
    int score = 1;
    int score2 = 1;
    //right diag
    for(int i = 0; i < boardSize; i++){
        string line = "", line2 = "", line3 = "", line4 = "";
        int diagTmp = i;
        for(int j = 0; j < boardSize; j++){
            if (diagTmp >= boardSize) break;
            line += s->board[j][diagTmp];
            line2 += s->board[diagTmp][j];
            
            //cout<< j<< " " << diagTmp<< endl;
            line3 += s->board[diagTmp][boardSize-j-1];
            line4 += s->board[diagTmp-i][boardSize-j-i-1];
            diagTmp++;
        }

        if(line.size() == line2.size() && line.size() == boardSize) line2 = "";
        if(line3.size() == line4.size() && line.size() == boardSize) line4 = "";
        if(line.size() < 5) continue;

    

        score += 100000 * (countPattern("XbbbbX", line) + countPattern("XbbbbX", line2)
                            +countPattern("XbbbbX", line3) + countPattern("XbbbbX", line4)); //the straight four
        score += 50000 * (countPattern("bbbbX", line) + countPattern("bbbXb", line) + countPattern("bbXbb", line) + countPattern("bXbbb", line) + countPattern("Xbbbb", line)
                        + countPattern("bbbbX", line2) + countPattern("bbbXb", line2) + countPattern("bbXbb", line2) + countPattern("bXbbb", line2) + countPattern("Xbbbb", line2)
                        + countPattern("bbbbX", line3) + countPattern("bbbXb", line3) + countPattern("bbXbb", line3) + countPattern("bXbbb", line3) + countPattern("Xbbbb", line3)
                        + countPattern("bbbbX", line4) + countPattern("bbbXb", line4) + countPattern("bbXbb", line4) + countPattern("bXbbb", line4) + countPattern("Xbbbb", line4)); //the four
         score += 5000* (countPattern("XXbbbX", line) +  countPattern("XbbbXX", line) + 
                         countPattern("XXbbbX", line2) +  countPattern("XbbbXX", line2) +
                         countPattern("XXbbbX", line3) +  countPattern("XbbbXX", line3)+ 
                         countPattern("XXbbbX", line4) +  countPattern("XbbbXX", line4)); //the three
        score += 5000 * (countPattern("XbXbbX", line) + countPattern("XbXbbX", line2)
                         +countPattern("XbXbbX", line3) + countPattern("XbXbbX", line4)); //the broken three  
       

        score2 += 100000 * (countPattern("XwwwwX", line) + countPattern("XwwwwX", line2)
                            +countPattern("XwwwwX", line3) + countPattern("XwwwwX", line4)); //the straight four
        score2 += 50000 * (countPattern("wwwwX", line) + countPattern("wwwXw", line) + countPattern("wwXww", line) + countPattern("wXwww", line) + countPattern("Xwwww", line)
                        + countPattern("wwwwX", line2) + countPattern("wwwXw", line2) + countPattern("wwXww", line2) + countPattern("wXwww", line2) + countPattern("Xwwww", line2)
                        + countPattern("wwwwX", line3) + countPattern("wwwXw", line3) + countPattern("wwXww", line3) + countPattern("wXwww", line3) + countPattern("Xwwww", line3)
                        + countPattern("wwwwX", line4) + countPattern("wwwXw", line4) + countPattern("wwXww", line4) + countPattern("wXwww", line4) + countPattern("Xwwww", line4)); //the four
        score2 += 5000* (countPattern("XXwwwX", line) +  countPattern("XwwwXX", line) + 
                         countPattern("XXwwwX", line2) +  countPattern("XwwwXX", line2) +
                         countPattern("XXwwwX", line3) +  countPattern("XwwwXX", line3)+ 
                         countPattern("XXwwwX", line4) +  countPattern("XwwwXX", line4)); //the three
        score2 += 5000 * (countPattern("XwXwwX", line) + countPattern("XwXwwX", line2)
                         +countPattern("XwXwwX", line3) + countPattern("XwXwwX", line4)); //the wroken three  


    }
    if(AIPiece == 'b') return score - score2;
    return score2 - score; 
}

int gobang::evalFunc(state* s, int depth, bool isAIturn){
    char** tmp = s->board;
    int score = 0;
    if(gameEnds(s)){
        return 100000000;
    }
    score = diagonal(s, isAIturn) + vertical(s, isAIturn) + horizantal(s, isAIturn);
    score = score - depth;
    if(isAIturn) return score;
    return score * -1;
}

string gobang::AIOptimalMove(state* s){
    int MaxValue = -100000000;
    string s1 = "";
    char col;
    int row;
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            if(s->board[i][j] == 'X'){
                s->board[i][j] = AIPiece;
                int tmp = minmax(s, 1,false);
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

int gobang::minmax(state* s, int depth, bool isAIturn){
    if (isTie(s)) return 0;
    int pointForCurrentState = evalFunc(s, depth, isAIturn);
    //cout<< "Eval for current state: " << pointForCurrentState<<endl;
    //printBoard();
    if (depth == this->depth)
        {
        //    cout << "Exited bitch " <<endl;
            return pointForCurrentState;
        }
    if (abs(pointForCurrentState) == 100000000) return pointForCurrentState;
    if(isAIturn){
        int MaxValue = -100000000;
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++){
                if(s->board[i][j] == 'X'){
                    s->board[i][j] = AIPiece;
                    MaxValue = max(MaxValue, minmax(s, depth+1, !isAIturn));
                    s->board[i][j] = 'X';
                }
            }
        }
        return MaxValue;
    } else{
        int MaxValue = 100000000;
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++){
                if(s->board[i][j] == 'X'){
                    s->board[i][j] = HumanPiece;
                    MaxValue = min(MaxValue, minmax(s,depth +1, !isAIturn));
                    s->board[i][j] = 'X';
                }
            }
        }
        return MaxValue;
    }
}




void gobang::startGame()
{
    if(AIPiece == 'b'){
        string move = AIOptimalMove(&currentState);
        int col = move[0] - 'a';
        int row = stoi(move.substr(1));
        row--;
       // cout<< col << " " << row<< endl;
        currentState.board[row][col] = AIPiece;
        cout<< "Move played: " << move << endl;
      //  printBoard();
    }

    while(true){
        HumanTurn();
      //  printBoard();
        string move = AIOptimalMove(&currentState);
        int col = move[0] - 'a';
        int row = stoi(move.substr(1));
        row--;
        //cout<< col << " " << row<< endl;
        currentState.board[row][col] = AIPiece;
        cout<< "Move played: " << move << endl;
      //  printBoard();
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