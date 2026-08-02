#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ostream>
#include <set>
#include <iterator>
#include <chrono>
#include <thread>  
#include <utility> 
#include <stack>

struct GridChange {
    std::pair<int, int> position;
    int value;
};

class Grid {
    public:
        std::vector<std::vector<int>> data{9, std::vector<int>{9}};
        std::vector<std::vector<std::set<int>>> candidates{9, std::vector<std::set<int>>(9, std::set<int>{1,2,3,4,5,6,7,8,9})};
        std::stack<GridChange> change_stack;
        bool solvable;

        Grid() : data(9, std::vector<int>(9,0)), solvable(false) {};

        Grid (std::string str) : data(9, std::vector<int>(9,0)), solvable(false){
            int index = 0;
            for (const auto& ch: str) {
                if (std::isdigit(ch)) {
                    data.at(index/9).at(index%9) = ch - '0';
                    index += 1;
                }
            }
        };

        std::vector<std::pair<int, int>> get_row_positions(int row, int column) {
            std::vector<std::pair<int, int>> positions;
            for (int i=0; i<9; i++) {
                if (i != column) {
                    positions.push_back(std::make_pair(row, i));
                }
            }
            return positions;
        };

        std::vector<std::pair<int, int>> get_column_positions(int row, int column) {
            std::vector<std::pair<int, int>> positions;
            for (int i=0; i<9; i++) {
                if (i != row) {
                    positions.push_back(std::make_pair(i, column));
                }
            }
            return positions;
        };

        std::vector<std::pair<int, int>> get_box_positions(int row, int column) {
            std::vector<std::pair<int, int>> positions;
            int box_row = row/3;
            int box_col = column/3;
            for (int i=0; i<3; i++) {
                for (int j=0; j<3; j++) {
                    if (box_row*3 + i != row || box_col*3 + j != column) {
                        positions.push_back(std::make_pair(box_row*3 + i, box_col*3 + j));
                    }
                }
            }
            return positions;
        };

        void propagate(int row, int column, int value) {
            //std::cout << "Propagating value " << value << " at position (" << row << ", " << column << ")" << std::endl;
            //candidates.at(row).at(column).erase(value);
            auto autofill = [&](const std::pair<int, int>& position) {
                int new_value = *candidates.at(position.first).at(position.second).begin();
                data.at(position.first).at(position.second) = new_value;
                propagate(position.first, position.second, new_value);
                GridChange change = {position, new_value};
                change_stack.push(change);
            };

            std::set<std::pair<int, int>> all_positions;
            auto row_pos = get_row_positions(row, column);
            auto col_pos = get_column_positions(row, column);
            auto box_pos = get_box_positions(row, column);
            all_positions.insert(row_pos.begin(), row_pos.end());
            all_positions.insert(col_pos.begin(), col_pos.end());
            all_positions.insert(box_pos.begin(), box_pos.end());

            std::vector<std::pair<int, int>> positions_to_autofill;
            for (const auto& pos: all_positions) {
                if (data.at(pos.first).at(pos.second) == 0 && candidates.at(pos.first).at(pos.second).count(value)) {
                    candidates.at(pos.first).at(pos.second).erase(value);
                    
                    GridChange change = {pos, value};
                    change_stack.push(change);

                    if (candidates.at(pos.first).at(pos.second).size() == 1) {
                        positions_to_autofill.push_back(pos);
                    }
                }
            }
            for (const auto& pos: positions_to_autofill) {
                if (candidates.at(pos.first).at(pos.second).size() == 0) {
                    break;
                }
                autofill(pos);
            }
        };

        int find_empty(int index) {
            while (index < 81 && data.at(index/9).at(index%9) != 0) {
                index++;
            }
            return index;
        }

        void solve(int index) {
            if (index>=81) {
                solvable = true;
                return;
            }
            if (candidates.at(index/9).at(index%9).empty()) {
                return;
            }
            std::set<int> candidates_copy = candidates.at(index/9).at(index%9);
            for (const auto& i: candidates_copy) {
                // Save the current state before making a change
                GridChange change = {std::make_pair(index/9, index%9), i};
                change_stack.push(change);
                int curr_size = change_stack.size();


                // Make the change and propagate constraints
                data.at(index/9).at(index%9) = i;
                propagate(index/9,index%9,i);

                solve(find_empty(index+1));
                if (solvable) {
                    return;
                }
                // Backtrack: undo changes until we reach the current change
                while (change_stack.size() > curr_size - 1) {
                    GridChange last_change = change_stack.top();
                    change_stack.pop();
                    if (data.at(last_change.position.first).at(last_change.position.second) != 0) {
                        data.at(last_change.position.first).at(last_change.position.second) = 0;
                        //std::cout << "Backtracking on position (" << last_change.position.first << ", " << last_change.position.second << ") with value " << last_change.value << std::endl;
                    } 
                    candidates.at(last_change.position.first).at(last_change.position.second).insert(last_change.value);
                }


            }
        };

        void set_candidates() {
            for (int i=0; i<9; i++) {
                for (int j=0; j<9; j++) {
                    if (data.at(i).at(j) != 0) {
                        candidates.at(i).at(j).clear();
                        for (const auto& pos: get_row_positions(i, j)) {
                            candidates.at(pos.first).at(pos.second).erase(data.at(i).at(j));
                        }
                        for (const auto& pos: get_column_positions(i, j)) {
                            candidates.at(pos.first).at(pos.second).erase(data.at(i).at(j));
                        }
                        for (const auto& pos: get_box_positions(i, j)) {
                            candidates.at(pos.first).at(pos.second).erase(data.at(i).at(j));
                        }
                    } 
                }
            }
        };
            
        void full_solve() {
            set_candidates();
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
                    std::cout << data.at(i).at(j) << " ";
                }
                std::cout << "\n";
            }
        };
};

int main() {
    std::string str_grid = "0,1,2 | 0,7,0 | 0,0,3"
                           "0,0,0 | 1,0,4 | 2,0,0"
                           "3,0,6 | 0,0,0 | 0,7,0"
                           "_____________________"
                           "0,0,0 | 4,0,0 | 0,0,5"
                           "1,9,4 | 0,2,8 | 0,3,0"
                           "0,8,3 | 0,1,0 | 0,0,0"
                           "_____________________"
                           "7,0,0 | 0,0,0 | 0,0,8"
                           "0,0,0 | 0,0,2 | 0,0,0"
                           "0,0,8 | 0,0,0 | 4,0,0";
    Grid grid1(str_grid);
    auto start = std::chrono::steady_clock::now();
    grid1.full_solve();
    auto end = std::chrono::steady_clock::now();
    if (grid1.solvable) {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Solve time: " << duration.count() << " microseconds\n";
        grid1.print_grid();
    }
    return 0;
}

    