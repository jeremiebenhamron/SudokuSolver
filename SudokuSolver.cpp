#include <algorithm>
#include <iterator>
#include <set>

#include "SudokuSolver.hpp"

std::optional<SudokuBoard> SudokuSolver::solve() {
    populateCandidates();
    populateCellByCandidateMatrices();
    while (board.hasEmptyCells() && allEmptyCellsHaveCandidates())
    {
        for (const auto& [index, value]: getCellsWithUniqueCandidate()) {
            board.updateCell(index, value);
            updateCandidatesAfterChangeAt(index, value);
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