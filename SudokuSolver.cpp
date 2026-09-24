#include <algorithm>
#include <iterator>

#include "SudokuSolver.hpp"

using CellUniqueCandidatePair = std::pair<SudokuCell, int>;

std::optional<SudokuBoard> SudokuSolver::solve() {
    populateCandidates();
    while (board.hasEmptyCells())
    {
        for (const auto& [index, value]: getCellsWithUniqueCandidate()) {
            board.updateCell(index, value);
            updateCandidatesAfterChangeAt(index, value);
        }
    }
    
    return std::nullopt; // Placeholder return value
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
    
    for (const auto& values: {rowValues, columnValues, blockValues})
    {
        nonCandidates.insert(values.cbegin(), values.cend());
    }
    
    
    std::set<int> candidates;
    std::set_difference(allValues.begin(), allValues.end(),
                        nonCandidates.begin(), nonCandidates.end(),
                        std::inserter(candidates, candidates.end())
    );

    this->candidates[cell.arrayIndex] = std::move(candidates);
 }

std::vector<CellUniqueCandidatePair> SudokuSolver::getCellsWithUniqueCandidate() {
    std::vector<CellUniqueCandidatePair> cellsWithUniqueCandidates{};
    for (const auto& emptyCell: board.emptyCells())
    {
        const auto& cellCandidates = candidates[emptyCell.arrayIndex];
        if (cellCandidates.size() == 1)
        {
            cellsWithUniqueCandidates.emplace_back(emptyCell, *cellCandidates.begin());
        }
    }
    return cellsWithUniqueCandidates;
}

std::set<SudokuCell> SudokuSolver::getCellsAffectedByChangeAt(const SudokuCell& cellChanged)
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
}