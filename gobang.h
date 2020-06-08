struct state
{
        char **board;
};

class gobang
{
private:
    state currentState;
    int boardSize;
    char AIPiece;
    char HumanPiece;
    int depth;
    void HumanTurn();
    int horizantal(state*,bool);
    int vertical(state*, bool);
    int diagonal(state*, bool);
    int countPattern(std::string, std::string);
    bool hasAdjacentPiece(int row, int col){
        if(col+ 1 < boardSize && currentState.board[row][col+1] != 'X') return true;
        if(row + 1 < boardSize && currentState.board[row+1][col] != 'X') return true;
        if(col- 1 >=0 && currentState.board[row][col-1] != 'X') return true;
        if(row -1 >=0 && currentState.board[row-1][col] != 'X') return true;
        if(col- 1 >=0 && row - 1 >=0 && currentState.board[row-1][col-1] != 'X') return true;
        if(col+ 1 < boardSize && row+1 < boardSize && currentState.board[row+1][col+1] != 'X') return true;
        return false;
    };
    
public:
    gobang(int boardSize, char AIPiece);
    bool isTie(state*);
    int evalFunc(state*, int, bool);
    int minmax(state*, int, bool, int, int);
    std::string AIOptimalMove(state*);
    void startGame();
    bool gameEnds(state*);
    void printBoard();
};