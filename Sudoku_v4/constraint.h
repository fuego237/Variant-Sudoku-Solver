# pragma once

#include<vector>
#include<ranges>

#include "position.h"
#include<unordered_map>
class Grid;

class Constraint {
    public:
        virtual void setupGrid(Grid& grid) {};
        virtual std::vector<Position> get_positions(Position pos) = 0;
        virtual void change_candidates(Grid& grid, Position pos, Position current_pos,int value) = 0;
        virtual ~Constraint() = default;
};

class BaseConstraint : public Constraint {
    public:
        void change_candidates(Grid& grid, Position pos, Position current_pos, int value) override ;
        ~BaseConstraint() noexcept override = default;
};

class RowConstraint : public BaseConstraint {
    public:
        std::vector<Position> get_positions(Position pos) override;
};

class ColumnConstraint : public BaseConstraint {
    public:
        std::vector<Position> get_positions(Position pos) override;
};

class BoxConstraint : public BaseConstraint {
    public:
        std::vector<Position> get_positions(Position pos) override;
};

class LineConstraint : public Constraint {
    public:
        std::unordered_map<Position, std::vector<Position>> neighbors;
        LineConstraint(std::vector<std::vector<Position>> lines);
        std::vector<Position> get_positions(Position pos) override;
        //virtual void change_candidates(Grid& grid, Position pos, Position current_pos, int value) = 0; 
        ~LineConstraint() noexcept override = default;
};

class GermanWhisperConstraint : public LineConstraint {
    public:
        using LineConstraint::LineConstraint; 
        void change_candidates(Grid& grid, Position pos, Position current_pos, int value) override ; 
        void setupGrid(Grid& grid) override;
};

class RenbanConstraint : public LineConstraint {
    public:
        RenbanConstraint(std::vector<std::vector<Position>> lines);
        void change_candidates(Grid& grid, Position pos, Position current_pos, int value) override ; 
};

class ThermoConstraint : public LineConstraint {
    public:
        std::unordered_map<Position, std::vector<Position>> bulbs;
        ThermoConstraint(std::vector<std::vector<Position>> lines);
        void change_candidates(Grid& grid, Position pos, Position current_pos, int value) override ; 
        void setupGrid(Grid& grid) override;
};


