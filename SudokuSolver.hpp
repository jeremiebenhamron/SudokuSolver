#pragma once

#include <array>
#include <optional>
#include <vector>

#include "SudokuBoard.hpp"

using CandidatesByCell = std::array<std::set<int>, SUDOKU_CELL_COUNT>;

class SudokuSolver {
public:
    SudokuSolver(SudokuBoard&& board)
    :board(board)
    {}

    SudokuSolver() = delete;

    std::optional<SudokuBoard> solve();

private:
    SudokuBoard board;
    CandidatesByCell candidates;
    
    void populateCandidates();
    void populateCandidates(const SudokuCell& cell);

    std::vector<std::pair<SudokuCell,int>> getCellsWithUniqueCandidate();

    void updateCell(const SudokuCell& cell, const int& value);

    std::set<SudokuCell> getCellsAffectedByChangeAt(const SudokuCell& cellChanged);
    void updateCandidatesAfterChangeAt(const SudokuCell& cellChanged, const int& value);
};