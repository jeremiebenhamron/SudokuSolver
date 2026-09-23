#include <fstream>
#include <iostream>

#include <array>
#include <optional>
#include <string>
#include <unordered_set>

#include "SudokuBoard.hpp"

std::string getSudokuString(const std::string& fileName) {
    std::fstream file(fileName);

    if(!file.is_open()) { 
        throw std::runtime_error("Failed to open: " + fileName);
    }

    std::string sudokuPuzzle;
    // Ignore all lines except the first one
    std::getline(file, sudokuPuzzle);

    return sudokuPuzzle;
}

bool isValidCell(const int& value) noexcept {
    return  (0 <= value) && (value <= 9);
}

std::optional<SudokuBoard> validSudoku(const std::string& sudokuString) {
    if(sudokuString.length() != SUDOKU_CELL_COUNT)
    {
        std::cerr << "Sudoku must have " << SUDOKU_CELL_COUNT << " cells, but has: " << sudokuString.length() << " cells" << std::endl;
        return std::nullopt;
    }

    SudokuPuzzle sudokuArray{};
    auto outputCell = sudokuArray.begin();
    for (const auto& cellChar: sudokuString) {
        const int cellValue = cellChar - '0';
        if (!isValidCell(cellValue)) {
            std::cerr << "Found invalid cell value: " << cellChar << std::endl;
            return std::nullopt;
        }
        *outputCell++ = cellValue;
    }
    return SudokuBoard(std::move(sudokuArray));
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sudoku_file.txt>" << std::endl;
        return 1;
    }

    std::string sudokuFileName(argv[1]);
    try {
        std::string sudokuString = getSudokuString(sudokuFileName);
        if (auto sudokuBoard = validSudoku(sudokuString)) {
            std::cout << "Valid Sudoku puzzle." << std::endl;
            sudokuBoard->print();
        } else {
            std::cerr << "Invalid Sudoku puzzle." << std::endl;
            return 1;
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    
    return 0;
}
