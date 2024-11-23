#include <iostream>
#include <string>
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

bool DPLL::allClausesSatisfied(const std::unordered_map<int, bool>& model) {
    for(const auto& clause : cnf) {
        bool clauseSatisfied = false;
        for(const int literal : clause) {
            bool negated = literal < 0;
            int symbol = abs(literal);
            if (model.find(symbol) != model.end() && model.at(symbol) != negated) {
                clauseSatisfied = true;
                break;
            }
        }
        if(!clauseSatisfied) return false;
    }    
    return true;
}

bool DPLL::someClauseFalsified(const std::unordered_map<int, bool>& model) {
    for(const auto& clause : cnf) {
        bool clauseSatisfied = false;
        bool allLiteralsAssigned = true;

        for(const int literal : clause) {
            bool negated = literal < 0;
            int symbol = abs(literal);
            if(model.find(symbol) != model.end()) {
                if(model.at(symbol) != negated) {
                    clauseSatisfied = true;
                    break;
                }
            }else {
                allLiteralsAssigned = false;
                break;
            }
        }

        //if every literal is assigned a value and the clause is not satisfied,
        //some clause was falsified
        if(allLiteralsAssigned && !clauseSatisfied) {
            return true;
        }
    }
    return false;
}

int DPLL::findUnitClause(const std::unordered_map<int, bool>& model) {
    for (const auto& clause : cnf) {
        int unassignedLiteral = 0;
        bool foundUnassigned = false;
        bool clauseSatisfied = false;

        for (int literal : clause) {
            bool negated = literal < 0;
            int symbol = abs(literal);

            // Check if the literal is assigned in the model
            if (model.find(symbol) != model.end()) {
                if (model.at(symbol) != negated) {
                    clauseSatisfied = true;
                    break;  // Clause is already satisfied
                }
            } else {
                // Found an unassigned literal
                if (foundUnassigned) {
                    // More than one unassigned literal, so not a unit clause
                    foundUnassigned = false;
                    break;
                }
                foundUnassigned = true;
                unassignedLiteral = literal;
            }
        }

        // If it's a unit clause and not already satisfied, return the literal
        if (foundUnassigned && !clauseSatisfied) {
            return unassignedLiteral; // Positive if true, negative if false
        }
    }

    // If no unit clause is found, return 0 (indicating no unit clause)
    return 0;
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

    int P = findUnitClause(model);
    if(P!= 0) {
        bool sign = P > 0;
        model[abs(P)] = sign;
        return search(model, literals);
    }

    P = literals.back();
    literals.pop_back();

    model[P] = true;
    bool P_true = search(model, literals);
    if(P_true) {
        return true;
    }

    model[P] = false;
    bool P_false = search(model, literals);

    return P_false;
}
