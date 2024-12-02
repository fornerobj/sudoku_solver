#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "DPLL.h"
using std::vector, std::unordered_map, std::string, std::ifstream, std::istringstream;

DPLL::DPLL(const string& filename, std::vector<std::string> extraLiterals = {}): cnf(), literalMap(),reverseLiteralMap(),
    literals(), model(), nextLiteralID(1), numCalls(0){
    ifstream ifs(filename);
    string line;

    if(!ifs) {
        throw std::invalid_argument("Unable to open file " + filename);
    }
    
    while(std::getline(ifs, line)) {
        if(line.empty() || line[0] == '#') {
            continue;
        }

        istringstream stream(line);
        vector<int> clause;
        string literal;

        while(stream >> literal) {
            bool negated = (literal[0] == '-');
            string literalName = negated ? literal.substr(1) : literal;
            
            int id = getLiteralAsInt(literalName);

            clause.push_back(negated ? -id : id);
        }

        if(!clause.empty()) {
            cnf.push_back(clause);
        }
    } 
    while(!extraLiterals.empty()) {
        vector<int> clause;
        string lit = extraLiterals.back();
        extraLiterals.pop_back();
        bool negated = (lit[0] == '-');
        string literalName = negated ? lit.substr(1) : lit;

        int id = getLiteralAsInt(literalName);
        clause.push_back(id);
        cnf.push_back(clause);

    }

    ifs.close();
}

void DPLL::printModel(const std::unordered_map<int, bool>& model) {
    std::cout << "model: {";
    bool first = true;
    for (const auto& pair : model) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;

        // Convert the integer literal to its string representation
        std::string literalName = reverseLiteralMap[pair.first];
        int value = pair.second ? 1 : 0;  // Convert bool to 1 or 0

        // Print the literal and its value
        std::cout << "'" << literalName << "': " << value;
    }
    std::cout << "}" << std::endl;
}

int DPLL::getLiteralAsInt(const std::string& literal) {
    auto it = literalMap.find(literal);
    if(it == literalMap.end()) {
        literalMap[literal] = nextLiteralID;
        reverseLiteralMap[nextLiteralID] = literal;
        literals.push_back(nextLiteralID);
        return nextLiteralID++;
    }
    return it->second;
}

bool DPLL::isClauseSatisfied(const vector<int>& clause, const std::unordered_map<int, bool>& model) {
    for (int literal : clause) {
        int symbol = abs(literal);
        bool negated = literal < 0;

        if (model.find(symbol) != model.end()) {
            if (model.at(symbol) != negated) {
                return true;
            }
        }
    }
    return false;
}

bool DPLL::allClausesSatisfied(const std::unordered_map<int, bool>& model) {
    for(const auto& clause : cnf) {
        if(!isClauseSatisfied(clause, model)) {
            return false;
        }
    }    
    return true;
}

bool DPLL::someClauseFalsified(const std::unordered_map<int, bool>& model) {
    for(const auto& clause : cnf) {
        bool clauseSatisfied = isClauseSatisfied(clause, model);
        bool allLiteralsAssigned = true;

        for(const int literal : clause) {
            int symbol = abs(literal);
            if(model.find(symbol) == model.end()) {
                allLiteralsAssigned = false;
                break;
            }        
        }

        if(allLiteralsAssigned && !clauseSatisfied) {
            return true;
        }
    }
    return false;
}

int DPLL::findUnitClause(const std::unordered_map<int, bool>& model) {
    for (const auto& clause : cnf) {
        if(isClauseSatisfied(clause, model)) {
            continue;
        }

        int unassignedLiteral = 0;
        bool foundUnassigned = false;


        for (int literal : clause) {
            int symbol = abs(literal);

            if(model.find(symbol) == model.end()) {
                if(foundUnassigned) {
                    unassignedLiteral = 0;
                    foundUnassigned = false;
                    break;
                }else {
                    foundUnassigned = true;
                    unassignedLiteral = literal;
                }
            }
        }

        if (foundUnassigned) {
            return unassignedLiteral;
        }
    }

    return 0;
}
int DPLL::findPureSymbol(const std::unordered_map<int, bool>& model, const std::vector<int>& literals) {
    std::unordered_map<int, bool> polarity;
    std::unordered_map<int, bool> isPure;

    for (int literal : literals) {
        polarity[literal] = true;
        isPure[literal] = true;
    }

    for (const auto& clause : cnf) {
        if (isClauseSatisfied(clause, model)) {
            continue;
        }

        for (int literal : clause) {
            int symbol = abs(literal);
            bool negated = literal < 0;

            if (isPure.find(symbol) != isPure.end()) {
                if (polarity[symbol] != !negated) {
                    isPure[symbol] = false;
                }
            }
        }
    }

    for (int literal : literals) {
        if (isPure[literal]) {
            return polarity[literal] ? literal : -literal;
        }
    }

    return 0;
}

int DPLL::findMOMLiteral(const std::unordered_map<int, bool>& model, const std::vector<int>& literals) {
    std::unordered_map<int, int> frequency;
    int maxFrequency = 0;
    int momLiteral = 0;
    
    for (const auto& clause : cnf) {
        if (isClauseSatisfied(clause, model)) {
            continue;
        }

        for (int literal : clause) {
            int symbol = abs(literal);
            if (model.find(symbol) == model.end()) {
                frequency[symbol]++;
                if(frequency[symbol] > maxFrequency) {
                    maxFrequency = frequency[symbol];
                    momLiteral = symbol;
                }
            }
        }
    }

    
    return momLiteral;
}

bool DPLL::search(std::unordered_map<int, bool> model, std::vector<int> literals) {
    numCalls++;
    if (this->allClausesSatisfied(model)){
        this->model = model;
        return true;
    }
    if (this->someClauseFalsified(model)){
        return false;
    }

    // commented out for now becuase it negatively impacts performance
    // maybe not suited for sudoku puzzles with very few pure symbols
    // int P = findPureSymbol(model, literals);
    // if(P!= 0) {
    //     bool sign = P > 0;
    //     model[abs(P)] = sign;
    //     literals.erase(std::remove(literals.begin(), literals.end(), abs(P)), literals.end());
    //     return search(model, literals);
    // }

    int P = findUnitClause(model);
    if(P!= 0) {
        bool sign = P > 0;
        model[abs(P)] = sign;
        literals.erase(std::remove(literals.begin(), literals.end(), abs(P)), literals.end());
        return search(model, literals);
    }

    P = findMOMLiteral(model, literals);
    literals.erase(std::remove(literals.begin(), literals.end(), abs(P)), literals.end());

    model[P] = true;
    bool P_true = search(model, literals);
    if(P_true) {
        return true;
    }

    model[P] = false;
    return search(model, literals);
}
