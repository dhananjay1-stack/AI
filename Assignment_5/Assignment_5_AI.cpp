// tic_tac_toe_minimax.cpp
// Compile: g++ -std=c++17 -O2 -o ttt tic_tac_toe_minimax.cpp

#include <bits/stdc++.h>
using namespace std;

const char AI = 'X';
const char HUMAN = 'O';
const char EMPTY = ' ';

// Winning lines (0..8 indices for a 3x3 board)
const int WIN_LINES[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8}, // rows
    {0,3,6}, {1,4,7}, {2,5,8}, // cols
    {0,4,8}, {2,4,6}           // diagonals
};

void printBoard(const vector<char>& board) {
    cout << "\n";
    for (int r = 0; r < 3; ++r) {
        cout << " ";
        for (int c = 0; c < 3; ++c) {
            cout << board[r*3 + c];
            if (c < 2) cout << " | ";
        }
        cout << "\n";
        if (r < 2) cout << "---+---+---\n";
    }
    cout << "\n";
}

bool isMovesLeft(const vector<char>& board) {
    for (char c : board) if (c == EMPTY) return true;
    return false;
}

char checkWinner(const vector<char>& board) {
    for (auto &line : WIN_LINES) {
        char a = board[line[0]], b = board[line[1]], c = board[line[2]];
        if (a != EMPTY && a == b && b == c) return a;
    }
    return 0; // no winner yet
}

// Evaluation function:
// +10 for AI win, -10 for Human win, 0 for draw/no-win.
// We will subtract/add depth in minimax to prefer faster wins and slower losses.
int evaluate(const vector<char>& board, int depth) {
    char winner = checkWinner(board);
    if (winner == AI) return 10 - depth;
    if (winner == HUMAN) return -10 + depth;
    return 0;
}

// Minimax with alpha-beta pruning
int minimax(vector<char>& board, int depth, bool isMaximizing, int alpha, int beta) {
    char winner = checkWinner(board);
    if (winner != 0 || !isMovesLeft(board)) {
        return evaluate(board, depth);
    }

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == EMPTY) {
                board[i] = AI;
                int val = minimax(board, depth + 1, false, alpha, beta);
                board[i] = EMPTY;
                best = max(best, val);
                alpha = max(alpha, best);
                if (beta <= alpha) break; // beta cut-off
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == EMPTY) {
                board[i] = HUMAN;
                int val = minimax(board, depth + 1, true, alpha, beta);
                board[i] = EMPTY;
                best = min(best, val);
                beta = min(beta, best);
                if (beta <= alpha) break; // alpha cut-off
            }
        }
        return best;
    }
}

int findBestMove(vector<char>& board) {
    int bestVal = INT_MIN;
    int bestMove = -1;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == EMPTY) {
            board[i] = AI;
            int moveVal = minimax(board, 0, false, INT_MIN, INT_MAX);
            board[i] = EMPTY;
            if (moveVal > bestVal) {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<char> board(9, EMPTY);
    cout << "Tic-Tac-Toe: You (O) vs AI (X)\n";
    cout << "Board positions are numbered 1..9 as:\n";
    cout << " 1 | 2 | 3\n";
    cout << "---+---+---\n";
    cout << " 4 | 5 | 6\n";
    cout << "---+---+---\n";
    cout << " 7 | 8 | 9\n\n";

    char first;
    cout << "Do you want to play first? (y/n): ";
    cin >> first;
    bool humanTurn = (first == 'y' || first == 'Y');

    printBoard(board);

    while (true) {
        if (checkWinner(board) != 0 || !isMovesLeft(board)) break;

        if (humanTurn) {
            int pos;
            while (true) {
                cout << "Enter your move (1-9): ";
                if (!(cin >> pos)) {
                    cout << "Invalid input. Exiting.\n";
                    return 0;
                }
                if (pos < 1 || pos > 9) {
                    cout << "Position must be 1..9.\n";
                    continue;
                }
                if (board[pos-1] != EMPTY) {
                    cout << "Cell already occupied. Try again.\n";
                    continue;
                }
                break;
            }
            board[pos-1] = HUMAN;
            printBoard(board);
        } else {
            cout << "AI is thinking...\n";
            int best = findBestMove(board);
            if (best == -1) {
                // no moves left
                break;
            }
            board[best] = AI;
            cout << "AI played at position " << (best+1) << "\n";
            printBoard(board);
        }

        humanTurn = !humanTurn;
    }

    char winner = checkWinner(board);
    if (winner == AI) cout << "AI wins!\n";
    else if (winner == HUMAN) cout << "You win! Congratulations.\n";
    else cout << "It's a draw.\n";

    return 0;
}
