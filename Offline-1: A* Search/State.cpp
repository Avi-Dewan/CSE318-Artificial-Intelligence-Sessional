#include <iostream>
#include <vector>



using namespace std;


class State{
    int n;
    vector<int> board;
    int g_n; // tne actual cost
    int f_n_ham;  //priority function value using hamming distance as heuristic 
    int f_n_man;  //priority function value using manhattan distance as heuristic
    
    State* prev_state;

    void swap(vector<int> &vect, int i, int j) {
        int temp = vect[i];
        vect[i] = vect[j];
        vect[j] = temp;
    }



public: 

    State(int n, vector<int> board, int g_n, State* prev_state) {
        this->n = n;
        this->board = board;
        this->g_n = g_n;
        this->f_n_ham = g_n + Heuristic::hamming_distance(n, board);
        this->f_n_man = g_n + Heuristic::manhattan_distance(n, board);
        this->prev_state = prev_state;
    }


    vector<int> getBoard() {
        return board;
    }

    int getN() const {
        return n;
    }

    int getG_n() const {
        return g_n;
    }

    int getF_n_ham() const {
        return f_n_ham;
    }

    int getF_n_man() const {
        return f_n_man;
    }

    State* getPrev_state() {
        return prev_state;
    }
    
    void print() {
        // cout << n;

        for(int i = 0; i < n*n; i++) {
            if(i % n == 0) cout << endl;
            cout << board[i] << " ";

        }

        cout << endl;

        // cout << "g_n = " << g_n << endl;
        // cout << "f_n_hamming = " << f_n_ham << endl;
        // cout << "f_n_man = " << f_n_man << "\n--------------------------" << endl;
    }

    vector<State*> getNextStates() {
        vector<State*> next_states;
        vector<int> newBoard;
        int zero_index = 0;

        for(int i = 0; i < n*n; i++) 
            if (board[i] == 0) {
                zero_index = i;
                break;
            }

        int zero_row = Heuristic::get_present_row(zero_index, n), zero_column = Heuristic::get_present_column(zero_index, n);

        // cout << zero_row << " " << zero_column << endl;

        if(zero_column == 0) {
            if(zero_row == 0) {
                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(0,1, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(1,0, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));
    
                // State nextState(n, newBoard, g_n+1, this);
                // next_states.push_back(nextState);
            } else if(zero_row == n-1) {
                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(n-2, 0, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(n-1, 1, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));
            } else {
                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row-1, 0, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row+1, 0, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, 1, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));
            }

        } else if(zero_column == n-1) {
            if(zero_row == 0) {
                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(0, n-2, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(1, n-1, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));
    
                // State nextState(n, newBoard, g_n+1, this);
                // next_states.push_back(nextState);
            } else if(zero_row == n-1) {
                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(n-2, n-1, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(n-1, n-2, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));
            } else {
                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row-1, n-1, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row+1, n-1, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));

                newBoard = board;
                swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, n-2, n));
                next_states.push_back(new State(n, newBoard, g_n+1, this));
            }
        } else if(zero_row == 0) {
            
            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, zero_column+1, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, zero_column-1, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row+1, zero_column, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));
            
        } else if(zero_row == n-1) {

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, zero_column+1, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, zero_column-1, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row-1, zero_column, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

        } else {

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, zero_column+1, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row, zero_column-1, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row+1, zero_column, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

            newBoard = board;
            swap(newBoard, zero_index, Heuristic::get_idx_from_row_col(zero_row-1, zero_column, n));
            next_states.push_back(new State(n, newBoard, g_n+1, this));

        }

        return next_states;
    }

};