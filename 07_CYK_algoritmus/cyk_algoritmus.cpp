// Fordítóprogramok 7.feladat - CYK algoritmus
// A lenti program a CYK algoritmust használja egy input string elemzésére
// egy Chomsky normál formájú nyelvtan alapján.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Grammar {
public:
    std::unordered_map<std::string, std::vector<std::string>> rules;

    void addRule(const std::string& lhs, const std::string& rhs) {
        rules[lhs].push_back(rhs);
    }

    bool produces(const std::string& lhs, const std::string& rhs) {
        if (rules.find(lhs) != rules.end()) {
            for (const std::string& rule : rules[lhs]) {
                if (rule == rhs) {
                    return true;
                }
            }
        }
        return false;
    }
};

bool cykParse(const std::string& input, Grammar& grammar) {
    int n = input.size();
    std::vector<std::vector<std::unordered_set<std::string>>> dp(n, std::vector<std::unordered_set<std::string>>(n));

    for (int i = 0; i < n; ++i) {
        std::string sym(1, input[i]);
        for (const auto& rule : grammar.rules) {
            const std::string& lhs = rule.first;
            const std::vector<std::string>& rhsList = rule.second;
            for (const std::string& rhs : rhsList) {
                if (rhs == sym) {
                    dp[i][0].insert(lhs);
                }
            }
        }
    }

    for (int length = 2; length <= n; ++length) {
        for (int i = 0; i <= n - length; ++i) {
            for (int k = 1; k < length; ++k) {
                for (const auto& rule : grammar.rules) {
                    const std::string& lhs = rule.first;
                    const std::vector<std::string>& rhsList = rule.second;
                    for (const std::string& rhs : rhsList) {
                        if (rhs.size() == 2) {
                            std::string B = rhs.substr(0, 1);
                            std::string C = rhs.substr(1, 1);

                            if (dp[i][k-1].count(B) && dp[i+k][length-k-1].count(C)) {
                                dp[i][length-1].insert(lhs);
                            }
                        }
                    }
                }
            }
        }
    }

    return dp[0][n-1].count("S") > 0;
}

int main() {
    Grammar grammar;
    grammar.addRule("S", "AB");
    grammar.addRule("S", "BC");
    grammar.addRule("A", "BA");
    grammar.addRule("A", "a");
    grammar.addRule("B", "CC");
    grammar.addRule("B", "b");
    grammar.addRule("C", "AB");
    grammar.addRule("C", "a");

    std::string input = "baaba";
    if (cykParse(input, grammar)) {
        std::cout << "Elfogadva!\n";
    } else {
        std::cout << "Elutasítva!\n";
    }

    return 0;
}
