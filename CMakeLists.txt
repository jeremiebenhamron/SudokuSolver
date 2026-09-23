cmake_minimum_required(VERSION 3.28)

project(SudokuSolver LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(SudokuSolver 
    main.cpp 
    SudokuBoard.cpp
    SudokuBoard.hpp 
    SudokuSolver.cpp
    SudokuSolver.hpp
)

target_compile_features(SudokuSolver PRIVATE cxx_std_20)
