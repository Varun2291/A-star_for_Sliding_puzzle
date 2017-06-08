#ifndef PUZZLE_HEURISTIC_HPP_
#define PUZZLE_HEURISTIC_HPP_

#include "Puzzle/Puzzle.hpp"

class PuzzleHeuristic
{
public:
	double Misplaced_Distance;
    PuzzleHeuristic(void) 
    {
    }
    
    virtual ~PuzzleHeuristic(void)
    {
    }
    
    virtual double Evaluate(Puzzle *puzzle, Puzzle::Board *board)
    {
	return 0.0;
    }
};

#endif
