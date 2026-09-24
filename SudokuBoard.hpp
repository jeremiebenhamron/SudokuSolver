#pragma once

#include <array>
#include <compare>
#include <span>
#include <set>


constexpr int SUDOKU_CELL_COUNT = 81;
using SudokuPuzzle = std::array<int, SUDOKU_CELL_COUNT>;

/**
 * Identifies one cell in a Sudoku board.
 *
 * Cells are stored in row-major order, so arrayIndex is calculated as
 * row * 9 + column.
 */
struct SudokuCell {
    /// Creates a cell from its zero-based linear board index.
    explicit SudokuCell(int arrayIndex) noexcept
    : arrayIndex(arrayIndex)
    {}

    /// Creates a cell from its zero-based row and column.
    SudokuCell(const int& row, const int& col) noexcept 
    : arrayIndex(row * 9 + col)
    {}

    /// Zero-based index into the board's underlying array.
    int arrayIndex;

    /// Orders cells by their linear board index.
    auto operator<=>(const SudokuCell&) const = default;

    /// Returns the cell's zero-based row.
    int row() const noexcept {
        return arrayIndex / 9;
    }

    /// Returns the cell's zero-based column.
    int column() const noexcept {
        return arrayIndex % 9;
    }

    /// Returns the cell's zero-based 3x3 block index.
    int block() const noexcept {
        return (row() / 3) * 3 + (column() / 3);
    }
};

/**
 * Stores and provides operations for a 9x9 Sudoku board.
 *
 * A value of zero represents an empty cell. The board owns its underlying
 * puzzle data and exposes methods for querying and updating cells and groups.
 */
class SudokuBoard {
public:
    /// Creates a board from an 81-cell puzzle.
    SudokuBoard(SudokuPuzzle&& puzzle)
    : board(puzzle)
    {}

    /// A board must be constructed from puzzle data.
    SudokuBoard() = delete;

    /// Assigns a value to the specified cell.
    void updateCell(const SudokuCell& cell, const int& value);
    
    /// Prints the board, displaying empty cells as dots.
    void print() const;
    
    /// Returns whether the specified cell is empty.
    bool isEmpty(const SudokuCell& cell) const;

    /// Returns whether the board contains at least one empty cell.
    bool hasEmptyCells() const;

    /// Returns all empty cells on the board.
    std::vector<SudokuCell> emptyCells() const;

    /// Returns empty cells in the specified row.
    std::vector<SudokuCell> emptyCellsInRow(const int& row) const;

    /// Returns empty cells in the specified column.
    std::vector<SudokuCell> emptyCellsInCol(const int& row) const;

    /// Returns empty cells in the specified 3x3 block.
    std::vector<SudokuCell> emptyCellsInBlock(const int& block) const;

    /// Returns the values in the specified row.
    std::set<int> rowValues(const SudokuCell& cell) const;

    /// Returns the values in the specified column.
    std::set<int> columnValues(const SudokuCell& cell) const;

    /// Returns the values in the specified 3x3 block.
    std::set<int> blockValues(const SudokuCell& cell) const;


private:
    /// Underlying row-major board storage.
    SudokuPuzzle board;
};
