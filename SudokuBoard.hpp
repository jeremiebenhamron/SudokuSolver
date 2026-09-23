#include <array>

constexpr int SUDOKU_CELL_COUNT = 81;
using SudokuPuzzle = std::array<int, SUDOKU_CELL_COUNT>;

class SudokuBoard {
public:
    SudokuBoard(SudokuPuzzle&& puzzle)
    : board(puzzle)
    {}

    SudokuBoard() = delete;

    void print();

private:
    SudokuPuzzle board;
};
