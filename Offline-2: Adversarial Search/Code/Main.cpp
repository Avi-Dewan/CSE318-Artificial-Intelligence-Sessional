#include<iostream>
#include "Headers/Adversial_Search.h"
#include "Headers/Mancala_Board.h"
#include "Headers/State.h"

using namespace std;

int main() {

    Mancala mancala;

    mancala.showboard();

    int move, moveReturn;

    Adversial_Search adversial_search;

    while (true)
    {

        if(mancala.isNextMoveA()) {
            cout << "Enter move A: " << endl;
            // cin  >> move;

            move = adversial_search.minimax(new State(mancala, -1), 15, true);
            cout << move << endl;
            moveReturn = mancala.makeMoveA(move);

            // if(moveReturn == -1) cout << "That pit is empty" << endl;

        } else {
            cout << "Enter move B: " << endl;
            // cin  >> move;
            // State state(mancala, -1);

            move = adversial_search.minimax(new State(mancala, -1), 15, false);
            cout << move << endl;
            moveReturn = mancala.makeMoveB(move); 

            // if(moveReturn == -1) cout << "That pit is empty" << endl;

        }
        
        if(moveReturn != -1) mancala.showboard();

        if(moveReturn >= 2 && moveReturn <= 4) break;
    }
    

    if(moveReturn == 2) {
        cout << "DRAW......" << endl;
    } else if(moveReturn == 3) {
        cout << "Winner A !!!" << endl;
    } else {
        cout << "Winner B !!!" << endl;
    }

    return 0;
}