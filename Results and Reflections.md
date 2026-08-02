As a beginner to C++, I believe the process of writing different versions and incrementally improving my code helped me understand concepts like time/space complexity and backtracking.

To test runtime, I used a NYT hard level sudoku. For version 4, I also tested a variant sudoku that incorporates all 3 added constraints. 
These puzzles can be found in the "Example Grids" folder.
The variant sudoku can also be found [here](https://sudokupad.app/i0lz556ia7) with a full walkthrough [here](https://www.youtube.com/watch?v=Q9kVuyrX7tU&t=15s).

Full runtimes for the basic sudoku are as follows:
- Version 1: 15,296 microseconds
- Version 2: 17,170 microseconds
- Version 3: 17,071 microseconds
- Version 4: 1,909 microseconds

Version 4 solves the variant sudoku in 232,778,243 microseconds (a little under 4 minutes).

Interestingly, adding constraint propagation and autofilling does not seem to speed up the algorithm. 
This may have to do with the cost of storing candidates for every single cell. Additionally, when propagation is added, the program is constantly pulling neighboring positions.
It may also be the case that for a basic sudoku, there are enough cells already given that there is not much room for optimization.
I would like to do some more research on time complexity to understand why we don't see a major decrease in solve time from Version 1 to Versions 2 and 3.

The real key to cutting solve time is changing the order in which the puzzle is solved. The basic sudoku solver places digits in order from left to right and top to bottom.
This may be fine when solving an easy puzzle when there are already a lot of given cells. However, when there are many empty cells in the grid, the solver may be wasting a lot of time solving and recursing on cells that are not really relevant in the solve path.
I was motivated to fix this issue when I added variant constraints. I noticed for variant puzzles, the amount of empty cells is much higher compared to regular puzzles, and my runtime solving these puzzles in order
was in the magnitude of several hours. This was because the algorithm was starting at the top row that was full of empty cells, when it should really have been solving the puzzle from the most constrained area.
I was able to fix this by sorting each remaining cell by number of candidates and having a vector store this; this becomes a bit complex when autofilling cells, as they may have to be deleted from the middle of the vector,
but the added complexity is more than made up for by a faster runtime.

Overall, I enjoyed this project as an introduction to C++. I got to learn about different topics in my implementation: OOP, polymorphism, header vs source files in a multifile program,
storing custom class instances in a vector, using position stacks for backtracking, etc. I also had to think deeply about software design. What methods should I give the grid class and the constraint class? 
How do I allocate responsibilities to different classes? How should I design the constraint interface in a way that makes it simple for another person to add their own constraint? 
These are all questions I had to think deeply on.

Beyond this, I spent a lot of time debugging the program in VSCode. I became more comfortable handling C++ error messages and understanding possible issues. Coming from Python, where things like type handling are already done 
under the hood, C++ was quite intimidating in this regard. However, with the experience from this project, I definitely feel like I am more prepared to handle bugs and improve my code.

As a final note, I would like to shout out the Youtube channel [Cracking the Cryptic](https://www.youtube.com/@CrackingTheCryptic). This channel first got me interested in Variant Sudoku,
and they do a wonderful job of both solving insanely hard puzzles and explaining difficult concepts to beginners. 

Some areas of further research:
- Adding additional constraints (killer cages, region sum lines, etc.)
- Given a set of constraints, generating a custom puzzle (this seems a little more involved!)
- Testing whether a starting grid is valid. In my code, I check whether a solution exists, but I assume that if it does, it is the only solution. Extending the code to check for multiple solutions may be helpful to someone testing their own puzzle.

