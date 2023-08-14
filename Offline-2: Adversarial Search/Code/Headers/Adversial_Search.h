#ifndef ADVERSIAL_SEARCH_H
#define ADVERSIAL_SEARCH_H

#include "State.h"

using namespace std;

class Adversial_Search
{
private:
    static const int MAX_SCORE = 1000;
    static const int MIN_SCORE = -1000;

public:
    Adversial_Search();
    ~Adversial_Search();

    int alphaBetaPrune(State *, int, int, int, bool);
    int minimax(State*, int, bool);
};

Adversial_Search::Adversial_Search(/* args */)
{
}

Adversial_Search::~Adversial_Search()
{
}


int Adversial_Search::alphaBetaPrune(State *state, int depth, int alpha, int beta, bool maximizingPlayer) {

    if(depth == 0 || state->isGameOver() ) {
        if(maximizingPlayer)
            return state->getHeuristic2();
        else
            return state->getHeuristic4();
    }


    if(maximizingPlayer) {
        int maxScore = MIN_SCORE;

        for (State* nextState: state->getNextStates()) {

            int score;
            if(nextState->isMoveA()) score = alphaBetaPrune(nextState, depth - 1, alpha, beta, true);
            else score = alphaBetaPrune(nextState, depth - 1, alpha, beta, false);

            maxScore = max(maxScore, score);
            alpha = max(alpha, maxScore);

            if (alpha >= beta) {
                break; // Prune
            }
        }

        return maxScore;
    } else {
        int minScore = MAX_SCORE;

        for (State* nextState: state->getNextStates()) {

            int score;
            if( nextState->isMoveA()) score = alphaBetaPrune(nextState, depth - 1, alpha, beta, true);
            else score = alphaBetaPrune(nextState, depth - 1, alpha, beta, false);

            minScore = min(minScore, score);
            beta = min(beta, minScore);

            if (alpha >= beta) {
                break; // Prune
            }
        }

        return minScore;
    }

}

int Adversial_Search::minimax(State* state, int depth, bool maximizingPlayer) {
    int alpha = MIN_SCORE, beta = MAX_SCORE;
    int bestScore, bestChoice;

    if(maximizingPlayer) {
        bestScore = MIN_SCORE;
        bestChoice = -1;
        for (State* nextState: state->getNextStates()) {

            int score;
            if(nextState->isMoveA()) score = alphaBetaPrune(nextState, depth - 1, alpha, beta, true);
            else score = alphaBetaPrune(nextState, depth - 1, alpha, beta, false);

            bestScore = max(score, bestScore);

            if(bestScore > alpha) {
                alpha = bestScore;
                bestChoice = nextState->getPrevChoice();
            }
        }

        return bestChoice;
    } else {
        bestScore = MAX_SCORE;
        bestChoice = -1;

        for (State* nextState: state->getNextStates()) {

            int score;
            if( nextState->isMoveA()) score = alphaBetaPrune(nextState, depth - 1, alpha, beta, true);
            else score = alphaBetaPrune(nextState, depth - 1, alpha, beta, false);

            bestScore = min(score, bestScore);
            
            if(bestScore < beta) {
                beta = bestScore;
                bestChoice = nextState->getPrevChoice();
            }
        }

        return bestChoice;

    }
}

#endif