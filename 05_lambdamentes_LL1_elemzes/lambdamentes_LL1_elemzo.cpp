// Fordítóprogramok 5.feladat - Lambdamentes LL1 elemző
// A lenti program egy nagyon egyszerű lambdamentes elemzőt valósít meg.
// Az elemző az input stringet a parsing táblázat és szabályok alapján elemzi,
// hogy megfelel-e a definiált nyelvtannak vagy sem. A parsing tábla és szabályok
// alapján a program végiglépkedi a bemenetet és meghatározza, hogy érvényes volt-e.

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class LL1Parser {
private:
    std::unordered_map<char, std::unordered_map<char, std::string>> parsing_table;
    std::vector<std::string> production_rules;
    std::string start_symbol;

public:
    LL1Parser(std::unordered_map<char, std::unordered_map<char, std::string>> table,
              std::vector<std::string> rules,
              std::string start)
        : parsing_table(table), production_rules(rules), start_symbol(start) {}

    void parse(const std::string& input) {
        std::string stack = start_symbol + "$";
        std::string remaining_input = input + "$";

        size_t input_index = 0;
        char current_input = remaining_input[input_index];
        char stack_top = stack.back();

        while (stack_top != '$') {
            if (stack_top == current_input) {
                stack.pop_back();
                input_index++;
                current_input = remaining_input[input_index];
            } else if (!is_terminal(stack_top)) {
                if (parsing_table.find(stack_top) == parsing_table.end()
                    || parsing_table[stack_top].find(current_input) == parsing_table[stack_top].end()) {
                    std::cerr << "Hiba: A bejegyzés nem található a táblában." << std::endl;
                    return;
                }

                std::string production = parsing_table[stack_top][current_input];
                stack.pop_back();
                if (production != "ε") {
                    for (int i = production.size() - 1; i >= 0; --i) {
                        stack.push_back(production[i]);
                    }
                }
            } else {
                std::cerr << "Hiba: Nem megfelelő illeszkedés az input és a verem szimbólumai között." << std::endl;
                return;
            }

            stack_top = stack.back();
        }

        if (current_input == '$') {
            std::cout << "Input elemzése sikeres!" << std::endl;
        } else {
            std::cerr << "Hiba: Az input teljes elemzése sikertelen." << std::endl;
        }
    }

private:
    bool is_terminal(char symbol) {
        return !(symbol >= 'A' && symbol <= 'Z');
    }
};

int main() {
    std::unordered_map<char, std::unordered_map<char, std::string>> parsing_table = {
        {'E', {{'(', "TX"}}},
        {'T', {{'(', "FY"}}},
        {'X', {{')', "ε"}, {'+', "+TX"}}},
        {'Y', {{')', "ε"}, {'*', "*FY"}}}
    };

    std::vector<std::string> production_rules = {"E->TX", "X->+TX|ε", "T->FY", "Y->*FY|ε"};

    LL1Parser parser(parsing_table, production_rules, "E");

    std::cout << "Adjon meg egy kifejezést: ";
    std::string input;
    std::getline(std::cin, input);

    parser.parse(input);

    return 0;
}
