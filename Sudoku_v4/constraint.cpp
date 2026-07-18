# include "constraint.h"
# include "grid.h"
# include "position.h"

void BaseConstraint::change_candidates(Grid& grid, Position pos, Position current_pos, int value) {
    grid.removeCandidate(pos, value);
}

std::vector<Position> RowConstraint::get_positions(Position pos) {
    std::vector<Position> positions;
    for (int i=0; i<9; i++) {
        if (i != pos.column) {
            positions.push_back(Position{pos.row, i});
        }
    }
    return positions;
}

std::vector<Position> ColumnConstraint::get_positions(Position pos) {
    std::vector<Position> positions;
    for (int i=0; i<9; i++) {
        if (i != pos.row) {
            positions.push_back(Position{i, pos.column});
        }
    }
    return positions;
}

std::vector<Position> BoxConstraint::get_positions(Position pos) {
    std::vector<Position> positions;
    int box_row = pos.row/3;
    int box_col = pos.column/3;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (box_row*3 + i != pos.row || box_col*3 + j != pos.column) {
                positions.push_back(Position{box_row*3 + i, box_col*3 + j});
            }
        }
    }
    return positions;
}


LineConstraint::LineConstraint(std::vector<std::vector<Position>> lines) : neighbors(std::unordered_map<Position, std::vector<Position>>()) {
    for (const auto& positions: lines) {
        for (size_t index = 0; index < positions.size(); ++index) {
            const auto& pos = positions[index];

            auto key_it = neighbors.find(pos);
            if (key_it == neighbors.end()) {
                auto kv_pair = neighbors.emplace(pos, std::vector<Position>());
                key_it = kv_pair.first;
            } 
            if (index == 0) {
                key_it->second.push_back(positions[index + 1]);
            }
            else if (index == positions.size() - 1) {
                key_it->second.push_back(positions[index - 1]);
            }
            else {
                key_it->second.push_back(positions[index - 1]);
                key_it->second.push_back(positions[index + 1]);
            }
        }
    }
}

std::vector<Position> LineConstraint::get_positions(Position pos) {
    auto it = neighbors.find(pos);
    if (it != neighbors.end()) {
        return std::vector<Position>(it->second.begin(), it->second.end());
    }
    return std::vector<Position>();
}

void GermanWhisperConstraint::change_candidates(Grid& grid, Position pos, Position current_pos, int value) {
    for (int i =1; i<= 9; i++) {
        if (i>value-5 && i<value+5) {
            grid.removeCandidate(pos,i);
        }
    }
}
// method to remove 5 for all positions on lines
void GermanWhisperConstraint::setupGrid(Grid& grid) {
    for (const auto& [pos, neighbors_set]: neighbors) {
        grid.removeCandidate(pos, 5);
        if (neighbors_set.size()==2) {
            if (neighbors_set[0].related(neighbors_set[1])) {
                grid.removeCandidate(pos,4);
                grid.removeCandidate(pos,6);
            }
        }
    }
}

RenbanConstraint::RenbanConstraint(std::vector<std::vector<Position>> lines) : LineConstraint(lines) {
    for (const auto& positions: lines) {
        for (size_t index = 0; index < positions.size(); ++index) {
            const auto& pos = positions[index];

            auto key_it = neighbors.find(pos);
            key_it->second.clear();
            // Add all other positions in the line as neighbors
            for (size_t j = 0; j < positions.size(); ++j) {
                if (j != index) {
                    key_it->second.push_back(positions[j]);
                }
            }
        }
    }
};

void RenbanConstraint::change_candidates(Grid& grid, Position pos, Position current_pos, int value) {
    int range = neighbors.at(pos).size();
    for (int i = 1; i<= 9; i++) {
        if (i<value-range || i>value+range || i==value) {
            grid.removeCandidate(pos,i);
        }
    }
}

ThermoConstraint::ThermoConstraint(std::vector<std::vector<Position>> lines) : LineConstraint(lines), bulbs(std::unordered_map<Position, std::vector<Position>>()) {
    // For each line, the first position is the bulb, and the rest are the positions that must be greater than the previous one
    // Keep track of the bulbs and set dictionary of neighbors for each bulb to the rest of the positions in the line
    for (const auto& positions: lines) {
        bulbs[positions[0]] = std::vector<Position>(positions.begin() + 1, positions.end());
    }
};

void ThermoConstraint::change_candidates(Grid& grid, Position pos, Position current_pos, int value) {
    if (bulbs.contains(pos)) {
        for (int i = 1; i<=9; i++) {
            if (i>=value)  {
                grid.removeCandidate(pos,i);
            }
        }
    }
    else {
        int index = std::find(neighbors.at(pos).begin(), neighbors.at(pos).end(), current_pos) - neighbors.at(pos).begin();
        if (index==0) {
            for (int i = 1; i<=9; i++) {
                if (i<=value)  {
                    grid.removeCandidate(pos,i);
                }
            }
        }
        else {
            for (int i = 1; i<=9; i++) {
                if (i>=value)  {
                    grid.removeCandidate(pos,i);
                }
            }
        }
    }
}

void ThermoConstraint::setupGrid(Grid& grid) {
    for (auto& [bulb, line]: bulbs) {
        line.insert(line.begin(), bulb);
        int size = line.size();
        for (auto const [index, pos]: std::views::enumerate(line)) {
            for (int i=1; i<=9; i++) {
                 if (i<index+1 || i>9+index+1-size) {
                    grid.removeCandidate(pos, i);
                }
            }
        }
        line.erase(line.begin());
    }
    
}

