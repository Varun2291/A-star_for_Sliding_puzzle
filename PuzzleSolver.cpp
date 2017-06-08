#include "Puzzle/PuzzleSolver.hpp"
#include<time.h>
#include<iostream>
#include<string.h>
using namespace std;

void printBoard(Puzzle::Board *b)
{
	for(int i = 8; i >= 0; i--)
		printf("%d", b->GetValue(i));
	printf("\n");
} 

// Function to Initialize the parent node
void PuzzleSolver::Initialize(Puzzle *puzzle, PuzzleHeuristic *h, Puzzle::Board *board)
{
	parentNode = new node();
	parentNode->board_config = std::move(board); 
	parentNode->parentNode = nullptr;
	parentNode->g_value = 0;
	parentNode->f_value = h->Evaluate(puzzle, board);  
	parentNode->move = -1;
}

// Function to compare two boards; return true if they are equal
bool PuzzleSolver::CompareBoards(node* const board1, node* const board2, Puzzle *puzzle)
{
	// Compare their strings to check if they are equal
	string board1_value = "", board2_value = "";
	char temp1[8] = "", temp2[8] = "";
	
	// The board contains value in integer data type;
	// Convert and append to strings for each board
	for(int i = ((puzzle->GetNrSquares())-1); i >= 0; i--)
	{
		strcpy(temp1,"");
		itoa((board1->board_config->GetValue(i)), temp1, 10); 
		board1_value.append(temp1);

		strcpy(temp2,"");
		itoa((board2->board_config->GetValue(i)), temp2, 10);
		board2_value.append(temp2);
	}

	if(board1_value.compare(board2_value) == 0)
		return true;
	else
		return false;
}

// Function to create a copy of the passed node
PuzzleSolver::node* PuzzleSolver::CreateCopy(node* node1, Puzzle *puzzle)
{
	node* newCopyTemp = new node();
	newCopyTemp->board_config = puzzle->CopyBoard(node1->board_config);
	newCopyTemp->parentNode = node1->parentNode;
	newCopyTemp->f_value = node1->f_value;
	newCopyTemp->g_value = node1->g_value;
	newCopyTemp->move = node1->move;

	return newCopyTemp;
}

// Function to create a Child Node
PuzzleSolver::node* PuzzleSolver::CreateChildNode(node* parent, Puzzle *puzzle)
{
	node* child = new node();

	child = CreateCopy(parent, puzzle);
	child->parentNode = parent;
	child->g_value += 1;

	return child;
}

std::vector<PuzzleSolver::node*> PuzzleSolver::GetChildNode(node* parentNode1, Puzzle *puzzle, PuzzleHeuristic *h)
{
	std::vector<node*> ChildrenNodes;
	node* go_left = new node(), *go_right, *go_up, *go_down;

	// Push the blank (0) to left
	go_left = CreateChildNode(parentNode1, puzzle);
	puzzle->MakeMove(go_left->board_config, static_cast<Puzzle::Move>(0));
	go_left->f_value = go_left->g_value + h->Evaluate(puzzle, go_left->board_config);
	if(!CompareBoards(go_left, parentNode1, puzzle))
	{	
		go_left->move = 0;
		ChildrenNodes.push_back(go_left);
		//cout<<"go_left: "<<go_left->f_value<<"-> "; printBoard(go_left->board_config);
	}

	// Push the blank (0) to right
	go_right = CreateChildNode(parentNode1, puzzle);
	puzzle->MakeMove(go_right->board_config, static_cast<Puzzle::Move>(1));
	go_right->f_value = go_right->g_value + h->Evaluate(puzzle, go_right->board_config);
	if(!CompareBoards(go_right, parentNode1, puzzle))
	{
		go_right->move = 1;
		ChildrenNodes.push_back(go_right);
		//cout<<"go_right: "<<go_right->f_value<<"-> "; printBoard(go_right->board_config);
	}

	// Push the blank (0) to Up
	go_up = CreateChildNode(parentNode1, puzzle);
	puzzle->MakeMove(go_up->board_config, static_cast<Puzzle::Move>(2));
	go_up->f_value = go_up->g_value + h->Evaluate(puzzle, go_up->board_config);
	if(!CompareBoards(go_up, parentNode1, puzzle))
	{
		go_up->move = 2;
		ChildrenNodes.push_back(go_up);
		//cout<<"go_up: "<<go_up->f_value<<"-> "; printBoard(go_up->board_config);
	}

	// Push the blank (0) to Down
	go_down = CreateChildNode(parentNode1, puzzle);
	puzzle->MakeMove(go_down->board_config, static_cast<Puzzle::Move>(3));
	go_down->f_value = go_down->g_value + h->Evaluate(puzzle, go_down->board_config);
	if(!CompareBoards(go_down, parentNode1, puzzle))
	{
		go_down->move = 3;
		ChildrenNodes.push_back(go_down);
		//cout<<"go_down: "<<go_down->f_value<<"-> "; printBoard(go_down->board_config);
	}
	
	return ChildrenNodes;
}

// Function to Store the moves after choosing the lowest f value board
void StoreMoves(int MoveType, std::vector<Puzzle::Move> *moves)
{
	switch(MoveType)
	{
	case 0: // Moved left
		moves->push_back(static_cast<Puzzle::Move>(0));
		break;

	case 1: // Moved Right
		moves->push_back(static_cast<Puzzle::Move>(1));
		break;

	case 2: // Moved Up
		moves->push_back(static_cast<Puzzle::Move>(2));
		break;

	case 3: // Moved Down
		moves->push_back(static_cast<Puzzle::Move>(3));
		break;
	}
}

// Function to trace the root node from the current node 
void PuzzleSolver::GetPathToRoot(node* Final, std::vector<Puzzle::Move> *moves)
{
	std::vector<int> OrderedPath;
	node* currentNode = Final;

	// Iterate to the root node and store all the moves
	while(currentNode != NULL)
	{
		OrderedPath.push_back(currentNode->move);
		currentNode = currentNode->parentNode;
	}

	// Iterating from the end to get the correct moves
	// -2 is because; the root node is not considered
	for(int iter = (OrderedPath.size()-2); iter >= 0; --iter)
	{
		StoreMoves(OrderedPath.at(iter), moves);
	}
}

// Function to check if the board is solvable using inversion
bool IsBoardSolvable(Puzzle::Board *board, Puzzle *puzzle)
{
	int cur_value;
	int inversion = 0; 
	for(int i = 0; i < puzzle->GetNrSquares(); ++i)
	{
		cur_value = board->GetValue(i);
		for(int j = (i+1); j < puzzle->GetNrSquares(); ++j)
		{
			if((cur_value > board->GetValue(j)) && (board->GetValue(j) != 0))
			{
				inversion++;
			}
		}
	}

	if((inversion % 2) == 0)
		return true;
	else
		return false;
}

// Function to solve the given puzzle using A* search
bool PuzzleSolver::Solve(Puzzle *puzzle, 
			 PuzzleHeuristic *h, 
			 Puzzle::Board *board, 
			 const double tmax, 
			 std::vector<Puzzle::Move> *moves) 
{
	cout<<endl<<"Inside PuzzleSolver.cpp: "<<endl;

	int Start_time, Stop_time;	// Variables to calculate the time
	double total_time = 0.0;	// Variable to store the total time taken

	// Call Initialize function to set the appropriate or initial values
	Initialize(puzzle, h, board);
	
	std::vector<node*> ClosedList;		// Variable to store the Visited Nodes (Board)
	std::vector<node*> OpenList;		// Variable to store the Unvisited Nodes (Board)

	OpenList.push_back(CreateCopy(parentNode, puzzle));			// Store the first node in the open list

	if(!IsBoardSolvable(board, puzzle))
	{
		cout<<endl<<"The Board is not solvable"<<endl;
		return false; 
	}

	while((!OpenList.empty()) && (total_time < tmax))
	{
		Start_time = clock();	// Get the start time

		// Get the board with the lowest F-Value
		std::vector<node*>::iterator current_node = OpenList.begin();
		for(auto i = OpenList.begin(); i != OpenList.end(); ++i)
		{
			if((*i)->f_value < (*current_node)->f_value)
			{
				current_node = i;
			}
		}
	
		if(puzzle->IsSolved((*current_node)->board_config) == true)
		{
			// The Goal has reached, return the moves performed till now

			GetPathToRoot(*current_node, moves);
			cout<<endl<<"Finally Reached Destination"<<endl;
			cout<<"Time Taken: "<<total_time<<endl;
			return true;
		}
	//	cout<<"Parent: ";printBoard((*current_node)->board_config);cout<<endl;
		std::vector<node*> childNode = GetChildNode(*current_node, puzzle, h);

		// After getting the children; Move the parent to the Closed list 
		// and remove it from the open list
		ClosedList.push_back(*current_node);		// Add to the end of the list i.e. Visited
		OpenList.erase(current_node);				// Remove from the unvisited list
		
		// Add the child node to the Open list only if it has not been visited
		// Otherwise dicard that node
		bool DuplicateBoard_OpenList = false;
		bool DuplicateBoard_ClosedList = false;
		for(auto it = childNode.begin(); it != childNode.end(); ++it)
		{
			DuplicateBoard_OpenList = false;
			DuplicateBoard_ClosedList = false;
		
			// Check if Child node is present in the Open List
			for(auto jt = OpenList.begin(); jt != OpenList.end(); ++jt)
			{
				// Compare the each new child node with the 
				if(CompareBoards((*it), (*jt), puzzle))
				{
					DuplicateBoard_OpenList = true;
					break;					// Breaks out of the first loop
				}
			}

			// Check if Child node is present in the Closed List
			for(auto kt = ClosedList.begin(); kt != ClosedList.end(); ++kt)
			{
				// Compare the each new child node with the 
				if(CompareBoards((*it), (*kt), puzzle))
				{
					DuplicateBoard_ClosedList = true;
					break;					// Breaks out of the first loop
				}
			}
			
			// Add to the Open list if the child node is unique
			if((DuplicateBoard_OpenList == false) && (DuplicateBoard_ClosedList == false))
			{
				OpenList.push_back(*it);
			}
		}

		Stop_time = clock();	// Get the stop time
		total_time += (float(Stop_time - Start_time) / CLOCKS_PER_SEC);	// Calculate the total time in seconds
	}

	// Return false if it did not find a solution
    return false;
}
