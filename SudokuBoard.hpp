#pragma once

#include <array>
#include <compare>
#include <span>
#include <set>


constexpr int SUDOKU_CELL_COUNT = 81;
using SudokuPuzzle = std::array<int, SUDOKU_CELL_COUNT>;

struct SudokuCell {
    explicit SudokuCell(int arrayIndex) noexcept
    : arrayIndex(arrayIndex)
    {}
    
    SudokuCell(const int& row, const int& col) noexcept 
    : arrayIndex(row * 9 + col)
    {}

    // index value in array<int, 81>
    int arrayIndex;

    auto operator<=>(const SudokuCell&) const = default;

    int row() const noexcept {
        return arrayIndex / 9;
    }

    int column() const noexcept {
        return arrayIndex % 9;
    }

    int block() const noexcept {
        return (row() / 3) * 3 + (column() / 3);
    }
};

class SudokuBoard {
public:
    SudokuBoard(SudokuPuzzle&& puzzle)
    : board(puzzle)
    {}

    SudokuBoard() = delete;

    void print();

    void updateCell(const SudokuCell& cell, const int& value);

    bool isEmpty(const SudokuCell& cell);

    bool hasEmptyCells();
    std::vector<SudokuCell> emptyCells();
    std::vector<SudokuCell> emptyCellsInRow(const int& row);
    std::vector<SudokuCell> emptyCellsInCol(const int& row);
    std::vector<SudokuCell> emptyCellsInBlock(const int& block);

    std::set<int> rowValues(const SudokuCell& cell);
    std::set<int> columnValues(const SudokuCell& cell);
    std::set<int> blockValues(const SudokuCell& cell);


private:
    SudokuPuzzle board;
};
