#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iterator>
#include <chrono>
#include <thread>   

class Grid {
    public:
        std::vector<std::vector<int>> data{9, std::vector<int>{9}};
        bool solvable;

        Grid() : data(9, std::vector<int>(9,0)), solvable(false) {};

        Grid (std::string str) : data(9, std::vector<int>(9,0)), solvable(false){
            int index = 0;
            for (const auto& ch: str) {
                if (std::isdigit(ch)) {
                    data[index/9][index%9] = ch - '0';
                    index += 1;
                }
            }
        };

        std::vector<int> get_row(int row) {
            return data[row];
        };

        std::vector<int> get_column(int column) {
            std::vector<int> col(9);
            for (int i=0; i<9; i++) {
                col[i] = data[i][column];
            }
            return col;
        };

        std::vector<int> get_box(int row, int column) {
            std::vector<int> box(9);
            int box_row = row / 3;
            int box_col = column / 3;
            for (int i=0; i<3; i++) {
                for (int j=0; j<3; j++) {
                    box[i*3 + j] = data[box_row*3 + i][box_col*3 + j];
                }
            }
            return box;
        };

        bool is_valid(int row, int column, int value) {
            std::vector<int> row_vec = get_row(row);
            std::vector<int> col_vec = get_column(column);
            std::vector<int> box_vec = get_box(row, column);
            for (int i=0; i<9; i++) {
                if (row_vec[i] == value || col_vec[i] == value || box_vec[i] == value) {
                    return false;
                }
            }
            return true;

        }

        int find_empty(int index) {
            while (index < 81 && data[index/9][index%9] != 0) {
                index++;
            }
            return index;
        }

        void solve(int index) {
            if (index>=81) {
                solvable = true;
                return;
            }
            for (int i=1; i<10; i++) {
                if (is_valid(index/9, index%9, i)) {
                    data[index/9][index%9] = i;
                    //std::this_thread::sleep_for(std::chrono::milliseconds(50));

                    solve(find_empty(index+1));
                    if (solvable) {
                        return;
                    }

                    data[index/9][index%9] = 0;
                    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                }
            }

        void full_solve() {
            solve(find_empty(0));
            if (solvable) {
                std::cout << "Solution found!" << std::endl;
            } else {
                std::cout << "No solution exists." << std::endl;
            }
        };

        
        
        void print_grid() {
            std::cout<<"---------------"<< std::endl;
            for (int i=0; i<9; i++) {
                for (int j=0; j<9; j++) {
                    std::cout << data[i][j] << " ";
                }
                std::cout << "\n";
            }
        };
};

int main() {
    std::string str_grid = "0,5,0 | 0,9,2 | 0,0,0"
                           "0,6,0 | 4,0,8 | 0,0,2"
                           "0,0,0 | 0,0,0 | 0,0,0"
                           "_____________________"
                           "0,0,0 | 3,0,0 | 6,5,0"
                           "0,0,3 | 0,0,0 | 4,8,0"
                           "8,1,0 | 0,0,0 | 0,0,7"
                           "_____________________"
                           "0,0,0 | 0,7,0 | 2,0,0"
                           "2,0,0 | 6,0,5 | 8,0,0"
                           "7,0,0 | 0,0,9 | 0,6,0";
    Grid grid1(str_grid);
    grid1.full_solve();
    grid1.print_grid();
}