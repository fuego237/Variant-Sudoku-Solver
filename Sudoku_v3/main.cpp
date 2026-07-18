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
    std::string str_grid = "0,0,0 | 0,0,2 | 1,0,4"
                           "0,0,8 | 0,0,1 | 0,0,3"
                           "5,0,0 | 0,6,0 | 0,9,0"
                           "_____________________"
                           "0,9,0 | 0,8,0 | 0,4,6"
                           "6,0,0 | 7,0,0 | 0,0,0"
                           "1,0,0 | 0,0,0 | 0,8,0"
                           "_____________________"
                           "0,3,7 | 2,0,0 | 0,1,9"
                           "0,0,0 | 0,0,0 | 0,3,0"
                           "0,0,0 | 0,9,0 | 0,0,0";

    std::vector<std::unique_ptr<Constraint>> constraints;

    // Adding polymorphic objects
    constraints.emplace_back(std::make_unique<RowConstraint>());
    constraints.emplace_back(std::make_unique<ColumnConstraint>());
    constraints.emplace_back(std::make_unique<BoxConstraint>());

    Grid grid1(str_grid, std::move(constraints));

    std::cout << "Grid created" << std::endl;
    grid1.full_solve();
    grid1.print_grid();
    return 0;
}

    