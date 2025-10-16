#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <stack>
#include <limits>
#include <tuple>
using namespace std;

struct Cell {
    
    int parent_x, parent_y;
    
    double f, g, h;

    Cell() : parent_x(-1), parent_y(-1), f(numeric_limits<double>::max()), 
             g(numeric_limits<double>::max()), h(numeric_limits<double>::max()) {}
};


bool isValid(int row, int col, int max_row, int max_col) {
    return (row >= 0) && (row < max_row) && (col >= 0) && (col < max_col);
}


bool isUnblocked(const vector<vector<int>>& grid, int row, int col) {
    return grid[row][col] == 1;
}


bool isDestination(int row, int col, const pair<int, int>& dest) {
    return row == dest.first && col == dest.second;
}


double calculateHValue(int row, int col, const pair<int, int>& dest) {
    return abs(row - dest.first) + abs(col - dest.second);
}


void tracePath(const vector<vector<Cell>>& cellDetails, const pair<int, int>& dest) {
    cout << "\nThe Path is ";
    int row = dest.first;
    int col = dest.second;

    stack<pair<int, int>> Path;

    while (!(cellDetails[row][col].parent_x == row && cellDetails[row][col].parent_y == col)) {
        Path.push({row, col});
        int temp_row = cellDetails[row][col].parent_x;
        int temp_col = cellDetails[row][col].parent_y;
        row = temp_row;
        col = temp_col;
    }

    Path.push({row, col});
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        cout << "-> (" << p.first << "," << p.second << ") ";
    }
    cout << endl;
}


void aStarSearch(const vector<vector<int>>& grid, const pair<int, int>& src, const pair<int, int>& dest) {
    int ROW = grid.size();
    int COL = grid[0].size();
    
    
    if (!isValid(src.first, src.second, ROW, COL) || !isValid(dest.first, dest.second, ROW, COL)) {
        cout << "Source or Destination is invalid\n";
        return;
    }

    
    if (!isUnblocked(grid, src.first, src.second) || !isUnblocked(grid, dest.first, dest.second)) {
        cout << "Source or the destination is blocked\n";
        return;
    }

    
    if (isDestination(src.first, src.second, dest)) {
        cout << "We are already at the destination\n";
        return;
    }
    
    
    vector<vector<bool>> closedList(ROW, vector<bool>(COL, false));
    
    
    vector<vector<Cell>> cellDetails(ROW, vector<Cell>(COL));

    int i, j;
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_x = i;
    cellDetails[i][j].parent_y = j;

    
    priority_queue<tuple<double, int, int>, 
                        vector<tuple<double, int, int>>, 
                        greater<tuple<double, int, int>>> openList;

    openList.push({0.0, i, j});

    while (!openList.empty()) {
        auto [f, x, y] = openList.top();
        openList.pop();

        
        closedList[x][y] = true;

        
        int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};
        double cost[] = {1.0, 1.0, 1.0, 1.0, 1.414, 1.414, 1.414, 1.414};

        for (int k = 0; k < 8; ++k) {
            int newX = x + dx[k];
            int newY = y + dy[k];

            if (isValid(newX, newY, ROW, COL)) {
                if (isDestination(newX, newY, dest)) {
                    cellDetails[newX][newY].parent_x = x;
                    cellDetails[newX][newY].parent_y = y;
                    cout << "\nThe destination cell is found\n";
                    tracePath(cellDetails, dest);
                    return; 
                }
                else if (!closedList[newX][newY] && isUnblocked(grid, newX, newY)) {
                    double gNew = cellDetails[x][y].g + cost[k];
                    double hNew = calculateHValue(newX, newY, dest);
                    double fNew = gNew + hNew;

                    if (cellDetails[newX][newY].f > fNew) {
                        openList.push({fNew, newX, newY});
                        cellDetails[newX][newY].f = fNew;
                        cellDetails[newX][newY].g = gNew;
                        cellDetails[newX][newY].h = hNew;
                        cellDetails[newX][newY].parent_x = x;
                        cellDetails[newX][newY].parent_y = y;
                    }
                }
            }
        }
    }

    cout << "Failed to find the Destination Cell\n";
}


int main() {
    int rows, cols;
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;
    
    vector<vector<int>> grid(rows, vector<int>(cols));

    cout << "\nEnter the grid layout (1 for walkable, 0 for obstacle):\n";
    for(int i = 0; i < rows; ++i) {
        cout << "Row " << i << ": ";
        for(int j = 0; j < cols; ++j) {
            cin >> grid[i][j];
        }
    }

    pair<int, int> src, dest;

    cout << "\nEnter the source coordinates (row col): ";
    cin >> src.first >> src.second;

    cout << "Enter the destination coordinates (row col): ";
    cin >> dest.first >> dest.second;

    aStarSearch(grid, src, dest);

    return 0;
}