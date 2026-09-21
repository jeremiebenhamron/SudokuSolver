cmake_minimum_required(VERSION 3.28)

project(SudokuSolver LANGUAGES CXX)

add_executable(SudokuSolver main.cpp)

target_compile_features(SudokuSolver PRIVATE cxx_std_17)
