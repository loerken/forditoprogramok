// Fordítóprogramok 3.feladat - Általános alulról felfelé haladó elemző
// A lenti program működése alapvetően nagyon hasonló az előző feladatban is
// használt egyszerű aritmetikai kifejezéseket feldolgozó elemzőhöz, azonban
// ugyanazt az eredményt alulról-felfelé haladó elemzéssel éri el

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <stdexcept>

enum TokenType {
    NUMBER,
    PLUS,
    MULTIPLY,
    END,
    INVALID
};

struct Token {
    TokenType type;
    int value;
};

class Tokenizer {
public:
    Tokenizer(const std::string &text) : text(text), pos(0) {}

    Token getNextToken() {
        while (pos < text.length() && isspace(text[pos])) {
            pos++;
        }

        if (pos == text.length()) {
            return {END, 0};
        }

        char currentChar = text[pos];

        if (isdigit(currentChar)) {
            int value = 0;
            while (pos < text.length() && isdigit(text[pos])) {
                value = value * 10 + (text[pos] - '0');
                pos++;
            }
            return {NUMBER, value};
        }

        if (currentChar == '+') {
            pos++;
            return {PLUS, 0};
        }

        if (currentChar == '*') {
            pos++;
            return {MULTIPLY, 0};
        }

        return {INVALID, 0};
    }

private:
    std::string text;
    size_t pos;
};

struct Rule {
    int lhs; 
    int length;
};

const std::vector<Rule> rules = {
    {1, 3}, 
    {1, 1}, 
    {2, 3}, 
    {2, 1}, 
    {3, 1}  
};

struct Action {
    enum Type { SHIFT, REDUCE, ACCEPT, ERROR } type;
    int stateOrRule;
};

std::map<std::pair<int, TokenType>, Action> createActionTable() {
    std::map<std::pair<int, TokenType>, Action> actionTable;
    actionTable[{0, NUMBER}] = {Action::SHIFT, 5};
    actionTable[{0, PLUS}] = {Action::SHIFT, 4};
    actionTable[{1, END}] = {Action::ACCEPT, 0};
    actionTable[{2, PLUS}] = {Action::SHIFT, 6};
    actionTable[{2, END}] = {Action::REDUCE, 2};
    actionTable[{3, MULTIPLY}] = {Action::SHIFT, 7};
    actionTable[{3, PLUS}] = {Action::REDUCE, 4};
    actionTable[{3, END}] = {Action::REDUCE, 4};
    actionTable[{4, PLUS}] = {Action::SHIFT, 6};
    actionTable[{4, END}] = {Action::REDUCE, 1};
    actionTable[{5, MULTIPLY}] = {Action::REDUCE, 5};
    actionTable[{5, PLUS}] = {Action::REDUCE, 5};
    actionTable[{5, END}] = {Action::REDUCE, 5};
    actionTable[{6, NUMBER}] = {Action::SHIFT, 5};
    actionTable[{7, NUMBER}] = {Action::SHIFT, 5};
    return actionTable;
}

std::map<std::pair<int, int>, int> createGotoTable() {
    std::map<std::pair<int, int>, int> gotoTable;
    gotoTable[{0, 1}] = 1; // E
    gotoTable[{0, 2}] = 2; // T
    gotoTable[{0, 3}] = 3; // F
    gotoTable[{6, 2}] = 2; // T
    gotoTable[{7, 3}] = 3; // F
    return gotoTable;
}

class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer(tokenizer), actionTable(createActionTable()), gotoTable(createGotoTable()) {}

    int parse() {
        std::stack<int> stateStack;
        std::stack<int> valueStack;

        stateStack.push(0);
        Token token = tokenizer.getNextToken();

        while (true) {
            int currentState = stateStack.top();
            auto actionIter = actionTable.find({currentState, token.type});

            if (actionIter == actionTable.end()) {
                throw std::runtime_error("Syntax error");
            }

            Action action = actionIter->second;
            switch (action.type) {
                case Action::SHIFT:
                    stateStack.push(action.stateOrRule);
                    valueStack.push(token.value);
                    token = tokenizer.getNextToken();
                    break;
                case Action::REDUCE: {
                    const Rule &rule = rules[action.stateOrRule];
                    for (int i = 0; i < rule.length; ++i) {
                        stateStack.pop();
                    }
                    int lhs = rule.lhs;
                    auto gotoIter = gotoTable.find({stateStack.top(), lhs});
                    if (gotoIter == gotoTable.end()) {
                        throw std::runtime_error("Syntax error");
                    }
                    stateStack.push(gotoIter->second);
                    // Számítás a redukció alapján
                    int result = 0;
                    if (rule.length == 3) {
                        int right = valueStack.top(); valueStack.pop();
                        int left = valueStack.top(); valueStack.pop();
                        if (rule.lhs == 1) { // E -> E + T
                            result = left + right;
                        } else if (rule.lhs == 2) { // T -> T * F
                            result = left * right;
                        }
                    } else if (rule.length == 1) {
                        result = valueStack.top();
                    }
                    valueStack.push(result);
                    break;
                }
                case Action::ACCEPT:
                    return valueStack.top();
                case Action::ERROR:
                    throw std::runtime_error("Syntax error");
            }
        }
    }

private:
    Tokenizer &tokenizer;
    std::map<std::pair<int, TokenType>, Action> actionTable;
    std::map<std::pair<int, int>, int> gotoTable;
};

// Fő program
int main() {
    std::string text;
    std::cout << "Enter an expression: ";
    std::getline(std::cin, text);

    Tokenizer tokenizer(text);
    Parser parser(tokenizer);

    try {
        int result = parser.parse();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
