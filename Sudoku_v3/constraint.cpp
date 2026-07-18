# include "constraint.h"
# include "grid.h"

std::vector<std::pair<int, int>> RowConstraint::get_positions(int row, int column) {
    std::vector<std::pair<int, int>> positions;
    for (int i=0; i<9; i++) {
        if (i != column) {
            positions.push_back(std::pair<int, int>{row, i});
        }
    }
    return positions;
}

void RowConstraint::change_candidates(Grid& grid, int row, int column, int value) {
    grid.removeCandidate(row, column, value);
}

std::vector<std::pair<int, int>> ColumnConstraint::get_positions(int row, int column) {
    std::vector<std::pair<int, int>> positions;
    for (int i=0; i<9; i++) {
        if (i != row) {
            positions.push_back(std::pair<int, int>{i, column});
        }
    }
    return positions;
}

void ColumnConstraint::change_candidates(Grid& grid, int row, int column, int value) {
    grid.removeCandidate(row, column, value);
}

std::vector<std::pair<int, int>> BoxConstraint::get_positions(int row, int column) {
    std::vector<std::pair<int, int>> positions;
    int box_row = row/3;
    int box_col = column/3;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (box_row*3 + i != row || box_col*3 + j != column) {
                positions.push_back(std::pair<int, int>{box_row*3 + i, box_col*3 + j});
            }
        }
    }
    return positions;
}

void BoxConstraint::change_candidates(Grid& grid, int row, int column, int value) {
    grid.removeCandidate(row, column, value);
}

