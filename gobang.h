#include <vector>
class gobang
{
private:
    struct state
    {
        char **board;
    };
    state currentState;
    int boardSize;
    void HumanTurn(char);
    bool gameEnds();

public:
    gobang(int boardSize = 11);
    double evalFunc(state);
    state optimalMove(state);
    void startGame(char);
    void printBoard();
};