# 8-Puzzle-Solver
An implementation of the A* and the Greedy search algorithm to solve the 8-puzzle problem.

# 8-Puzzle Solver

This is a C++ implementation of an 8-puzzle solver using two search algorithms: Greedy Search and A* Search. The program allows users to input their initial state for the 8-puzzle, and then it finds the solution path, solution cost, and number of expanded nodes using the selected search algorithm.

## Table of Contents
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Search Algorithms](#search-algorithms)
- [Example Input](#example-input)
- [Contributing](#contributing)
- [License](#license)

## Getting Started

To deploy and run this code, follow the steps below:

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/8-puzzle-solver.git
   ```

2. Change into the project directory:

   ```bash
   cd 8-puzzle-solver
   ```

3. Compile the C++ code:

   ```bash
   g++ main.cpp -o puzzle_solver
   ```

4. Run the executable:

   ```bash
   ./puzzle_solver
   ```

## Usage

1. Enter the initial state of the puzzle. Use `0` for the blank space.

2. Choose the search algorithm:
   - Enter `1` for Greedy Search
   - Enter `2` for A* Search

3. Choose the heuristic function:
   - Enter `1` for Manhattan Distance
   - Enter `2` for Number of Misplaced Tiles

4. Wait for the algorithm to find the solution path.

5. View the solution path, solution cost, and number of expanded nodes.

## Search Algorithms

The program supports two search algorithms:

- **Greedy Search**: Uses the heuristic function as the cost function (`f(n) = h(n)`).

- **A* Search**: Uses the sum of the depth and heuristic function as the cost function (`f(n) = g(n) + h(n)`).

## Example Input

For example, if the initial state of the puzzle is:

```
1 2 3
5 6 0
7 8 4
```

The program will find the solution path using the selected search algorithm and heuristic function.

## Contributing

Feel free to contribute to this project by opening issues or submitting pull requests. Your feedback and suggestions are highly appreciated.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
