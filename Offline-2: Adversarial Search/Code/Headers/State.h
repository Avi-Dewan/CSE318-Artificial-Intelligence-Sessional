#ifndef STATE_H
#define STATE_H

#include<vector>
#include "Mancala_Board.h"
class State
{
private:
    Mancala mancala;
    int prevChoice;

    int heuristic1;
    int heuristic2;
    int heuristic3;
    int heuristic4;

public:
    State(Mancala, int);
    ~State();

    int getHeuristic1() { return heuristic1; }
    int getHeuristic2() { return heuristic2; }
    int getHeuristic3() { return heuristic3; }
    int getHeuristic4() { return heuristic4; }
    int getPrevChoice() { return prevChoice; }
    vector<State*> getNextStates();
    bool isGameOver();
    Mancala getMancala() { return mancala; }
    bool isMoveA() { return mancala.isNextMoveA(); }
};



State::State(Mancala mancala, int prevChoice)
{   
    this->mancala = mancala;
    this->prevChoice = prevChoice;

    this->heuristic1 = mancala.getDiffStorageAB();
    this->heuristic2 = 2*mancala.getDiffStorageAB() + 1*mancala.getDiffSideAB();
    this->heuristic3 = 2*mancala.getDiffStorageAB() + 1*mancala.getDiffSideAB() + 1*mancala.getAdditionalMove();
    this->heuristic4 = 2*mancala.getDiffStorageAB() + 1*mancala.getDiffSideAB() + 1*mancala.getAdditionalMove() + 1*mancala.getStonesCaptured();
}

State::~State()
{
}

vector<State*> State::getNextStates() {
    vector<State*> next_states;
    Mancala newMancala;
    int moveReturn;
    
    for(int i = 1; i <= 6; i++) {
        newMancala = mancala;

        if(newMancala.isNextMoveA())
            moveReturn = newMancala.makeMoveA(i);
        else
            moveReturn = newMancala.makeMoveB(i);

        if(moveReturn != -1) 
            next_states.push_back(new State(newMancala, i));
    }


    return next_states;
}

bool State::isGameOver() {
    return this->mancala.checkGameOver();
}


#endif