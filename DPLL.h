#include <vector>
#include <unordered_map>
#include <string>

class DPLL {
public:
    std::vector<std::vector<int>> cnf;
    std::unordered_map<std::string, int> literalMap;
    std::unordered_map<int, std::string> reverseLiteralMap;
    std::vector<int> literals;
    std::unordered_map<int, bool> model;
    int nextLiteralID;
    size_t numCalls;

    DPLL(const std::string& filename, std::vector<std::string> extraLiterals);
    int getLiteralAsInt(const std::string& literal);
    bool allClausesSatisfied(std::unordered_map<int, bool>);
    bool someClauseFalsified(std::unordered_map<int, bool>);
    int findUnitClause(std::unordered_map<int, bool>);
    bool search(std::unordered_map<int, bool>, std::vector<int>);
    void printModel(std::unordered_map<int,bool>);
};
