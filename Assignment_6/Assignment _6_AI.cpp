#include <iostream>
#include <vector>
#include <cmath> // For std::abs

// Define the size of the chessboard
const int N = 8;

// A counter for the number of solutions found
int solutionCount = 0;

/*
 * Function to print the chessboard configuration.
 * It takes a vector where the index represents the column
 * and the value at that index represents the row of the queen.
 */
void printSolution(const std::vector<int>& board) {
    std::cout << "Solution " << ++solutionCount << ":\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[j] == i) {
                // If a queen is at (row i, col j)
                std::cout << "Q ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n-----------------\n\n";
}

/*
 * Function to check if it's safe to place a queen at a given position.
 * We only need to check columns to the left of the current column,
 * as columns to the right are still empty.
 * board: The current board configuration.
 * row: The row to check.
 * col: The column to check.
 */
bool isSafe(const std::vector<int>& board, int row, int col) {
    // Check all previously placed queens (in columns 0 to col-1)
    for (int i = 0; i < col; ++i) {
        // 1. Check if a queen is in the same row
        if (board[i] == row) {
            return false;
        }

        // 2. Check for diagonal attacks.
        // Two queens are on the same diagonal if the absolute difference
        // of their row and column numbers is the same.
        if (std::abs(board[i] - row) == std::abs(i - col)) {
            return false;
        }
    }
    // If no attacks are found, the position is safe
    return true;
}

/*
 * The main recursive function to solve the N-Queens problem using backtracking.
 * board: The current board configuration.
 * col: The current column we are trying to place a queen in.
 */
void solveNQueens(std::vector<int>& board, int col) {
    // Base case: If all queens have been placed successfully (i.e., we've filled all columns)
    if (col >= N) {
        printSolution(board);
        return; // Return to find other solutions
    }

    // Recursive step: Try placing a queen in each row of the current column
    for (int i = 0; i < N; ++i) {
        // Check if placing a queen at (row i, col) is safe
        if (isSafe(board, i, col)) {
            // If it's safe, place the queen
            board[col] = i;

            // Recur to place the rest of the queens in the next column
            solveNQueens(board, col + 1);

            // Backtrack: If the recursive call doesn't lead to a solution,
            // this position is implicitly undone by the loop trying the next row 'i'.
        }
    }
}

int main() {
    // The 'board' vector stores the row number for each queen.
    // The index of the vector represents the column number.
    // For example, board[3] = 5 means a queen is at column 3, row 5.
    std::vector<int> board(N);

    std::cout << "Finding all solutions for the 8-Queens problem...\n\n";
    
    // Start the recursive search from the first column (column 0)
    solveNQueens(board, 0);

    if (solutionCount == 0) {
        std::cout << "No solution exists.\n";
    } else {
        std::cout << "Found a total of " << solutionCount << " solutions.\n";
    }

    return 0;
}