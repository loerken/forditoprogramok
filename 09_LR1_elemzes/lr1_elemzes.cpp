// Fordítóprogramok 9.feladat - LR1 elemzés
// A lenti program egy nagyon egyszerű LR1 elemzőt valósít meg
// mely adott nyelvtan alapján értékeli a bemenetként kapott 
// karaktertláncokat. Az elemzéshez táblákat, és a verem
// sajátosságait használja fel.

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <utility>

struct LR1TableEntry {
    char action;
    int nextState;
    int productionRule; 
};

class LR1Parser {
private:
    std::vector<std::map<char, int>> gotoTable;
    std::vector<LR1TableEntry> actionTable;      
    std::vector<std::pair<char, std::string>> grammar;

    std::stack<int> stateStack;
    std::stack<char> symbolStack;
    std::string input;

public:
    LR1Parser(std::vector<std::pair<char, std::string>> grammar)
        : grammar(grammar)
    {
        initializeTables();
    }

    void parse(std::string input) {
        this->input = input + "$"; 
        stateStack.push(0); 

        int inputIndex = 0;
        while (true) {
            int currentState = stateStack.top();
            char currentSymbol = input[inputIndex];

            LR1TableEntry& entry = actionTable[currentState];

            if (entry.action == 's') {
                stateStack.push(entry.nextState);
                symbolStack.push(currentSymbol);
                inputIndex++;
            } else if (entry.action == 'r') {
                int ruleIndex = entry.productionRule;
                char lhs = grammar[ruleIndex].first;
                std::string rhs = grammar[ruleIndex].second;

                for (size_t i = 0; i < rhs.length(); ++i) {
                    stateStack.pop();
                    symbolStack.pop();
                }

                symbolStack.push(lhs);

                currentState = stateStack.top();
                int gotoState = gotoTable[currentState][lhs];
                stateStack.push(gotoState);
            } else if (entry.action == 'a') {
                std::cout << "Bemeneti string elfogadva.\n";
                break;
            } else {
                std::cout << "Hiba: A bemenet nem elfogadható.\n";
                break;
            }
        }
    }

private:
    void initializeTables() {
        actionTable.resize(10); 
        gotoTable.resize(10); 

        actionTable[0] = { 's', 1, -1 }; 
        gotoTable[0]['A'] = 1;          
    }
};

int main() {
    std::vector<std::pair<char, std::string>> grammar = {
        {'A', "BC"},
        {'B', "b"},
        {'C', "c"}
    };

    LR1Parser parser(grammar);

    parser.parse("bbc");

    return 0;
}
