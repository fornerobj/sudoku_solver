#include <iostream>
#include <sstream>
#include "DPLL.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input file> -[additional literals]" << std::endl;
        return -1;
    }

    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;

    const std::string filename = argv[1];

    // Collect additional literals if provided
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
    for(std::string str : extraLiterals) {
        std::cout << str << std::endl;
    }
    DPLL problem(filename, extraLiterals);
    if(problem.search({}, problem.literals)) {
        std::cout << "Solution found!" << std::endl;
        std::cout << "Just the true propositions:" << std::endl;
        for(auto mapping : problem.model) {
            if(mapping.second){
                std::cout << problem.reverseLiteralMap[mapping.first] << std::endl;
            }
        }
        std::cout << "\n";
    } else {
        std::cout << "No solution found :(" << std::endl;
    }

    std::cout << "Number of DPLL calls: " << problem.numCalls << std::endl;
 }
