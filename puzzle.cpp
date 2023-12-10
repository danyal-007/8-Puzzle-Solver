#include <iostream>
#include <ctime>

using std::cin;
using std::cout;
using std::string;

// global final variable
#define ROW_COL 3
const unsigned int row = 3, col = 3;

// measure the running time
double cpuTimeUsed;
clock_t start, end;

// versatile global variable(puzzle)
unsigned int counter, x, y, i, cost;

/*** OBJECT tile, to keep track of a tile in PUZZLE ***/
class TILE
{
public:
    unsigned int X;
    unsigned int Y;

    TILE(){};
    ~TILE(){};

    void set(const unsigned int &_X, const unsigned int &_Y)
    {
        this->X = _X;
        this->Y = _Y;
    }
};

class DIRECTION
{
public:
    string move;

    void setDirection(const char &move)
    {
        if (move == 'U')
            this->move = "UP";
        if (move == 'L')
            this->move = "LEFT";
        if (move == 'D')
            this->move = "DOWN";
        if (move == 'R')
            this->move = "RIGHT";
        else
            this->move = "START";
    }

    DIRECTION(){};
    ~DIRECTION(){};
};

enum HeuristicChoice
{
    MANHATTAN_DISTANCE = 1,
    MISPLACED_TILES = 2
};
enum AlgorithmChoice
{
    GREEDY = 1,
    ASTAR = 2
};

int getUserAlgoChoice()
{
    int choice;
    cout << "\n\t=================================================================================\n";
    cout << "\t**** Choose Search Algorithm ****\n";
    cout << "1. Greedy Search\n";
    cout << "2. A* Search\n";
    cout << ">> Enter your choice (1 or 2): ";
    cin >> choice;
    cout << "\n\t=================================================================================\n";

    return choice;
}

int getUserHeuristicChoice()
{
    int choice;
    cout << "\n\t=================================================================================\n";
    cout << "\t**** Choose heuristic function **** \n";
    cout << "1. Manhattan Distance\n";
    cout << "2. Number of Misplaced Tiles\n";
    cout << ">> Enter your choice (1 or 2): ";
    cin >> choice;
    cout << "\n\t=================================================================================\n";

    return choice;
}

/*** OBJECT puzzle ***/
class PUZZLE
{
    // The Goal State
    const unsigned int goalState[ROW_COL][ROW_COL] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

    int *getLoc(int &_row, int &_col, const unsigned int board[][ROW_COL])
    {
        static int location[2];
        for (x = 0; x < row; x++)
        {
            for (y = 0; y < col; y++)
            {
                if (board[x][y] == goalState[_row][_col])
                {
                    location[0] = x;
                    location[1] = y;
                }
            }
        }
        return location;
    }

public:
    PUZZLE *parent;                             // for backtracking the parent node of this current puzzle
    unsigned int board[ROW_COL][ROW_COL] = {0}; // puzzle object must have a board
    unsigned int depth;                         // depth attribute that will be used in A* Cost Calculations
    TILE blank;                                 // a puzzle must have a coordinate(puzzle) of where the blank tile is
    DIRECTION direction;                        // board move (UP, DOWN, LEFT, RIGHT)

    /**
     * @note - this is a Contructor that used to Construct a new PUZZLE object
     *       - this function will set a state and set the blank tile for that state
     *       - set heuristic to (-1) to determine if it is not yet calculated
     *
     * @param state
     * @return PUZZLE*
     */
    PUZZLE(){};

    PUZZLE(unsigned int puzzle[][ROW_COL])
    {
        for (x = 0; x < row; x++)
        {
            for (y = 0; y < col; y++)
            {
                if (puzzle[x][y] == 0)
                    this->blank.set(x, y);
                this->board[x][y] = puzzle[x][y];
            }
        }
    };

    /**
     * @note  - we also provided a deconstructor...
     *          Destructor - is an instance member function which is invoked
     *          automatically whenever an object is going to be destroyed.
     *          Meaning, a destructor is the last function that is going to
     *          be called before an object is destroyed.
     */
    ~PUZZLE()
    {
        delete parent;
    };

    // PUZZLE functions, when a puzzle object is created, it can use this functionalities (implemented in public)
    bool isGoal() const
    {
        for (x = 0; x < row; x++)
        {
            for (y = 0; y < col; y++)
            {
                if (board[x][y] != goalState[x][y])
                    return false;
            }
        }
        return true;
    }

    void display() const
    {
        for (x = 0; x < row; x++)
        {
            cout << "\t                                                  ";
            for (y = 0; y < col; y++)
                cout << "+---";
            cout << "+\n";

            cout << "\t                                                  ";
            for (y = 0; y < col; y++)
            {
                cout << "| ";
                if (this->board[x][y] == 0)
                    cout << " ";
                else
                    cout << this->board[x][y];
                cout << " ";
            }
            cout << "|\n";
        }

        cout << "\t                                                  ";
        for (y = 0; y < col; y++)
            cout << "+---";
        cout << "+\n";
        cout << "\t                                                  "
             << "   " << this->direction.move << "\n";
    }

    // Function to calculate the number of misplaced tiles heuristic
    int h1(int AlgorithmChoice)
    {
        int miss = 0;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (goalState[i][j] != 0 && board[i][j] != goalState[i][j])
                {
                    miss++;
                }
            }
        }

        if (AlgorithmChoice==GREEDY){
            // For Greedy Search
            // f(n) = g(n)
            return miss;
            }
        else {
            // For A* Search
            // f(n) = g(n) + h(n)
            return this->depth + miss;
            }
        
    }

    // Function to calculate the Manhattan Distance heuristic
    int h2(int AlgorithmChoice)
    {
        int totalManhattanDistance = 0, row_diff = 0, col_diff = 0;
        int *location = nullptr;

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (goalState[i][j] != 0)
                {
                    location = getLoc(i, j, this->board);
                    row_diff = (i > location[0]) ? i - location[0] : location[0] - i;
                    col_diff = (j > location[1]) ? j - location[1] : location[1] - j;
                    totalManhattanDistance += col_diff + row_diff;
                }
            }
        }
        location = nullptr;
        delete location;
        location = nullptr;
        if (AlgorithmChoice==GREEDY){
            // For Greedy Search
            // f(n) = g(n)
            return totalManhattanDistance;
            }
        else {
            // For A* Search
            // f(n) = g(n) + h(n)
            return this->depth + totalManhattanDistance;
            }
        
    }

    // Function to select heuristic
    int selectHeuristic(int heuristicChoice, int AlgorithmChoice)
    {
        if (heuristicChoice == MISPLACED_TILES){
            h1(AlgorithmChoice);}
        else if (heuristicChoice == MANHATTAN_DISTANCE){
            h2(AlgorithmChoice);}
        else
        {
            // Handle other cases or use a default heuristic
            // In this example, defaulting to Manhattan Distance
            h1(AlgorithmChoice);
        }
    }
    
    /**
     * @note this function is used for checking the state is already in the visited list
     *
     * @param a
     * @param b
     *
     *  compare each state and return if they were equally match or not
     *
     * @return true/false
     */
    bool compare(unsigned int _board[][ROW_COL]) const
    {
        for (x = 0; x < row; x++)
        {
            for (y = 0; y < col; y++)
            {
                if (this->board[x][y] != _board[x][y])
                    return false;
            }
        }
        return true;
    }

    // get total cost for expansion
    int getCost()
    {
        cost = 0;
        while (this->parent != nullptr)
        {
            cost++;
            this->parent = this->parent->parent;
        }
        return cost;
    }

    /**
     * @brief check if move is possible
     *
     * @param state
     * @param direction
     * @return true/false
     */
    bool canMoveUp() const
    {
        if (this->blank.X > 0)
            return true;
        return false;
    }

    bool canMoveLeft() const
    {
        if (this->blank.Y > 0)
            return true;
        return false;
    }

    bool canMoveDown() const
    {
        if (this->blank.X < ROW_COL - 1)
            return true;
        return false;
    }

    bool canMoveRight() const
    {
        if (this->blank.Y < ROW_COL - 1)
            return true;
        return false;
    }

    // puzzle actions
    PUZZLE *moveUp()
    {
        PUZZLE *temp = new PUZZLE(this->board);
        temp->parent = this;
        temp->depth = this->depth + 1;

        temp->direction.move = "Up";
        temp->board[temp->blank.X][temp->blank.Y] = temp->board[temp->blank.X - 1][temp->blank.Y];
        temp->blank.X--;
        temp->board[temp->blank.X][temp->blank.Y] = 0;
        return temp;
    }

    PUZZLE *moveLeft()
    {
        PUZZLE *temp = new PUZZLE(this->board);
        temp->parent = this;
        temp->depth = this->depth + 1;

        temp->direction.move = "Left";
        temp->board[temp->blank.X][temp->blank.Y] = temp->board[temp->blank.X][temp->blank.Y - 1];
        temp->blank.Y--;
        temp->board[temp->blank.X][temp->blank.Y] = 0;
        return temp;
    }

    PUZZLE *moveDown()
    {
        PUZZLE *temp = new PUZZLE(this->board);
        temp->parent = this;
        temp->depth = this->depth + 1;

        temp->direction.move = "Down";
        temp->board[temp->blank.X][temp->blank.Y] = temp->board[temp->blank.X + 1][temp->blank.Y];
        temp->blank.X++;
        temp->board[temp->blank.X][temp->blank.Y] = 0;
        return temp;
    }

    PUZZLE *moveRight()
    {
        PUZZLE *temp = new PUZZLE(this->board);
        temp->parent = this;
        temp->depth = this->depth + 1;

        temp->direction.move = "Right";
        temp->board[temp->blank.X][temp->blank.Y] = temp->board[temp->blank.X][temp->blank.Y + 1];
        temp->blank.Y++;
        temp->board[temp->blank.X][temp->blank.Y] = 0;
        return temp;
    }
};

/*** @note This contains the list of State of Puzzle ***/
class STATE
{
public:
    PUZZLE *state;
    STATE *next;

    STATE(){};
    ~STATE(){};
};

class NODE
{
public:
    STATE *node = nullptr; // an attribute for initializing an empty list of states

    PUZZLE *front()
    {
        PUZZLE *tempNode = nullptr;
        if (node != nullptr)
        {
            tempNode = node->state;
            STATE *temp = node;
            node = node->next;
            delete temp;
        }
        return tempNode;
    }

    /**
     * @brief - use to get the best state
     */
    PUZZLE *bestState(int heuristicChoice, int AlgorithmChoice)
    {
        int minimum = 0;
        STATE *temp = node;
        STATE *previous = nullptr;
        STATE *lowheur = nullptr;
        PUZZLE *best = nullptr;

        if (node->next == nullptr)
        {
            best = node->state;
            delete node;
            node = nullptr;
            return best;
        }
           
        minimum = temp->state->selectHeuristic(heuristicChoice,AlgorithmChoice);
        lowheur = temp;

        while (temp->next != nullptr)
        {
            int f = temp->next->state->selectHeuristic(heuristicChoice,AlgorithmChoice);
            if (f < minimum)
            {
                previous = temp;
                lowheur = temp->next;
                minimum = f;
            }
            temp = temp->next;
        }

        best = lowheur->state;

        if (node != nullptr)
        {
            if (lowheur == node)
                return front();
            if (lowheur->next == nullptr)
                previous->next = nullptr;
            else
                previous->next = lowheur->next;
        }

        delete lowheur;
        return best;
    }


    void insert(PUZZLE *puzzle)
    {
        STATE *tempNode = new STATE;
        tempNode->state = puzzle;
        tempNode->next = nullptr;
        if (node != nullptr)
            tempNode->next = node;
        node = tempNode;
    }


    bool isListed(PUZZLE *state)
    {
        STATE *list = node;
        while (list != nullptr)
        {
            if ((state->compare(list->state->board)))
                return false;
            list = list->next;
        }
        return true;
    }

    NODE(){};
    ~NODE(){};
};

/**
 * @brief - state will create a new state that acceps array which contains tile arrangment
 *        - which also gets and set the state'puzzle blank tile
 *
 *        - set the depth limit
 *        - set move
 *        - and set parent of the state
 *
 * @param board := array from the main function that gets -> (easy, medium, hard, worst, preferred)
 * @return created state
 */
PUZZLE *initialState(unsigned int board[][ROW_COL])
{
    PUZZLE *state = new PUZZLE(board);
    state->depth = 0;
    state->direction.move = "Start";
    state->parent = nullptr;
    return state;
}

void solutionPath(PUZZLE *node)
{
    // If we hit the end of list, we return from there. End of recursion.
    if (node == nullptr)
        return;
    if (node->direction.move[0] != 'S')
    {
        // Move one node forward towards the end of list.
        solutionPath(node->parent);
        // While coming back from the end of list, start printing the node values. Last node will be first one in recursive open.
        std::cout << node->direction.move[0] << " ";
    }
}


/**
 * @note - this will display the path -> puzzle by puzzle
 *
 * @param node
 */
void displayPath(PUZZLE *node)
{
    if (node == nullptr)
        return;
    if (node->direction.move[0] != 'S')
    {
        displayPath(node->parent);
        node->display();
    }
}

/*** Search Algorithms ***/
void Solve_Puzzle(PUZZLE *state, int heuristicChoice, int AlgorithmChoice)
{
    NODE openList, closedList;

    openList.insert(state);

    counter = 0;

    while (openList.node != nullptr)
    {
        
        PUZZLE *puzzle = openList.bestState(heuristicChoice, AlgorithmChoice);
        closedList.insert(puzzle);

        if (puzzle->isGoal())
        {   
            if (AlgorithmChoice==GREEDY){
                cout << "\tGenerating Greedy Search Path...\n\n";
                displayPath(puzzle);
                cout << "\n\t Agent is using Greedy Search...";}

            else{cout << "\tGenerating A* Path...\n\n";
            displayPath(puzzle);
            cout << "\n\t Agent is using A*...";}

            cout << "\n\t Solution Path: ";
            solutionPath(puzzle);
            cout << "\n\t Solution Cost  = " << puzzle->getCost();
            cout << "\n\t Expanded Nodes = " << counter;
            return;
        }

        if (puzzle->canMoveUp())
        {
            if (closedList.isListed(puzzle->moveUp()))
                openList.insert(puzzle->moveUp());
        }

        if (puzzle->canMoveLeft())
        {
            if (closedList.isListed(puzzle->moveLeft()))
                openList.insert(puzzle->moveLeft());
        }

        if (puzzle->canMoveDown())
        {
            if (closedList.isListed(puzzle->moveDown()))
                openList.insert(puzzle->moveDown());
        }

        if (puzzle->canMoveRight())
        {
            if (closedList.isListed(puzzle->moveRight()))
                openList.insert(puzzle->moveRight());
        }

        counter++;
    }
}


int main(int argc, char **argv)
{
    unsigned int user_input = 0, end = 0;
    PUZZLE *init = new PUZZLE();
    // PUZZLE default config
    //unsigned int in_state[][ROW_COL] = {{8, 0, 6}, {5, 4,7}, {2, 3, 1}};
    do
    {
        const int N = 3;
        unsigned int in_state[N][N];
        // Read in user input
        cout << "\n\t=================================================================================\n";
        cout << "**** Enter the initial state of the puzzle (use 0 for the blank space) ****\n";
        cout << "For Example: 1 2 3 5 6 0 7 8 4 \n>> ";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cin >> in_state[i][j];
            }
        }
        unsigned int input[][ROW_COL] = {0};

        init = initialState(in_state);
        init->display();

        // A*
        start = clock();
        int algoChoice = getUserAlgoChoice();
        int heuristicChoice = getUserHeuristicChoice();
        cout << "\n\tPlease Wait while Algorithm Expand Nodes\n";
        Solve_Puzzle(init, heuristicChoice, algoChoice);
        end = clock();
        cpuTimeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;
        cout << "\n\t Running Time   = " << cpuTimeUsed << "\n";

        cout << "\n\t=================================================================================\n";

        cout << "\n\n\t press and enter (0 to continue and 1 to end program): ";
        cin >> end;

    } while (!(end >= 1 && end <= 1));

    delete init;
    init = nullptr;
    return 0;
}