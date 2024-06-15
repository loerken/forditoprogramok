// Fordítóprogramok 6.feladat - Általános LL1 elemző
// A lenti program egy nagyon egyszerű általános LL1 elemzőt valósít meg, mely aritmetikai kifejezések
// helyességét vizsgálja egy előre meghatározott nyelvtan alapján. Hasonlóan az előzőekhez első lépésben
// tokenekre bontja a bemeneti stringet és verem alapú algoritmust használva megvizsgálja, hogy az adott
// bemeneti string megfelel-e a nyelvtan szabályainak.

#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <vector>

enum TokenType { ID, PLUS, MUL, LPAREN, RPAREN, END };

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    for (size_t i = 0; i < input.length(); ++i) {
        if (isdigit(input[i])) {
            tokens.push_back({ ID, std::string(1, input[i]) });
        } else if (input[i] == '+') {
            tokens.push_back({ PLUS, "+" });
        } else if (input[i] == '*') {
            tokens.push_back({ MUL, "*" });
        } else if (input[i] == '(') {
            tokens.push_back({ LPAREN, "(" });
        } else if (input[i] == ')') {
            tokens.push_back({ RPAREN, ")" });
        }
    }
    tokens.push_back({ END, "" });
    return tokens;
}

std::map<std::string, std::map<TokenType, std::vector<std::string>>> parseTable = {
    { "E", { {ID, {"T", "E'"}}, {LPAREN, {"T", "E'"}} } },
    { "E'", { {PLUS, {"+", "T", "E'"}}, {RPAREN, {""}}, {END, {""}} } },
    { "T", { {ID, {"F", "T'"}}, {LPAREN, {"F", "T'"}} } },
    { "T'", { {PLUS, {""}}, {MUL, {"*", "F", "T'"}}, {RPAREN, {""}}, {END, {""}} } },
    { "F", { {ID, {"id"}}, {LPAREN, {"(", "E", ")"}} } }
};

bool parse(const std::vector<Token>& tokens) {
    std::stack<std::string> parseStack;
    parseStack.push("E");
    size_t i = 0;

    while (!parseStack.empty()) {
        std::string top = parseStack.top();
        parseStack.pop();

        if (top == "id" || top == "+" || top == "*" || top == "(" || top == ")") {
            if (top == tokens[i].value) {
                i++;
            } else {
                return false;
            }
        } else if (parseTable.find(top) != parseTable.end()) {
            if (parseTable[top].find(tokens[i].type) != parseTable[top].end()) {
                const std::vector<std::string>& rule = parseTable[top][tokens[i].type];
                for (auto it = rule.rbegin(); it != rule.rend(); ++it) {
                    if (!it->empty()) {
                        parseStack.push(*it);
                    }
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return tokens[i].type == END;
}

int main() {
    std::string input = "id+id*id";
    std::vector<Token> tokens = tokenize(input);

    if (parse(tokens)) {
        std::cout << "Az input string megfelel a megadott nyelvtan szabályainak." << std::endl;
    } else {
        std::cout << "Az input string nem felel meg a megadott nyelvtan szabályainak." << std::endl;
    }
    
    return 0;
}
