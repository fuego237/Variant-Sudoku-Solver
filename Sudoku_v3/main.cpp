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
    return 0;
}

    