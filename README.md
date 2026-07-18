# Variant-Sudoku-Solver
C++ Sudoku Solver with Variant Constraints


Sudoku is a popular puzzle game that tasks players with putting the digits 1-9 in each row, column, and box in an 81 cell grid. Variant sudoku is a game mode of sudoku which adds additional constraints to this base ruleset. For example, the grid may have lines on which numbers must be consecutive or must differ by a certain amount. The grid could have areas that must sum to a certain total. In this project, I improve upon the popular Sudoku Solver by incorporating several new features. I add constraint propagation, autofilling, and 3 popular variant sudoku constraints. I reimagine constraints in an Object-Oriented Programming paradigm, writing a constraint interface with basic methods and subclasses for different types of constraints. This makes it very easy for new constraints to be added. 

A brief summary of my progression:
- Sudoku_v1 (Simple Solver)
    - This program simply iterates through the digits 1 to 9 for every unfilled cell and uses backtracking to arrive at the solution.
    - A grid class maintains the grid as a 2d vector along with methods to read and print the grid, check if cell placements are valid, and recursively solve the puzzle.
- Sudoku_v2 (Enhanced Solver)
    - A candidates attribute is added to the grid class to store the set of possible candidates for each cell.
    - Whenever an empty cell is filled, the action is propagated to remove candidates from adjacent cells. Cells with only one candidate left are autofilled to speed up code execution.
    - A change stack is added to keep track of candidate removals and add candidates back during backtracking.
- Sudoku_v3 (Enhanced Solver with Constraint Interface)
    - Hardcoded row, box, and column constraints are replaced by a constraint interface / abstract class. Since the grid and constraint classes are dependent on one another, this becomes a multifile project.
    - Each constraint has 2 basic methods, one to retrieve a cell's adjacent positions and another one to change a cell's candidate set based on the value of a neighboring cell.
    - The grid class receives an attribute to store constraints and a method to enforce each constraint when filling a cell with a value.
- Sudoku_v4 (Enhanced Solver with Constraint Interface and Variant Constraints)
    - I add new subclasses to the Constraint Interface to incorporate 3 variant sudoku constraints:
        - German Whispers: neighboring cells must differ by 5 or more on a German Whispers line (ex. 6 1 8 3 9)
        - Renban Lines: all cells on a Renban line must have values that are consecutive, but the values do not need to be in order. (ex. 6 9 7 5 8)
        - Thermometers: cells along a thermometer must increase from the bulb. (ex. 1 4 6 8 9, assuming the bulb is on the left end)
    - I add a position struct to make it easier to index cell positions. An equality operator and hashing function are incorporated so that the struct can be used as a map key. 
    - I change the order in which the puzzle is solved. Instead of going in order from left to right, top to bottom, the program sorts all empty positions by the number of possible candidates, choosing cells with the least amount of candidates first. This drastically speeds up runtime, especially for variant constraint heavy grids where many cells may be empty at the beginning of the solve.
