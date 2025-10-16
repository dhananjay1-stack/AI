#include <iostream>
#include <vector>
using namespace std;
const int N = 9;

void printGrid(int grid[N][N]) {
    cout << "-------------------------\n";
    for (int row = 0; row < N; ++row) {
        cout << "| ";
        for (int col = 0; col < N; ++col) {
            cout << grid[row][col] << " ";
            if ((col + 1) % 3 == 0) {
                cout << "| ";
            }
        }
        cout << "\n";
        if ((row + 1) % 3 == 0) {
            cout << "-------------------------\n";
        }
    }
}


bool isSafe(int grid[N][N], int row, int col, int num) {
    
    for (int x = 0; x < N; ++x) {
        if (grid[row][x] == num) {
            return false;
        }
    }

    
    for (int x = 0; x < N; ++x) {
        if (grid[x][col] == num) {
            return false;
        }
    }

    
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (grid[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    
    
    return true;
}


bool solveSudoku(int grid[N][N]) {
    int row, col;
    bool isEmpty = false;

    
    for (row = 0; row < N; ++row) {
        for (col = 0; col < N; ++col) {
            if (grid[row][col] == 0) {
                isEmpty = true;
                break;
            }
        }
        if (isEmpty) {
            break;
        }
    }

    if (!isEmpty) {
        return true;
    }

    
    for (int num = 1; num <= 9; ++num) {
        
        if (isSafe(grid, row, col, num)) {
            
            grid[row][col] = num;

            if (solveSudoku(grid)) {
                return true; 
            }

           
            grid[row][col] = 0;
        }
    }

    return false;
}

int main() {
    int grid[N][N];

    cout << "🔢 Welcome to the Sudoku Solver! 🔢\n";
    cout << "Please enter the Sudoku puzzle row by row.\n";
    cout << "Use '0' for empty cells and separate numbers with spaces.\n\n";

    for (int i = 0; i < N; ++i) {
        cout << "Enter row " << i + 1 << ": ";
        for (int j = 0; j < N; ++j) {
            cin >> grid[i][j];
        }
    }

    cout << "\nInput Puzzle:\n";
    printGrid(grid);

    if (solveSudoku(grid)) {
        cout << "\nSolution Found:\n";
        printGrid(grid);
    } else {
        cout << "\n No solution exists for the given puzzle.\n";
    }

    return 0;
}