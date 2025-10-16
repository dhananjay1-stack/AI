#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <string>
using namespace std;
using namespace chrono;

// 3x3 board state
struct State {
    vector<int> board;
    int blank_pos;
    int depth;
    string path;
    
    State(vector<int> b = {}) : board(b), depth(0), path("") {
        blank_pos = find(board.begin(), board.end(), 0) - board.begin();
    }
    
    string serialize() const {
        string s;
        for(int x : board) s += to_string(x);
        return s;
    }
    
    bool is_goal() const {
        for(int i = 0; i < 9; i++)
            if(board[i] != i) return false;
        return true;
    }
    
    void print() const {
        for(int i = 0; i < 9; i++) {
            cout << board[i] << (i % 3 == 2 ? "\n" : " ");
        }
    }
};

// Performance metrics tracker
struct Metrics {
    int nodes_expanded = 0;
    int nodes_generated = 0;
    int max_depth = 0;
    double time_ms = 0;
    bool solved = false;
    int solution_length = 0;
    
    void print(const string& algo) const {
        cout << "\n=== " << algo << " Results ===\n";
        cout << "Solved: " << (solved ? "YES" : "NO") << "\n";
        cout << "Nodes Expanded: " << nodes_expanded << "\n";
        cout << "Nodes Generated: " << nodes_generated << "\n";
        cout << "Solution Length: " << solution_length << "\n";
        cout << "Max Depth Reached: " << max_depth << "\n";
        cout << "Time: " << fixed << setprecision(2) << time_ms << " ms\n";
    }
};

class PuzzleSolver {
private:
    State initial;
    vector<int> goal = {0,1,2,3,4,5,6,7,8};
    
    // Move directions: row_delta, col_delta, name
    vector<tuple<int,int,char>> moves;
    
    // Get neighbors based on current move ordering
    vector<State> get_neighbors(const State& s) {
        vector<State> neighbors;
        int row = s.blank_pos / 3;
        int col = s.blank_pos % 3;
        
        for(auto [dr, dc, move_char] : moves) {
            int new_row = row + dr;
            int new_col = col + dc;
            
            if(new_row >= 0 && new_row < 3 && new_col >= 0 && new_col < 3) {
                State next = s;
                int new_pos = new_row * 3 + new_col;
                swap(next.board[s.blank_pos], next.board[new_pos]);
                next.blank_pos = new_pos;
                next.depth = s.depth + 1;
                next.path = s.path + move_char;
                neighbors.push_back(next);
            }
        }
        return neighbors;
    }
    
public:
    PuzzleSolver(const State& start) : initial(start) {
        set_move_order("UDLR"); // Default: Up, Down, Left, Right
    }
    
    void set_move_order(const string& order) {
        moves.clear();
        for(char c : order) {
            switch(c) {
                case 'U': moves.push_back({-1, 0, 'U'}); break;
                case 'D': moves.push_back({1, 0, 'D'}); break;
                case 'L': moves.push_back({0, -1, 'L'}); break;
                case 'R': moves.push_back({0, 1, 'R'}); break;
            }
        }
    }
    
    // DFS with depth limit
    Metrics dfs(int max_depth = 31) {
        Metrics m;
        auto start_time = high_resolution_clock::now();
        
        unordered_set<string> visited;
        vector<State> stack;
        stack.push_back(initial);
        
        while(!stack.empty()) {
            State curr = stack.back();
            stack.pop_back();
            
            string key = curr.serialize();
            if(visited.count(key)) continue;
            visited.insert(key);
            
            m.nodes_expanded++;
            m.max_depth = max(m.max_depth, curr.depth);
            
            if(curr.is_goal()) {
                m.solved = true;
                m.solution_length = curr.path.length();
                m.time_ms = duration<double, milli>(high_resolution_clock::now() - start_time).count();
                return m;
            }
            
            if(curr.depth >= max_depth) continue;
            
            vector<State> neighbors = get_neighbors(curr);
            m.nodes_generated += neighbors.size();
            
            // Push in reverse for correct DFS order
            for(int i = neighbors.size()-1; i >= 0; i--) {
                if(!visited.count(neighbors[i].serialize()))
                    stack.push_back(neighbors[i]);
            }
        }
        
        m.time_ms = duration<double, milli>(high_resolution_clock::now() - start_time).count();
        return m;
    }
    
    // BFS
    Metrics bfs() {
        Metrics m;
        auto start_time = high_resolution_clock::now();
        
        unordered_set<string> visited;
        queue<State> q;
        q.push(initial);
        visited.insert(initial.serialize());
        
        while(!q.empty()) {
            State curr = q.front();
            q.pop();
            
            m.nodes_expanded++;
            m.max_depth = max(m.max_depth, curr.depth);
            
            if(curr.is_goal()) {
                m.solved = true;
                m.solution_length = curr.path.length();
                m.time_ms = duration<double, milli>(high_resolution_clock::now() - start_time).count();
                return m;
            }
            
            vector<State> neighbors = get_neighbors(curr);
            m.nodes_generated += neighbors.size();
            
            for(const State& next : neighbors) {
                string key = next.serialize();
                if(!visited.count(key)) {
                    visited.insert(key);
                    q.push(next);
                }
            }
        }
        
        m.time_ms = duration<double, milli>(high_resolution_clock::now() - start_time).count();
        return m;
    }
    
    // Iterative Deepening DFS - combines DFS memory efficiency with BFS completeness
    Metrics iddfs(int max_limit = 31) {
        Metrics m;
        auto start_time = high_resolution_clock::now();
        
        for(int limit = 0; limit <= max_limit; limit++) {
            unordered_set<string> visited;
            vector<State> stack;
            stack.push_back(initial);
            
            while(!stack.empty()) {
                State curr = stack.back();
                stack.pop_back();
                
                string key = curr.serialize();
                if(visited.count(key)) continue;
                visited.insert(key);
                
                m.nodes_expanded++;
                m.max_depth = max(m.max_depth, curr.depth);
                
                if(curr.is_goal()) {
                    m.solved = true;
                    m.solution_length = curr.path.length();
                    m.time_ms = duration<double, milli>(high_resolution_clock::now() - start_time).count();
                    return m;
                }
                
                if(curr.depth >= limit) continue;
                
                vector<State> neighbors = get_neighbors(curr);
                m.nodes_generated += neighbors.size();
                
                for(int i = neighbors.size()-1; i >= 0; i--) {
                    if(!visited.count(neighbors[i].serialize()))
                        stack.push_back(neighbors[i]);
                }
            }
        }
        
        m.time_ms = duration<double, milli>(high_resolution_clock::now() - start_time).count();
        return m;
    }
};

// Check if puzzle is solvable (inversion count must be even)
bool is_solvable(const vector<int>& board) {
    int inversions = 0;
    for(int i = 0; i < 9; i++) {
        if(board[i] == 0) continue;
        for(int j = i+1; j < 9; j++) {
            if(board[j] == 0) continue;
            if(board[i] > board[j]) inversions++;
        }
    }
    return inversions % 2 == 0;
}

// Experiment: Compare different move orderings
void move_ordering_experiment(const State& start) {
    vector<string> orderings = {"UDLR", "DURL", "LRUD", "RLDU"};
    
    cout << "\n========================================\n";
    cout << "MOVE ORDERING EXPERIMENT (BFS)\n";
    cout << "========================================\n";
    cout << "Testing how different move orders affect performance\n\n";
    
    for(const string& order : orderings) {
        PuzzleSolver solver(start);
        solver.set_move_order(order);
        Metrics m = solver.bfs();
        
        cout << "Order: " << order << " | ";
        cout << "Expanded: " << setw(6) << m.nodes_expanded << " | ";
        cout << "Time: " << fixed << setprecision(2) << setw(7) << m.time_ms << " ms\n";
    }
}

int main() {
    // Test cases
    vector<vector<int>> test_cases = {
        {1,2,3,4,5,6,7,8,0},  // Easy: 0 moves
        {1,2,3,4,5,6,0,7,8},  // Easy: 2 moves
        {1,2,3,4,0,5,7,8,6},  // Medium: 5 moves
        {0,1,3,4,2,5,7,8,6},  // Medium: 7 moves
        {8,6,7,2,5,4,3,0,1}   // Hard: 31 moves (near worst case)
    };
    
    for(int tc = 0; tc < test_cases.size(); tc++) {
        if(!is_solvable(test_cases[tc])) {
            cout << "Test case " << tc+1 << " is not solvable!\n";
            continue;
        }
        
        State start(test_cases[tc]);
        
        cout << "\n\n╔════════════════════════════════════════╗\n";
        cout << "║      TEST CASE " << tc+1 << "                    ║\n";
        cout << "╚════════════════════════════════════════╝\n";
        start.print();
        
        PuzzleSolver solver(start);
        
        // Run all three algorithms
        Metrics dfs_m = solver.dfs(31);
        dfs_m.print("DFS (depth limit 31)");
        
        Metrics bfs_m = solver.bfs();
        bfs_m.print("BFS");
        
        Metrics iddfs_m = solver.iddfs(31);
        iddfs_m.print("IDDFS");
        
        // Move ordering experiment (only for smaller cases)
        if(tc < 3) {
            move_ordering_experiment(start);
        }
        
        // Comparison summary
        cout << "\n--- Algorithm Comparison ---\n";
        cout << "Algorithm | Expanded | Generated | Time(ms)\n";
        cout << "----------|----------|-----------|----------\n";
        cout << "DFS       | " << setw(8) << dfs_m.nodes_expanded << " | " 
             << setw(9) << dfs_m.nodes_generated << " | " << fixed << setprecision(2) << dfs_m.time_ms << "\n";
        cout << "BFS       | " << setw(8) << bfs_m.nodes_expanded << " | " 
             << setw(9) << bfs_m.nodes_generated << " | " << fixed << setprecision(2) << bfs_m.time_ms << "\n";
        cout << "IDDFS     | " << setw(8) << iddfs_m.nodes_expanded << " | " 
             << setw(9) << iddfs_m.nodes_generated << " | " << fixed << setprecision(2) << iddfs_m.time_ms << "\n";
    }
    
    cout << "\n\n========================================\n";
    cout << "KEY OBSERVATIONS:\n";
    cout << "========================================\n";
    cout << "1. BFS guarantees shortest path but uses more memory\n";
    cout << "2. DFS uses less memory but may find longer paths\n";
    cout << "3. IDDFS combines benefits: optimal + low memory\n";
    cout << "4. Move ordering affects node expansions in BFS\n";
    cout << "5. IDDFS expands more nodes due to repeated work\n";
    
    return 0;
}