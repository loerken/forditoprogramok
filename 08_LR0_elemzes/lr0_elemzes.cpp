#include <iostream>
#include <vector>
#include <stack>
#include <string>

struct Transition {
    char symbol;
    int target_state;
};

class LR0Parser {
private:
    std::vector<std::vector<std::string>> productions; 
    std::vector<std::vector<Transition>> transitions;

public:
    LR0Parser(std::vector<std::vector<std::string>> productions) {
        this->productions = productions;
        transitions.resize(productions.size());

        for (int i = 0; i < productions.size(); ++i) {
            int dot_position = 0;
            for (int j = 0; j <= productions[i].size(); ++j) {
                if (j == productions[i].size() || productions[i][j] == "|") {
                    transitions[i].push_back(Transition{productions[i][dot_position][0], i});
                    dot_position = j + 1;
                }
            }
        }
    }

    bool parse(std::string input) {
        std::stack<int> state_stack;
        state_stack.push(0);
        int input_index = 0;

        while (input_index <= input.size()) {
            int current_state = state_stack.top();
            char current_symbol = (input_index < input.size()) ? input[input_index] : '$';

            bool transition_found = false;
            for (const Transition& transition : transitions[current_state]) {
                if (transition.symbol == current_symbol) {
                    state_stack.push(transition.target_state);
                    transition_found = true;
                    break;
                }
            }

            if (!transition_found) {
                return false;
            }

            if (current_symbol != '$') {
                ++input_index;
            }
        }

        return true;
    }
};

int main() {
    std::vector<std::vector<std::string>> productions = {
        {"S", "E"},
        {"E", "E", "+", "T"},
        {"E", "T"},
        {"T", "T", "*", "F"},
        {"T", "F"},
        {"F", "(", "E", ")"},
        {"F", "id"}
    };

    LR0Parser parser(productions);

    std::vector<std::string> inputs = {"id+id*id", "id+id*", "id*id", "id"};
    for (const std::string& input : inputs) {
        std::cout << "Bemenet: " << input << ", Eredmény: " << (parser.parse(input) ? "Elfogadva" : "Elutasítva") << std::endl;
    }

    return 0;
}
