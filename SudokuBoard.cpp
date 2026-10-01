#include <iostream>
#include <numeric>
#include <thread>

#include "SudokuBoard.hpp"

SudokuBoard::SudokuBoard(SudokuPuzzle&& puzzle)
: board(puzzle)
{
    for (int i = 0; i < static_cast<int>(board.size()); i++)
    {
        if (0 == board[i]) {
            m_emptyCells.emplace(i, SudokuCell(i));
        }
    }
}

int SudokuBoard::sumInRow(const SudokuCell& cell) const {
    const auto valuesInRow = rowValues(cell);
    return std::accumulate(valuesInRow.begin(), valuesInRow.end(), 0);
}

int SudokuBoard::sumInColumn(const SudokuCell& cell) const {
    const auto valuesInColumn = columnValues(cell);
    return std::accumulate(valuesInColumn.begin(), valuesInColumn.end(), 0);
}

int SudokuBoard::sumInBlock(const SudokuCell& cell) const {
    const auto valuesInBlock = blockValues(cell);
    return std::accumulate(valuesInBlock.begin(), valuesInBlock.end(), 0);
}

bool SudokuBoard::verify() const noexcept {
    // Implementation for verifying the Sudoku board
    if (hasEmptyCells()) {
        return false; // Board is not complete
    }

    bool rowsValid = true;
    std::thread rowThread([this, &rowsValid]() {
        for (int row = 0; row < 9; ++row) {
            SudokuCell cell(row, 0); // Get the first cell in the row
            if (sumInRow(cell) != 45) { // Sum of numbers 1-9 is 45
                rowsValid = false;
                break; // No need to check further rows if one is invalid
            }
        }
    });

    bool colsValid = true;
    std::thread colThread([this, &colsValid]() {
        for (int col = 0; col < 9; ++col) {
            SudokuCell cell(0, col); // Get the first cell in the column
            if (sumInColumn(cell) != 45) { // Sum of numbers 1-9 is 45
                colsValid = false;
                break; // No need to check further columns if one is invalid
            }
        }
    });

    bool blocksValid = true;
    std::thread blockThread([this, &blocksValid]() {
        for (int block = 0; block < 9; ++block) {
            SudokuCell firstCellInBlock((block / 3) * 3, (block % 3) * 3); // Get the first cell in the block
            if (sumInBlock(firstCellInBlock) != 45) { // Sum of numbers 1-9 is 45
                blocksValid = false;
                break; // No need to check further blocks if one is invalid
            }
        }    
    });

    rowThread.join();
    colThread.join();
    blockThread.join();
    
    return rowsValid && colsValid && blocksValid;
}

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

std::unordered_map<int, SudokuCell> SudokuBoard::emptyCells() const {
    return m_emptyCells;
}

std::set<int> SudokuBoard::rowValues(const SudokuCell& cell) const {
    std::set<int> values{};

    for (int col = 0; col < 9; ++col) {
        if (auto value = board[cell.row() * 9 + col])
        {
            values.insert(value);
        }
    }

    return values;
}

std::set<int> SudokuBoard::columnValues(const SudokuCell& cell) const {
    std::set<int> values{};

    for (int row = 0; row < 9; ++row) {
        if (auto value = board[row * 9 + cell.column()])
        {
            values.insert(value);
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
                values.insert(value);
            }
        }
    }
    return values;
}

void SudokuBoard::updateCell(const SudokuCell& cell, const int& value) {
    std::cout << "Updating cell at row: " << cell.row() + 1 << ", column: " << cell.column() + 1 << " with value: " << value << std::endl;
    board[cell.arrayIndex] = value;
    std::cout << std::endl;

    if (value == 0) {
        m_emptyCells.emplace(cell.arrayIndex, cell);
    } else {
        m_emptyCells.erase(cell.arrayIndex);
    }
    // std::cout << "Board after update:" << std::endl;
    // print();
}
