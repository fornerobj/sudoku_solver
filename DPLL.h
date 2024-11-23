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
    bool isClauseSatisfied(const std::vector<int>& clause, const std::unordered_map<int, bool>& model);
    bool allClausesSatisfied(const std::unordered_map<int, bool>&);
    bool someClauseFalsified(const std::unordered_map<int, bool>&);
    int findUnitClause(const std::unordered_map<int, bool>&);
    int findPureSymbol(const std::unordered_map<int, bool>& model, const std::vector<int>& literals);
    bool search(std::unordered_map<int, bool>, std::vector<int>);
    void printModel(const std::unordered_map<int,bool>&);
};
