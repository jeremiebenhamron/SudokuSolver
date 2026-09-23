#pragma once

#include <optional>

#include "SudokuBoard.hpp"

class SudokuSolver {
public:
    SudokuSolver(SudokuBoard&& board)
    :board(board)
    {}

    SudokuSolver() = delete;

    std::optional<SudokuBoard> solve();

private:
    SudokuBoard board;
};