#include "Puzzle/PuzzleHeuristic2.hpp"

#include<iostream>
using namespace std;

// Function to calculate Manhattan Distance
double PuzzleHeuristic2::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
	int man_distance = 0;
	int NR = puzzle->GetNrRows();
	int NC = puzzle->GetNrCols();
	int id, val;

	for(int r = 0; r < NR; r++)
	{
		for(int c = 0; c < NC; c++)
		{
			id = puzzle->IdFromRowCol(r,c);
			val = board->GetValue(id);
			
			// Calculate the Manhattan Distance
			if(id < (puzzle->GetNrSquares()) && (val != 0))
				man_distance += abs(r-(val / NR)) + abs(c-(val % NC));
		}
	}
    return man_distance;
}

