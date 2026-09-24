#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

#include "SudokuSolver.hpp"

std::optional<SudokuBoard> SudokuSolver::solve() {
    populateCandidates();
    populateCellByCandidateMatrices();
    while (board.hasEmptyCells() && allEmptyCellsHaveCandidates())
    {
        const auto& cellsWithUniqueCandidates = getCellsWithUniqueCandidate();
        if (cellsWithUniqueCandidates.empty())
        {
            // There are no cells with unique candidates, so we cannot make any further progress
            // without making a guess.
            const auto firstEmptyCell = board.emptyCells().front();
            const int guessedValue = *candidates[firstEmptyCell.arrayIndex].begin();

            std::cout << "Guessing value " << guessedValue << " for cell at row: " << firstEmptyCell.row() + 1 << ", column: " << firstEmptyCell.column() + 1 << std::endl;

            // Create a copy of the solver to try and solve the puzzle with the guessed value
            SudokuSolver solver = *this;
            solver.board.updateCell(firstEmptyCell, guessedValue);
            if (const auto solution = solver.solve())
            {
                // the guess led to a valid solution
                return solution;
            } else {
                // the guess is invalid, so update candidates accordingly
                invalidateGuess(firstEmptyCell, guessedValue);
                std::cout << "Invalidating guess: " << guessedValue << " for cell at row: " << firstEmptyCell.row() + 1 << ", column: " << firstEmptyCell.column() + 1 << std::endl;
            }

        } else {
            for (const auto& [index, value]: cellsWithUniqueCandidates) {
                board.updateCell(index, value);
                updateCandidatesAfterChangeAt(index, value);
            }
        }
    }

    if (board.hasEmptyCells())
    {
        return std::nullopt;
    }
    
    return board;
}


 void SudokuSolver::populateCandidates() {
    for (const auto& index: board.emptyCells())
    {
        populateCandidates(index);
    }
 }

void SudokuSolver::invalidateGuess(const SudokuCell& cell, const int value) {
    candidates[cell.arrayIndex].erase(value);

    auto& rowCandidateCells = candidateCellsByRow[cell.row()][value - 1];
    rowCandidateCells.erase(
        std::remove(rowCandidateCells.begin(), rowCandidateCells.end(), cell),
        rowCandidateCells.end()
    );

    auto& colCandidateCells = candidateCellsByColumn[cell.column()][value - 1];
    colCandidateCells.erase(
        std::remove(colCandidateCells.begin(), colCandidateCells.end(), cell),
        colCandidateCells.end()
    );

    auto& blockCandidateCells = candidateCellsByBlock[cell.block()][value - 1];
    blockCandidateCells.erase(
        std::remove(blockCandidateCells.begin(), blockCandidateCells.end(), cell),
        blockCandidateCells.end()
    );
}

 void SudokuSolver::populateCandidates(const SudokuCell& cell) {
    std::set<int> allValues{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto rowValues = board.rowValues(cell);
    auto columnValues = board.columnValues(cell);
    auto blockValues = board.blockValues(cell);
    std::set<int> nonCandidates;
    
    for (const auto& values : rowValues) {
        nonCandidates.insert(rowValues.cbegin(), rowValues.cend());
    }

    for (const auto& values : columnValues) {
        nonCandidates.insert(columnValues.cbegin(), columnValues.cend());
    }

    for (const auto& values : blockValues) {
        nonCandidates.insert(blockValues.cbegin(), blockValues.cend());
    }
    
    std::set<int> candidates;
    std::set_difference(allValues.begin(), allValues.end(),
                        nonCandidates.begin(), nonCandidates.end(),
                        std::inserter(candidates, candidates.end())
    );

    this->candidates[cell.arrayIndex] = std::move(candidates);
 }

 void SudokuSolver::populateCellByCandidateMatrices() {
    for (const auto& emptyCell: board.emptyCells()) {
        const auto& cellCandidates = candidates[emptyCell.arrayIndex];
        for (const auto& value: cellCandidates) {
            candidateCellsByRow[emptyCell.row()][value - 1].push_back(emptyCell);
            candidateCellsByColumn[emptyCell.column()][value - 1].push_back(emptyCell);
            candidateCellsByBlock[emptyCell.block()][value - 1].push_back(emptyCell);
        }
    }
 }

std::vector<CellUniqueCandidatePair> SudokuSolver::getCellsWithUniqueCandidate() const {
    std::vector<CellUniqueCandidatePair> cellsWithUniqueCandidates{};
    for (const auto& emptyCell: board.emptyCells())
    {
        const auto& cellCandidates = candidates[emptyCell.arrayIndex];
        if (cellCandidates.size() == 1)
        {
            cellsWithUniqueCandidates.emplace_back(emptyCell, *cellCandidates.begin());
        }
    }

    const auto& uniqueCandidatesInRows = getCellsWithUniqueCandidate(candidateCellsByRow);
    const auto& uniqueCandidatesInColumns = getCellsWithUniqueCandidate(candidateCellsByColumn);
    const auto& uniqueCandidatesInBlocks = getCellsWithUniqueCandidate(candidateCellsByBlock);

    cellsWithUniqueCandidates.insert(cellsWithUniqueCandidates.end(), uniqueCandidatesInRows.begin(), uniqueCandidatesInRows.end());
    cellsWithUniqueCandidates.insert(cellsWithUniqueCandidates.end(), uniqueCandidatesInColumns.begin(), uniqueCandidatesInColumns.end());
    cellsWithUniqueCandidates.insert(cellsWithUniqueCandidates.end(), uniqueCandidatesInBlocks.begin(), uniqueCandidatesInBlocks.end());

    return cellsWithUniqueCandidates;
}

std::vector<CellUniqueCandidatePair>
SudokuSolver::getCellsWithUniqueCandidate(
    const CellsByCandidateMatrix& cellsByValueMatrix) const {
    std::vector<CellUniqueCandidatePair> cellsWithUniqueCandidates{};
    for (const auto& candidateCellsByValue : cellsByValueMatrix)
    {
        for (int valueIndex = 0; valueIndex < 9; ++valueIndex)
        {
            const auto& cellsWithCandidate = candidateCellsByValue[valueIndex];
            if (cellsWithCandidate.size() == 1)
            {
                cellsWithUniqueCandidates.emplace_back(cellsWithCandidate.front(), valueIndex + 1);
            }
        }
    }
    return cellsWithUniqueCandidates;
}

bool SudokuSolver::allEmptyCellsHaveCandidates() const {
    const auto& emptyCells = board.emptyCells();
    return std::all_of(emptyCells.cbegin(), emptyCells.cend(),
        [this](const SudokuCell& cell) { return !candidates[cell.arrayIndex].empty(); });
}

std::set<SudokuCell> SudokuSolver::getCellsAffectedByChangeAt(const SudokuCell& cellChanged) const
{
    std::set<SudokuCell> cellsWithCandidatesAffected{};
    const auto& emptyCellsInSameRow = board.emptyCellsInRow(cellChanged.row());
    const auto& emptyCellsInSameCol = board.emptyCellsInCol(cellChanged.column());
    const auto& emptyCellsInSameBlock = board.emptyCellsInBlock(cellChanged.block());

    cellsWithCandidatesAffected.insert(emptyCellsInSameRow.cbegin(), emptyCellsInSameRow.cend());
    cellsWithCandidatesAffected.insert(emptyCellsInSameCol.cbegin(), emptyCellsInSameCol.cend());
    cellsWithCandidatesAffected.insert(emptyCellsInSameBlock.cbegin(), emptyCellsInSameBlock.cend());

    return cellsWithCandidatesAffected;
}


void SudokuSolver::updateCandidatesAfterChangeAt(const SudokuCell& cellChanged, const int& value) {
    candidates[cellChanged.arrayIndex].clear();
    
    for (const auto& cellWithAffectedCandidates: getCellsAffectedByChangeAt(cellChanged))
    {
        candidates[cellWithAffectedCandidates.arrayIndex].erase(value);
    }

    updateCellByCandidateMatricesAfterChangeAt(cellChanged, value);
}

void SudokuSolver::updateCellByCandidateMatricesAfterChangeAt(const SudokuCell& cellChanged, const int& value)
{
    candidateCellsByRow[cellChanged.row()][value - 1].clear();
    candidateCellsByColumn[cellChanged.column()][value - 1].clear();
    candidateCellsByBlock[cellChanged.block()][value - 1].clear();
}