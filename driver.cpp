#include <iostream>
#include <sstream>
#include "DPLL.h"

int main(int argc, char* argv[]) {

    const std::string filename = "./sudoku.cnf";

    std::cout << "Enter the starting layout of the puzzle, one row at a time" << std::endl;
    std::cout << "Use 0 for empty spaces" << std::endl;

    std::vector<std::string> extraLiterals;
    std::string line; 

    int row = 1;
    for(int i = 0; i < 9; i++) {
        std::cin >> line;

        if(line.size() != 9) {
            std::cout << "Invalid line length (must be 9)" << std::endl;
            i--;
            continue;
        }

        bool allNums = true;
        for (char c : line) {
            if(!isdigit(c)) {
                allNums = false;
                break;
            } 
        }
        if(!allNums) {
            std::cout << "The input line must be all numbers 0-9" << std::endl;
            i--;
            continue;
        }

        std::stringstream ss;
        int col = 1;
        for (char c : line) {
            if(c == '0'){
                col++;
                continue;
            }
            ss << "V" << row << col << c;
            extraLiterals.push_back(ss.str());
            ss.str("");
            col++;
        }
        row++;

    }

    DPLL problem(filename, extraLiterals);
    std::cout << "Thinking..." << std::endl;
    if(!problem.search({}, problem.literals)) {
        std::cout << "No solution found :(" << std::endl;
        return -1;
    }

    std::cout << "Solution found!\n" << std::endl;

    // std::vector<std::string> solution;
    char** solution = new char*[9];
    for(int i = 0; i < 9; i++) {
        solution[i] = new char[9];
    }

    for(auto mapping : problem.model) {
        if(mapping.second){
            std::string literal = problem.reverseLiteralMap[mapping.first];
            int row = (int)literal[1] - '0' - 1;
            int col = (int)literal[2] - '0' - 1;
            solution[row][col] = literal[3];
        }
    }

    for(int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << solution[i][j];
            if(j == 2 || j == 5) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
        if(i == 2 || i == 5) {
            std::cout << std::endl;
        }
    }
 }
