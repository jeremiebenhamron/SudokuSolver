#include <iostream>

#include "SudokuBoard.hpp"

void SudokuBoard::print() const {
    for (int i = 0; i < SUDOKU_CELL_COUNT; ++i) {
        if (board[i] == 0) {
            std::cout << ". ";
        } else {
            std::cout << board[i] << ' ';
        }

        if ((i + 1) % 9 == 0) {
            std::cout << std::endl;
        }
    }
}

bool SudokuBoard::hasEmptyCells() const {
    return emptyCells().size() > 0;
}

bool SudokuBoard::isEmpty(const SudokuCell& cell) const
{
    return 0 == board[cell.arrayIndex];
}

std::vector<SudokuCell> SudokuBoard::emptyCellsInRow(const int& row) const {
    std::vector<SudokuCell> cells{};
    for (int col = 0;  col < 9; ++col)
    {
        const auto cell = SudokuCell(row, col);
        if (isEmpty(cell))
        {
            cells.push_back(std::move(cell));
        }
    }
    return cells;
}

std::vector<SudokuCell> SudokuBoard::emptyCellsInCol(const int& col) const {
    std::vector<SudokuCell> cells{};
    for (int row = 0; row < 9; ++row)
    {
        const auto cell = SudokuCell(row, col);
        if (isEmpty(cell))
        {
            cells.push_back(std::move(cell));
        }
    }
    return cells;
}

std::vector<SudokuCell> SudokuBoard::emptyCellsInBlock(const int& block) const {
    std::vector<SudokuCell> cells{};
    int startRow = (block / 3) * 3;
    int startColumn = (block % 3) * 3;
    for (int row = startRow; row < startRow + 3; ++row)
    {
        for (int col = startColumn; col < startColumn + 3; ++col)
        {
            const auto cell = SudokuCell(row, col);
            if (isEmpty(cell))
            {
                cells.push_back(std::move(cell));
            }
        }
    }
    return cells;
}

std::vector<SudokuCell> SudokuBoard::emptyCells() const {
    std::vector<SudokuCell> emptyCells{};
    for (int i = 0; i < static_cast<int>(board.size()); i++)
    {
        if (0 == board[i]) {
            emptyCells.push_back(SudokuCell(i));
        }
    }

    return emptyCells;
}

std::set<int> SudokuBoard::rowValues(const SudokuCell& cell) const {
    std::set<int> values{};

    for (int col = 0; col < 9; ++col) {
        if (auto value = board[cell.row() * 9 + col])
        {
            values.insert(board[value]);
        }
    }

    return values;
}

std::set<int> SudokuBoard::columnValues(const SudokuCell& cell) const {
    std::set<int> values{};

    for (int row = 0; row < 9; ++row) {
        if (auto value = board[row * 9 + cell.column()])
        {
            values.insert(board[value]);
        }
    }
    return values;
}

std::set<int> SudokuBoard::blockValues(const SudokuCell& cell) const {
    std::set<int> values;
    int startRow = (cell.block() / 3) * 3;
    int startColumn = (cell.block() % 3) * 3;
    for (int row = startRow; row < startRow + 3; ++row)
    {
        for (int col = startColumn; col < startColumn + 3; ++col)
        {
            if (auto value = board[row * 9 + col])
            {
                values.insert(board[value]);
            }
        }
    }
    return values;
}

void SudokuBoard::updateCell(const SudokuCell& cell, const int& value) {
    board[cell.arrayIndex] = value;
}
