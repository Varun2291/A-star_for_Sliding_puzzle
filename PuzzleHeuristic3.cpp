#include "Puzzle/PuzzleHeuristic3.hpp"

#include<iostream>
using namespace std;

//  Function to calculate Linear Conflict
double PuzzleHeuristic3::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
	int distance = 0;
	int NR = puzzle->GetNrRows();
	int NC = puzzle->GetNrCols();
	int id = 0, val = 0;
	
	for(int r = 0;r < NR; r++)
	{
		for(int c = 0; c < NC; c++)
		{
			id = puzzle->IdFromRowCol(r,c);
			val = board->GetValue(id);
			
			// Calculate the Manhattan Distance
			if((id < (puzzle->GetNrSquares())) && (val != 0))
			{
				distance += abs( r - (val / NR)) + abs( c - (val % NC));
			}

			// Check if Linear confict is present for Horizontal
			if(r == (val / NR))
			{
				int id_r = 0, val_r = 0;

				for(int k = (c+1); k < NR; k++)
					id_r = puzzle->IdFromRowCol(r,k);
				
				val_r = board->GetValue(id_r);
				
				// Add 2 to the manhattan distance if there is a Horizontal conflict
				if ( ((val_r / NR) == r) && ((val_r % NC) < (val % NC)))
					distance += 2;
			}

			// Check if Linear confict is present for Vertical
			if(c == (val % NC))
			{
				int id_c = 0, val_c = 0;

				for(int k = (r+1); k < NC; k++)
					id_c = puzzle->IdFromRowCol(k,c);
				
				val_c= board->GetValue(id_c);
				
				// Add 2 to the manhattan distance if there is a Vertical conflict
				if ( ((val_c % NC) == c) && ((val_c / NR) < (val / NR)))
					distance += 2;
			}
		}
	}
    return distance;
}

