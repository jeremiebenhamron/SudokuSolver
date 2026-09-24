#pragma once

#include <array>
#include <optional>
#include <vector>

#include "SudokuBoard.hpp"

using CandidatesByCell = std::array<std::set<int>, SUDOKU_CELL_COUNT>;

/**
 * Solves a Sudoku board by tracking the candidate values for each cell.
 *
 * The solver owns the board passed to its constructor and updates candidate
 * values as cells are filled during the solving process.
 */
class SudokuSolver {
public:
    /**
     * Creates a solver for the provided Sudoku board.
     *
     * @param board Board to solve.
     */
    SudokuSolver(SudokuBoard&& board)
    :board(board)
    {}

    /// A solver must be constructed with a board to solve.
    SudokuSolver() = delete;

    /**
     * Attempts to solve the board.
     *
     * @return The solved board when a solution is found, or std::nullopt when
     *         the puzzle cannot be solved.
     */
    std::optional<SudokuBoard> solve();

private:
    /// Board currently being solved.
    SudokuBoard board;

    /// Candidate values currently possible for each cell.
    CandidatesByCell candidates;
    
    /// Finds cells whose candidate set contains exactly one value.
    std::vector<std::pair<SudokuCell,int>> getCellsWithUniqueCandidate() const;
    
    /// Finds empty cells sharing a row, column, or block with a changed cell.
    std::set<SudokuCell> getCellsAffectedByChangeAt(const SudokuCell& cellChanged) const;
    
    /// Initializes candidates for every cell on the board.
    void populateCandidates();

    /// Initializes candidates for one cell.
    void populateCandidates(const SudokuCell& cell);

    /// Writes a value to a cell on the board.
    void updateCell(const SudokuCell& cell, const int& value);


    /// Recomputes candidates affected by a newly assigned cell value.
    void updateCandidatesAfterChangeAt(const SudokuCell& cellChanged, const int& value);
};