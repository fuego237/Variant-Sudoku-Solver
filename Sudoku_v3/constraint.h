# pragma once

#include<vector>

class Grid;

struct Position {
    int row;
    int column;
};

class Constraint {
    public:
        virtual std::vector<std::pair<int, int>> get_positions(int row, int column) = 0;
        virtual void change_candidates(Grid& grid, int row, int column, int value) = 0;
        virtual ~Constraint() = default;
};

class RowConstraint : public Constraint {
    public:
        std::vector<std::pair<int, int>> get_positions(int row, int column) override;
        void change_candidates(Grid& grid, int row, int column, int value) override ;
};

class ColumnConstraint : public Constraint {
    public:
        std::vector<std::pair<int, int>> get_positions(int row, int column) override;
        void change_candidates(Grid& grid, int row, int column, int value) override ;
};

class BoxConstraint : public Constraint {
    public:
        std::vector<std::pair<int, int>> get_positions(int row, int column) override;
        void change_candidates(Grid& grid, int row, int column, int value) override ;
};