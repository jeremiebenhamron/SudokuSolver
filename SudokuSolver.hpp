#pragma once

#include <array>
#include <optional>
#include <vector>

#include "SudokuBoard.hpp"

using CellUniqueCandidatePair = std::pair<SudokuCell, int>;

using CandidatesByCell = std::array<std::set<int>, SUDOKU_CELL_COUNT>;

/**
 * Maps each Sudoku unit(row, column, or block index) and candidate value to the cells that can contain it.
 *
 * The first index identifies one of the nine units. The second index is the
 * candidate value minus one, so index 0 represents value 1 and index 8
 * represents value 9.
 */
using CellsByCandidateMatrix =
    std::array<std::array<std::vector<SudokuCell>, 9>, 9>;

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

    /**
     * For each row and candidate value, stores the cells in that row that can
     * accommodate the value.
     */
    CellsByCandidateMatrix candidateCellsByRow;

    /**
     * For each column and candidate value, stores the cells in that column
     * that can accommodate the value.
     */
    CellsByCandidateMatrix candidateCellsByColumn;

    /**
     * For each 3x3 block and candidate value, stores the cells in that block
     * that can accommodate the value.
     */
    CellsByCandidateMatrix candidateCellsByBlock;

    /// Finds cells whose candidate set contains exactly one value.
    std::vector<CellUniqueCandidatePair> getCellsWithUniqueCandidate() const;

    /// Find the cells in a row/column/block that can accommodate a candidate 
    /// that no other cells in that row/column/block can accommodate.
    std::vector<CellUniqueCandidatePair> getCellsWithUniqueCandidate(
        const CellsByCandidateMatrix& cellsByValueMatrix) const;
    
    /// Finds empty cells sharing a row, column, or block with a changed cell.
    std::set<SudokuCell> getCellsAffectedByChangeAt(const SudokuCell& cellChanged) const;
    
    /// Initializes candidates for every cell on the board.
    void populateCandidates();

    /// Initializes candidates for one cell.
    void populateCandidates(const SudokuCell& cell);

    /// Updates the candidate matrices to reflect the current candidates.
    void populateCellByCandidateMatrices();

    /// Writes a value to a cell on the board.
    void updateCell(const SudokuCell& cell, const int& value);

    /// Recomputes candidates affected by a newly assigned cell value.
    void updateCandidatesAfterChangeAt(const SudokuCell& cellChanged, const int& value);

    /// Updates the candidate matrices to reflect a newly assigned cell value.
    void updateCellByCandidateMatricesAfterChangeAt(const SudokuCell& cellChanged, const int& value);
};