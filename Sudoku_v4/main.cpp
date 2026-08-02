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
#include <memory>

#include "constraint.h"
#include "grid.h"
#include "position.h"

int main() {
    // Basic Sudoku without variant constraints
    std::cout << "Solving Basic Sudoku" << std::endl;
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

    std::vector<std::unique_ptr<Constraint>> constraints;

    // Adding polymorphic objects
    constraints.emplace_back(std::make_unique<RowConstraint>());
    constraints.emplace_back(std::make_unique<ColumnConstraint>());
    constraints.emplace_back(std::make_unique<BoxConstraint>());

    Grid grid1(str_grid, std::move(constraints));
    auto start = std::chrono::steady_clock::now();
    grid1.full_solve();
    auto end = std::chrono::steady_clock::now();
    if (grid1.solvable) {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Solve time: " << duration.count() << " microseconds\n";
        grid1.print_grid();
    }

    // Variant Sudoku with 3 added constraints
    std::cout << "Solving Variant Sudoku" << std::endl;
    str_grid =  "0,0,0 | 0,0,0 | 0,0,0"
                "0,0,0 | 0,0,0 | 0,0,0"
                "0,0,0 | 0,0,0 | 0,0,0"
                "_____________________"
                "0,0,0 | 0,0,0 | 0,0,0"
                "0,0,0 | 0,0,0 | 0,0,0"
                "0,0,0 | 0,0,0 | 0,0,0"
                "_____________________"
                "0,0,0 | 0,0,0 | 0,0,0"
                "0,0,0 | 0,0,0 | 0,0,0"
                "0,0,0 | 0,0,0 | 0,0,0";

    constraints.clear();
    constraints.emplace_back(std::make_unique<RowConstraint>());
    constraints.emplace_back(std::make_unique<ColumnConstraint>());
    constraints.emplace_back(std::make_unique<BoxConstraint>());

    std::vector<std::vector<Position>> gw_lines = {{{1,4},{0,5},{1,6},{2,5}},
                                                {{1,2},{2,3},{3,4},{4,3}},
                                                {{3,5},{3,6},{3,7},{3,8}},
                                                {{8,6},{8,7},{8,8}}};
    std::vector<std::vector<Position>> renban_lines = {{{0,3},{1,3},{2,3}},
                                                {{1,2},{2,1},{3,2},{4,3}},
                                                {{2,2},{3,1},{4,1},{5,1}},
                                                {{3,3},{3,4}},
                                                {{4,5},{4,6},{4,7}},
                                                {{5,6},{6,5}},
                                                {{7,0},{8,0}},
                                                {{6,1},{7,1},{8,1}},
                                                {{6,3},{7,4},{8,5}}};
    std::vector<std::vector<Position>> thermo_lines = {{{2,8},{2,7}}};
    constraints.emplace_back(std::make_unique<GermanWhisperConstraint>(gw_lines));
    constraints.emplace_back(std::make_unique<RenbanConstraint>(renban_lines));
    constraints.emplace_back(std::make_unique<ThermoConstraint>(thermo_lines));

    Grid grid2(str_grid, std::move(constraints));
    start = std::chrono::steady_clock::now();
    grid2.full_solve();
    end = std::chrono::steady_clock::now();
    if (grid2.solvable) {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Solve time: " << duration.count() << " microseconds\n";
        grid2.print_grid();
    }
    return 0;
}

    