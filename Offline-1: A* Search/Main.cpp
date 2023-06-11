#include <iostream>
#include<fstream>
#include <vector>
#include<queue>
#include<map>
// #include<thread>
// #include<chrono>
#include "Heuristic.cpp"
#include "State.cpp"

using namespace std;

struct ComparePointers {
    bool operator()(const State* a, const State* b) const {
        // Comparison based on priority
        return a->getF_n_ham() > b->getF_n_ham();
    }
};

priority_queue<State*, vector<State*>, ComparePointers> PQ;
map<vector<int>, State*> visited;
int exploredStates = 0, expandedStates = 0;

bool isVisited(vector<int> board) {
    if(visited.find(board) != visited.end()) return true;
    return false;
}


bool compareVector(vector<int> a, vector<int> b, int n) {

    for(int i = 0; i < n*n; i++) {
        if(a[i] != b[i]) return false;
    }

    return true;
}



State* A_star_search() {

    while(!PQ.empty()) {
        State *nowState = PQ.top();
        PQ.pop();

        

        if(isVisited(nowState->getBoard())) continue;
        visited[nowState->getBoard()] = nowState;
        
        int n = nowState->getN();

        vector<int> goal(n*n);

        for(int i = 0; i < n*n; i++) {
            goal[i] = i+1;
            if(i == n*n-1) goal[i] = 0;
        }


        if(compareVector(goal, nowState->getBoard(), n)) return nowState;

        expandedStates++;

        for(State *nextState: nowState->getNextStates()) {
            
            if( nowState->getPrev_state() == NULL ||
                (!compareVector(nextState->getBoard(), nowState->getPrev_state()->getBoard(), n))
             ) {
                PQ.push(nextState);
                exploredStates++;
             } else {
                delete nextState;
             }
                
        }
        // this_thread::sleep_for(chrono::seconds(9));
    }

    return NULL;
}

void printExpolredStates(State *state) {

    vector<State*> path;

    if(state == NULL) return;

    cout << "Minimum number of moves = " << state->getG_n() << endl;
    cout << "Total explored Nodes = " << exploredStates << endl;
    cout << "Total expanded Nodes = " << expandedStates << endl;

    while(state != NULL) {
        path.push_back(state);
        state = state->getPrev_state();
    }


    for(int i = path.size()-1; i >= 0; i--) {
        path[i]->print();

        delete path[i];
    }

    cout << endl;

    while(!PQ.empty()) {
        State *s = PQ.top();
        PQ.pop();
        delete s;
    }
}


int inversion_count(vector<int> v) {
    int count = 0;

    for(int i = 0; i < v.size(); i++) {
        for(int j = i+1; j < v.size(); j++) {
            if(v[i] > v[j]) count++;
        }
    }

    return count;

}

bool isSolvable(vector<int> board, int n) {
    // Odd puzzle inversion count is Even if solvable. Because end inversion count = 0 and change is even.
    // So, must start even
    // for even puzzle, X = IC + Row distance of zero. It must be even

    int zero_index = 0;

    for(int i = 0; i < n*n; i++) 
        if (board[i] == 0) {
            zero_index = i;
            break;
        }

    board.erase(board.begin() + zero_index);

    int ic = inversion_count(board);
    
    if(n%2 != 0) {

        if(ic % 2 == 0) return true;
        return false;

    } else {
        int row_distance_zero = abs(Heuristic::get_present_row(zero_index, n) - (n-1));
        int X = ic + row_distance_zero;

        if(X % 2 == 0) return true;
        return false;
    }
}



int main()
{

     // Redirecting cout to a file
    ofstream fileOut("output.txt");
    streambuf* coutBuffer = cout.rdbuf(); // Save cout buffer
    cout.rdbuf(fileOut.rdbuf()); // Redirect cout to the file

    // Redirecting cin from a file
    ifstream fileIn("in06.txt");
    streambuf* cinBuffer = cin.rdbuf(); // Save cin buffer
    cin.rdbuf(fileIn.rdbuf()); // Redirect cin from the file

    int n;

    cin >> n;

    vector<int> board(n*n);

    for(int i = 0; i < n*n; i++) {
        cin >> board[i];
    }

    if(!isSolvable(board, n)) {
        cout << "Unsolvable puzzle" << endl;
        return 0;    
    }

    State *init = new State(n, board, 0, NULL);


    PQ.push(init);
    exploredStates++;

    State *goal = A_star_search();

    if(goal != NULL) printExpolredStates(goal);

      // Restore cout and cin
    cout.rdbuf(coutBuffer);
    cin.rdbuf(cinBuffer);

    // Close the files
    fileOut.close();
    fileIn.close();

        
    return 0;
}