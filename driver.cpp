#include <iostream>
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
    for (int i = 2; i < argc; ++i) {
        extraLiterals.push_back(argv[i]);
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
