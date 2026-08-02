This file gives a brief overview of the variant sudoku rules I've added as constraints.

### German Whispers

The German Whispers constraint is a line constraint where neighboring cells must differ by more than 5. 
Importantly, this means 5 can never be on a German Whispers line, as every single digit from 1-9 is within 5 of the digit 5.
Additionally, if a cell is in the middle of a line and both of its neighbors relate to each other (i.e. they are in the same box, column, or row),
then the cell cannot have 4 or 6. This is because 4 and 6 only have one possible neighbor on a German Whispers line (9 and 1, respectively).
Eliminating these candidates is the key to solving German Whispers sudokus.

### Renban Lines
Renban Lines are lines on which digits must be consecutive, but not necessarily in order. For example, 6 9 8 7 is an example of a 4 cell Renban line. 
The length of the line is essential as it may force certain digits to be on the line. For example, a 6 cell Renban line forces digits 4,5, and 6
to all be on the line, as any set of 6 consecutive digits contains those numbers.

### Thermometers
Thermometers are line constraints where order matters; digits on thermometers must increase from the bulb. Here, digits 1 and 9 are important to cutting down candidates.
1 can never be on the end of a constraint, and 9 can never be at the beginning. The longer the line is, the more constrained the cells are. 
For example, a 7 cell thermometer only has 3 possible candidates for each cell.

