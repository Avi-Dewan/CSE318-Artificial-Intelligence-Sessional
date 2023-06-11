#include <vector>

using namespace std;

class Heuristic {

public:

    static int get_goal_row(int number, int n) {
        if(number == 0) return n-1;
        else return (number-1)/n;
    }

    static int get_goal_column(int number, int n) {
        if(number == 0) return n-1;
        else return (number-1)%n;
    }

    static int get_present_row(int idx, int n) {
        return idx/n;
    }

    static int get_present_column(int idx, int n) {
        return idx%n;
    }

    static int get_idx_from_row_col(int row, int col, int n) {
        return row*n + col;
    }

   

    static int hamming_distance(int n, vector<int> board) {

        int cost = 0;


        for(int i = 0; i < n*n; i++) {
            if(board[i] != i+1) cost++;
        }
    
        return --cost;
    }

    static int manhattan_distance(int n, vector<int> board) {
        int cost = 0;

    

        for(int i = 0; i < n*n; i++) {
            if(board[i] == 0) continue;

            int single_man_dis = abs(get_goal_row(board[i], n) - get_present_row(i, n)) +
                                abs(get_goal_column(board[i], n) - get_present_column(i, n));

            cost += single_man_dis;
        }
    
        return cost;
    }

    
};