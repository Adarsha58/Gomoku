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
    double horizantal(state *, bool);
    double vertical(state *, bool);
    double diagonal(state *, bool);
    int countPattern(std::string, std::string);
    bool hasPiecesNearThem(state *, int, int);

public:
    gobang(int boardSize, char AIPiece);
    bool isTie(state *);
    double evalFunc(state *, int, bool);
    double minmax(state *, int, bool, double &, double &);
    std::string AIOptimalMove(state *);
    void startGame();
    bool gameEnds(state *);
    void printBoard();
};