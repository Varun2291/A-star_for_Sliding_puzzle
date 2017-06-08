#ifndef PUZZLE_SOLVER_HPP_
#define PUZZLE_SOLVER_HPP_

#include "Puzzle/Puzzle.hpp"
#include "Puzzle/PuzzleHeuristic.hpp"
#include <vector>

class PuzzleSolver
{
public:
	struct node{
	double f_value;
	int g_value;
	int move;		// 0 - left, 1 - right, 2 - up, 3 - down

	Puzzle::Board *board_config;
	node* parentNode;
	};
	node* parentNode;

    PuzzleSolver(void)
    {
    }
    
    virtual ~PuzzleSolver(void)
    {
    }

    //you should implement this function   
    //board is the start configuration    
    //return true if solution found; false otherwise
    //add the solution moves to the moves vector
    virtual bool Solve(Puzzle *puzzle, 
		       PuzzleHeuristic *h, 
		       Puzzle::Board *board, 
		       const double tmax, 
		       std::vector<Puzzle::Move> *moves);
	virtual void Initialize(Puzzle *puzzle, PuzzleHeuristic *h, Puzzle::Board *board);
	virtual std::vector<node*> GetChildNode(node* parentNode, Puzzle *puzzle, PuzzleHeuristic *h);
	bool CompareBoards(node* board1, node* board2, Puzzle *puzzle);
	node* CreateCopy(node* node1, Puzzle *puzzle);
	node* CreateChildNode(node* parent, Puzzle *puzzle);
	void GetPathToRoot(node* current_node, std::vector<Puzzle::Move> *moves);
};



#endif
    
