#pragma once

#include<vector>
#include<set>
#include<utility>
#include<stack>
#include<string>
#include<memory>
#include<iostream>
#include<unordered_set>
#include<algorithm>

#include"position.h"

class Constraint;

struct GridChange {
    Position position;
    int value;
};

class Grid {
    public:
        std::vector<std::vector<int>> data{9, std::vector<int>{9}};
        std::vector<std::vector<std::set<int>>> candidates{9, std::vector<std::set<int>>(9, std::set<int>{1,2,3,4,5,6,7,8,9})};
        std::stack<GridChange> change_stack;
        std::vector<Position> position_stack;
        std::vector<std::unique_ptr<Constraint>> constraints;
        bool solvable;
        bool propagation_flag;

        Grid (std::string str, std::vector<std::unique_ptr<Constraint>> cons);

        void removeCandidate(Position pos, int value);
        void enforce(Constraint& constraint, Position pos, int value);
        void propagate(Position pos, int value);
        Position find_empty();
        void solve();
        void set_candidates();
        void full_solve();
        void print_grid();
};