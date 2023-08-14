#ifndef MANCALA_BOARD_H
#define MANCALA_BOARD_H

#include<vector>
#include<iostream>

using namespace std;

class  Mancala
{
private:
    vector<int> pitsA;
    vector<int> pitsB;
    int mancalaA;
    int mancalaB;

    bool nextMoveA;

    int additionalMove;
    int stonesCaptured;

public:
     Mancala();
    ~ Mancala();

    void showboard();

    int makeMoveA(int);
    int makeMoveB(int);
    bool checkGameOver();
    int getWinner();

    vector<int> getPitsA() { return pitsA; }
    vector<int> getPitsB() { return pitsB; }
    bool isNextMoveA() { return nextMoveA; }
    int getMancalaA() { return mancalaA; }
    int getMancalaB() { return mancalaB; }

    int getAdditionalMove() { return additionalMove; }
    int getStonesCaptured() { return stonesCaptured; }
    int getDiffStorageAB() { return (mancalaA - mancalaB); }
    int getDiffSideAB() { 
        int stonesA = 0, stonesB = 0;

        for(int i = 0; i < 6; i++) {
            stonesA += pitsA[i];
            stonesB += pitsB[i];
        }

        return (stonesA - stonesB);
    }
};


 Mancala:: Mancala()
{
    pitsA.assign(6, 4);
    pitsB.assign(6, 4);

    mancalaA = 0;
    mancalaB = 0;
    nextMoveA = true;
    additionalMove = 0;
    stonesCaptured = 0;
}

 Mancala::~ Mancala()
{
}

void Mancala::showboard() {
    cout << "|-----|-----------------------------------------------------|-----|" << endl;


    cout << "|   ";

    for(int i = 0; i < 6; i++) {
        cout << "  |   ";
        if(pitsB[i] < 10) cout <<"0";
        cout << pitsB[i] << " ";
    }

    cout << "  |     |" << endl;

    cout << "|  ";
    
    if(mancalaB < 10) cout << "0";
    
    cout << mancalaB << " |--------";


    for(int i = 1; i < 6; i++) {
        cout << "|--------";
    }


    cout << "|  ";
    if(mancalaA < 10) cout << "0";
    cout << mancalaA << " |" ;

    cout << endl;

    cout << "|   ";

    for(int i = 0; i < 6; i++) {
        cout << "  |   ";
        if(pitsA[i] < 10) cout <<"0";
        cout << pitsA[i] << " ";
    }

    cout << "  |     |" << endl;

    cout << "|-----|-----------------------------------------------------|-----|" << endl;


}

int Mancala::makeMoveA(int pitNo) {

    if(pitNo < 1 || pitNo > 6 ) {
        // cout << "Please input valid pit number ( 1 - 6) !" << endl; 
        return -1;
    }

    if(pitsA[pitNo-1] == 0) {
        // cout << "Pit no-" << pitNo << " is empty" << endl;
        return -1;
    }


    int gems = pitsA[pitNo-1];
    pitsA[pitNo-1] = 0;

    bool areaA = true;

    pitNo++;


    while (gems != 0)
    {
        if(areaA) {
            if(pitNo == 7) {
                mancalaA += 1;
                areaA = false;
                gems--;
                pitNo=6;

                if(gems == 0) {
                    if(checkGameOver()) return getWinner();
                    additionalMove++;
                    nextMoveA = true;
                    return 1;
                }
            } else {
                pitsA[pitNo-1] += 1;
                gems--;

                if(gems == 0) {
                    if(pitsA[pitNo-1] == 1 && pitsB[pitNo-1] > 0) {
                        mancalaA += (pitsA[pitNo-1] + pitsB[pitNo-1]);
                        stonesCaptured += (pitsB[pitNo-1] + 1);
                        pitsA[pitNo-1] = 0;
                        pitsB[pitNo-1] = 0;
                    }
                    if(checkGameOver()) return getWinner();
                    nextMoveA = false;
                    return 0;
                }

                pitNo++;
            }
        } else {
            if(pitNo == 0) {
                areaA = true;
                pitNo=1;
            } else {
                pitsB[pitNo-1] += 1;
                gems--;
                pitNo--;
            }

            if(gems == 0) {
                if(checkGameOver())return getWinner();
                nextMoveA = false;
                return 0;
            }
        }
    }

    return -1;

}


int Mancala::makeMoveB(int pitNo) {

    if(pitNo < 1 || pitNo > 6 ) {
        // cout << "Please input valid pit number ( 1 - 6) !" << endl; 
        return -1;
    }

    pitNo = 7 - pitNo;

    if(pitsB[pitNo-1] == 0) {
        // cout << "Pit no-" << pitNo << " is empty" << endl;
        return -1;
    }


    int gems = pitsB[pitNo-1];
    pitsB[pitNo-1] = 0;

    bool areaA = false;

    pitNo--;

    while (gems != 0)
    {
        if(areaA) {
            if(pitNo == 7) {
                areaA = false;
                pitNo=6;
            } else {
                pitsA[pitNo-1] += 1;
                gems--;
                pitNo++;
            }

            if(gems == 0){ 
                if(checkGameOver()) return getWinner();
                nextMoveA = true;
                return 0;
            }

        } else {

            if(pitNo == 0) {
                mancalaB += 1;

                areaA = true;
                gems--;
                pitNo=1;

                if(gems == 0) {
                    if(checkGameOver()) return getWinner();
                    nextMoveA = false;
                    return 1;
                }

            }  else {

                pitsB[pitNo-1] += 1;
                gems--;

                 if(gems == 0) {
                    if(pitsB[pitNo-1] == 1 && pitsA[pitNo-1] > 0) {
                        mancalaB += (pitsB[pitNo-1] + pitsA[pitNo-1]);
                        pitsA[pitNo-1] = 0;
                        pitsB[pitNo-1] = 0;
                    }

                    if(checkGameOver()) return getWinner(); 
                    nextMoveA = true;               
                    return 0;
                }

                pitNo--;
            }
        }
    }

    cout << "Not suupposed to come here" << endl;

    return -1;

}

bool Mancala::checkGameOver() {

    bool check1 = true, check2 = true;

    for(int i = 0; i < 6; i++) {
        if(pitsA[i] != 0) check1 = false;
        if(pitsB[i] != 0) check2 = false;
    }

    if(check1 || check2) return true;

    return false;
};

int Mancala::getWinner() {
    if(mancalaA == mancalaB) {
        // cout << "Draw" << endl;
        return 2;
    } else if (mancalaA > mancalaB)
    {
        // cout << "Winner: A !!!" << endl;
        return 3;
    } else {
        // cout << "Winner: B !!!" << endl;
        return 4;
    }
    
}


#endif