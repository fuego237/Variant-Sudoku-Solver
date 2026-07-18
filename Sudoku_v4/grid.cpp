#include "grid.h"
#include "constraint.h"
#include "position.h"

Grid::Grid (std::string str, std::vector<std::unique_ptr<Constraint>> cons) : data(9, std::vector<int>(9,0)), solvable(false), propagation_flag(false), constraints(std::move(cons)){
    int index = 0;
    for (const auto& ch: str) {
        if (std::isdigit(ch)) {
            data.at(index/9).at(index%9) = ch - '0';
            index += 1;
        }
    }
};

void Grid::removeCandidate(Position pos, int value) {
    if (candidates.at(pos.row).at(pos.column).count(value)) {
        candidates.at(pos.row).at(pos.column).erase(value);
        GridChange change = {pos, value};
        change_stack.push(change);
    }
};

void Grid::enforce(Constraint& constraint,Position pos, int value) {
    auto positions = constraint.get_positions(pos);
    for (const auto& neighbor_: positions) {
        if (data.at(neighbor_.row).at(neighbor_.column)==0) {
            constraint.change_candidates(*this, neighbor_, pos, value);
        }
    }
}

void Grid::propagate(Position pos, int value) {
    //std::cout << "Propagating value " << value << " at position (" << pos.row << ", " << pos.column << ")" << std::endl;
    //candidates.at(pos.row).at(pos.column).erase(value);
    auto autofill = [&](const Position& position) {
        int new_value = *candidates.at(position.row).at(position.column).begin();
        std::cout << "Autofilling value " << new_value << " at position (" << position.row << ", " << position.column << ")" << std::endl;
        data.at(position.row).at(position.column) = new_value;
        GridChange change = {position, new_value};
        change_stack.push(change);
        
        auto it = std::find(position_stack.begin(), position_stack.end(), position);
        if (it != position_stack.end()) {
            position_stack.erase(it);
        }

        propagate(position, new_value);
    };

    for (const auto& constraint: constraints) {
        enforce(*constraint, pos, value);
    }
    std::unordered_set<Position> all_positions;
    for (const auto& constraint: constraints) {
        auto positions = constraint->get_positions(pos);
        all_positions.insert(positions.begin(),positions.end());
    }
    for (const auto& adjacent_pos: all_positions) {
        if (data.at(adjacent_pos.row).at(adjacent_pos.column)==0) {
            if (candidates.at(adjacent_pos.row).at(adjacent_pos.column).size()==0) {
                propagation_flag = true;
                return;
            }
            else if (candidates.at(adjacent_pos.row).at(adjacent_pos.column).size() == 1) {
                autofill(adjacent_pos);
            }
        }
    }
    };

Position Grid::find_empty() {
    if (!position_stack.empty()) {
        Position next_pos = position_stack.back();
        return next_pos;
    }
    return Position{9, 9}; // Return an invalid position if no empty cell is found
}

void Grid::solve() {
    //std::system("cls");
    //std::cout << index << std::endl;
    Position pos = find_empty();
    //std::cout << "Next empty position: (" << pos.row << ", " << pos.column << ")" << std::endl;
    if (pos.row * 9 + pos.column >= 81) {
        solvable = true;
        return;
    }
    if (candidates.at(pos.row).at(pos.column).empty()) {
        return;
    }
    std::set<int> candidates_copy = candidates.at(pos.row).at(pos.column);
    for (const auto& i: candidates_copy) {
        // Save the current state before making a change
        GridChange change = {pos, i};
        change_stack.push(change);
        int curr_size = change_stack.size();
        position_stack.pop_back();
        //system("cls");
        std::cout << "Trying value " << i << " at position (" << pos.row << ", " << pos.column << ")" << std::endl;
        //std::cout << "Cells Left: " << position_stack.size() << std::endl;
        // Make the change and propagate constraints
        data.at(pos.row).at(pos.column) = i;
        propagate(pos, i);
        if (!propagation_flag) {
            solve();
            if (solvable) {
                return;
            }
        } else {
            propagation_flag = false;
        };
        // Backtrack: undo changes until we reach the current change
        //std::cout << "Backtracking from position (" << pos.row << ", " << pos.column << ") with value " << i << std::endl;
        while (change_stack.size() > curr_size-1) {
            GridChange last_change = change_stack.top();
            change_stack.pop();
            if (data.at(last_change.position.row).at(last_change.position.column) != 0) {
                data.at(last_change.position.row).at(last_change.position.column) = 0;
                position_stack.push_back(last_change.position);
                //std::cout << "Backtracking on position (" << last_change.position.row << ", " << last_change.position.column << ") with value " << last_change.value << std::endl;
            } 
            candidates.at(last_change.position.row).at(last_change.position.column).insert(last_change.value);
        }
        std::cout << "Backtracked to position (" << pos.row << ", " << pos.column << ") with value " << i << std::endl;
    }
};

void Grid::set_candidates() {
    for (const auto& constraint: constraints) {
        constraint->setupGrid(*this);
    }
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            if (data.at(i).at(j) != 0) {
                candidates.at(i).at(j).clear();
                for (const auto& constraint: constraints) {
                    enforce(*constraint, Position{i, j}, data.at(i).at(j));
                }
            }
        }
    }
    std::stack<GridChange>().swap(change_stack);
    std::vector<std::pair<Position, int>> remaining_positions;
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            if (data.at(i).at(j) == 0) {
                remaining_positions.push_back({Position{i, j}, candidates.at(i).at(j).size()});
            }
        }
    }
    std::sort(remaining_positions.begin(), remaining_positions.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    for (const auto& p: remaining_positions) {
        position_stack.push_back(p.first);
    }
};

void Grid::full_solve() {
    set_candidates();
    solve();
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