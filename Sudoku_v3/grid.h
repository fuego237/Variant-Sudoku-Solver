#pragma once

#include<vector>
#include<set>
#include<utility>
#include<stack>
#include<string>
#include<memory>
#include<iostream>

class Constraint;

struct GridChange {
    std::pair<int, int> position;
    int value;
};

class Grid {
    public:
        std::vector<std::vector<int>> data{9, std::vector<int>{9}};
        std::vector<std::vector<std::set<int>>> candidates{9, std::vector<std::set<int>>(9, std::set<int>{1,2,3,4,5,6,7,8,9})};
        std::stack<GridChange> change_stack;
        std::vector<std::unique_ptr<Constraint>> constraints;
        bool solvable;

        Grid (std::string str, std::vector<std::unique_ptr<Constraint>> cons);

        void removeCandidate(int row, int column, int value);
        void enforce(Constraint& constraint, int row, int column, int value);
        void propagate(int row, int column, int value);
        int find_empty(int index);
        void solve(int index);
        void set_candidates();
        void full_solve();
        void print_grid();
};