#include <iostream>

#include "SudokuBoard.hpp"

void SudokuBoard::print() {
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