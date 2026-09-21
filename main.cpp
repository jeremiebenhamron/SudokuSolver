#include <fstream>
#include <iostream>
#include <unordered_set>

std::string getSudokuPuzzle(const std::string& fileName) {
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

bool isValidSudoku(const std::string& sudokuString) {
    if(sudokuString.length() != 81)
    {
        std::cerr << "Sudoku must have 81 cells, but has: " << sudokuString.length() << " cells" << std::endl;
        return false;
    }

    std::array<int,81> sudokuArray;
    for (const auto& cellChar: sudokuString) {
        const int cellValue = cellChar - '0';
        if (!isValidCell(cellValue)) {
            std::cerr << "Found invalid cell value: " << cellChar << std::endl;
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sudoku_file.txt>" << std::endl;
        return 1;
    }

    std::string sudokuFileName(argv[1]);
    try {
        std::string sudokuPuzzle = getSudokuPuzzle(sudokuFileName);
        if (!isValidSudoku(sudokuPuzzle)) {
            std::cerr << "Invalid Sudoku puzzle." << std::endl;
            return 1;
        } else {
            std::cout << "Valid Sudoku puzzle." << std::endl;
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    
    return 0;
}



