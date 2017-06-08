#include "Puzzle/PuzzleHeuristic1.hpp"

#include<iostream>
using namespace std;

// Misplaced tiles Heuristics - Hamming Distance
double PuzzleHeuristic1::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
	Misplaced_Distance = 0;
	for(int i = ((puzzle->GetNrSquares())-1); i > 0; i--)
		if((board->GetValue(i) != i))
			Misplaced_Distance += 1;
    return Misplaced_Distance;
}

