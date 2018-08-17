#include <iostream>
#include <cstdlib>
#include <vector>
#include <deque>

enum Cell { Queen, Empty, Blocked };

using Row = std::vector<Cell>;
using Grid = std::vector<Row>;

class Chessboard {
private:
    Grid board;

    void blockRow(int i) {
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == Empty) {
                board[i][j] = Blocked;
            }
        }
    }

    void blockCol(int j) {
        for (int i = 0; i < size; ++i) {
            if (board[i][j] == Empty) {
                board[i][j] = Blocked;
            }
        }
    }

    void blockDiag(int i, int j) {
        for (int ii = i, jj = j; (ii < size) && (jj < size); ++ii, ++jj) {
            if (board[ii][jj] == Empty) {
                board[ii][jj] = Blocked;
            }
        }
        for (int ii = i, jj = j; (ii < size) && (jj >= 0); ++ii, --jj) {
            if (board[ii][jj] == Empty) {
                board[ii][jj] = Blocked;
            }
        }
    }

public:
    const int size;

    Chessboard(int n) : size(n) {
        for (int i = 0; i < n; ++i) {
            Row ri(n, Empty);
            board.push_back(ri);
        }
    }
    
    // copy constructor
    Chessboard(const Chessboard& oldBoard) : board(oldBoard.board), size(oldBoard.size) {}

    bool isEmpty(int i, int j) {
        return board[i][j] == Empty;
    }

    Chessboard& placeQueen(int i, int j) {
        board[i][j] = Queen;
        blockRow(i);
        blockCol(j);
        blockDiag(i,j);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream&, const Chessboard&);
};

std::ostream& operator<<(std::ostream& stream, const Chessboard& cb) {
    for (const Row& r : cb.board) {
        for (const Cell& c : r) {
            switch (c) {
                case Queen:   stream << 'Q'; break;
                case Empty:   stream << '_'; break;
                case Blocked: stream << '.'; break;
            }
        }
        stream << "\n";
    }
    return stream;
}

struct PartialSolution {
    Chessboard state;
    int rowSolvedTo;
    PartialSolution(Chessboard board, int row) : state(board), rowSolvedTo(row) {}
    PartialSolution(const PartialSolution& p) : state(p.state), rowSolvedTo(p.rowSolvedTo) {}
};

std::vector<Chessboard> solveNQueens(int n) {
    PartialSolution empty(Chessboard(n), 0);
    std::deque<PartialSolution> partialSolutions {empty};
    std::vector<Chessboard> solutions;

    while (!partialSolutions.empty()) {
        PartialSolution currentPSol = partialSolutions.front();
        partialSolutions.pop_front();
        int row = currentPSol.rowSolvedTo;
        for (int j = 0; j < n; ++j) {
            if (currentPSol.state.isEmpty(row, j)) {
                PartialSolution newPSol(currentPSol);
                newPSol.state.placeQueen(row,j);
                newPSol.rowSolvedTo++;
                if (newPSol.rowSolvedTo == n) {
                    solutions.push_back(newPSol.state);
                } else {
                    partialSolutions.push_back(newPSol);
                }
            }
        }
    }
    return solutions;
}

int main(int argc, const char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " n\n";
        return 1;
    }
    int n = atoi(argv[1]);
    std::vector<Chessboard> sols = solveNQueens(n);
    for (auto sol : sols) {
        std::cout << sol << "\n";
    }
    std::cout << "Number of solutions: " << sols.size() << "\n"; 
    return 0;
}
