# Ubistar
A* algorithm implementation

## A* Exercise
===========

## Objective
---------

The objective of the exercise is to program an A* path finder in C++.

- The map is a 126 x 126 grid.
- Movement is possible in 8 directions (NW, N, NE, ...). The cost of movement depends on the terrain type (see below).
- The A* algorithm must be used.
- The executable must be a console application.
- The executable must support the following command line paramters:
  astar.exe MapFileName StartX StartY EndX EndY [showmap]
- Input and output coordinates are 0-based.
- The executable must output the result exactly as specified below (see Output specification)
- The map file "MapFileName" is a text file. (see Map file specification below)
- If "showmap" is specified and a path was found the map including the 
  calculated path must be printed.
- The implementation must produce optimal path for any input (path cost should be
  equal to optimal with a tolerance of 0.001).

- Provide a Visual Studio solution file (VS 2015 or 2017) when possible.


## Bonus objective
---------------
- Optimize your implementation for fast execution speed


## Map file specification
----------------------
There are 126 lines, each is 126 characters long.
Each character represents a specific terrain type:
- '.' represents "plain", the cost of moving across plains is 1.0
- '*' represents "water", you can't move across water
- '-' represents "swamps", the cost of moving across swamps is 1.5
- '^' represents "mountains", the cost of moving through mountains is 2.0

See the AStarMap.txt file for an example.

NOTE: the cost depends only on the destination node;
e.g. moves "plain->mountain" and "mountain->mountain" both have a cost of 2.0.
Also, keep in mind that the cost must be multiplied by the geographical distance, the distance
for straight (N, E, W, S) move is 1.0, for diagonal (NW, NE, SW, SE) is sqrt(2).


## Output specification
--------------------

The output should follow this format exactly (see examples below):

Start position: (<StartX>, <StartY>)
End position:   (<EndX>, <EndY>)

Path found:     { true | false }
Path cost:      <path cost>

Total duration: <run duration> ms

My new change
