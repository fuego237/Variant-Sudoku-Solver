#include "grid.h"
#include "constraint.h"

Grid::Grid (std::string str, std::vector<std::unique_ptr<Constraint>> cons) : data(9, std::vector<int>(9,0)), solvable(false), constraints(std::move(cons)){
    int index = 0;
    for (const auto& ch: str) {
        if (std::isdigit(ch)) {
            data.at(index/9).at(index%9) = ch - '0';
            index += 1;
        }
    }
};

void Grid::removeCandidate(int row, int column, int value) {
    if (candidates.at(row).at(column).count(value)) {
        candidates.at(row).at(column).erase(value);
        GridChange change = {std::pair<int, int>{row, column}, value};
        change_stack.push(change);
    }
};

void Grid::enforce(Constraint& constraint, int row, int column, int value) {
    auto positions = constraint.get_positions(row, column);
    for (const auto& pos: positions) {
        if (data.at(pos.first).at(pos.second)==0) {
            constraint.change_candidates(*this, pos.first, pos.second, value);
        }
    }
}

void Grid::propagate(int row, int column, int value) {
    auto autofill = [&](const std::pair<int, int>& position) {
        int new_value = *candidates.at(position.first).at(position.second).begin();
        data.at(position.first).at(position.second) = new_value;
        GridChange change = {position, new_value};
        change_stack.push(change);
        propagate(position.first, position.second, new_value);
    };

    for (const auto& constraint: constraints) {
        enforce(*constraint, row, column, value);
    }
    std::set<std::pair<int, int>> all_positions;
    for (const auto& constraint: constraints) {
        auto positions = constraint->get_positions(row,column);
        all_positions.insert(positions.begin(),positions.end());
    }
    for (const auto& pos: all_positions) {
        if (data.at(pos.first).at(pos.second)==0) {
            if (candidates.at(pos.first).at(pos.second).size()==0) {
                return;
            }
            else if (candidates.at(pos.first).at(pos.second).size() == 1) {
                autofill(pos);
            }
        }
    }
    };

int Grid::find_empty(int index) {
    while (index < 81 && data.at(index/9).at(index%9) != 0) {
        index++;
    }
    return index;
}

void Grid::solve(int index) {
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
        GridChange change = {std::pair<int, int>{index/9, index%9}, i};
        change_stack.push(change);
        int curr_size = change_stack.size();

        // Make the change and propagate constraints
        data.at(index/9).at(index%9) = i;
        propagate(index/9,index%9,i);
        //log(100);

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
            } 
            candidates.at(last_change.position.first).at(last_change.position.second).insert(last_change.value);
        }
        //log(100);


    }
};

void Grid::set_candidates() {
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            if (data.at(i).at(j) != 0) {
                candidates.at(i).at(j).clear();
                for (const auto& constraint: constraints) {
                    enforce(*constraint, i, j, data.at(i).at(j));
                }
            }
        }
    }
    std::stack<GridChange>().swap(change_stack);
};

void Grid::full_solve() {
    set_candidates();
    solve(find_empty(0));
    if (solvable) {
        std::cout << "Solution found!" << std::endl;
    } else {
        std::cout << "No solution exists." << std::endl;
    }
};

        
void Grid::print_grid() {
    std::cout<<"---------------"<< std::endl;
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            std::cout << data.at(i).at(j) << " ";
        }
        std::cout << "\n";
    }
};