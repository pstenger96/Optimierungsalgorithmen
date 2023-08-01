# Optimierungsalgorithmen

This repository tries to solve an optimization problem; fit a number of rectangles with random edge sizes (< L) into quadratic boxes of edge length L and minimize the number
of boxes. 
For this several optimization algorithms are implemented, namely:
- Local Search - RuleBased
- Local Search - Geometry Based
- Local Search - Geometry Based with potential Overlapping during solving
- Greedy Search - Area Sorting
- Greedy Search - Diagonal Sorting

Local Search and Greedy Search are implemented such that they can be easily applied to any other optimization problem given a defined neighbourhood (including an initial solution) and a DataHolder fitting the problem.  

In UIConstants the GUI can be turned off, then the TestEnviornment is used instead and the results are written into a .csv file. 

