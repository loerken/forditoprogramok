// Fordítóprogramok 1.feladat - Lexikális elemző

// A program egy egyszerű lexikális elemzőt valósít meg, mely
// soronként beolvassa az input.txt fájl tartalmát, majd feldolgozza
// és elemzi őket, úgy, hogy az elválasztók mentén szétválasztott kifejezések
// mellé a program futásakor kiírja, hogy mely kategóriákba tartoznak. 

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

bool isPunctuator(char ch);
bool validIdentifier(const std::string& str);
bool isOperator(char ch);
bool isKeyword(const std::string& str);
bool isNumber(const std::string& str);
std::string subString(const std::string& str, int l, int r);
void parse(const std::string& str);

int main() {
    std::ifstream inputFile("input.txt");

    if (!inputFile) {
        std::cerr << "Az input fájl megnyitása nem sikerült!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        parse(line);
    }

		//Megnyitott fájl bezárása
    inputFile.close();
    return 0;
}

bool isPunctuator(char ch) {
    return ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
           ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
           ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
           ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
           ch == '&' || ch == '|';
}

bool validIdentifier(const std::string& str) {
    if (str.empty() || std::isdigit(str[0]) || isPunctuator(str[0])) {
        return false;
    }

    for (char ch : str) {
        if (isPunctuator(ch)) {
            return false;
        }
    }

    return true;
}

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' ||
           ch == '/' || ch == '>' || ch == '<' ||
           ch == '=' || ch == '|' || ch == '&';
}

bool isKeyword(const std::string& str) {
    static const std::vector<std::string> keywords = {
        "if", "else", "while", "do", "break", "continue", "int", "double",
        "float", "return", "char", "case", "long", "short", "typedef",
        "switch", "unsigned", "void", "static", "struct", "sizeof", "long",
        "volatile", "typedef", "enum", "const", "union", "extern", "bool"
    };

    for (const std::string& keyword : keywords) {
        if (str == keyword) {
            return true;
        }
    }

    return false;
}

bool isNumber(const std::string& str) {
    int numOfDecimal = 0;
    if (str.empty()) {
        return false;
    }

    for (char ch : str) {
        if (ch == '.') {
            numOfDecimal++;
        } else if (!std::isdigit(ch) && !(ch == '-' && &ch == &str[0])) {
            return false;
        }

        if (numOfDecimal > 1) {
            return false;
        }
    }

    return true;
}


std::string subString(const std::string& str, int l, int r) {
    return str.substr(l, r - l + 1);
}

void parse(const std::string& str) {

    int left = 0, right = 0;
    int len = str.length();

    while (right <= len && left <= right) {
        if (right < len && !isPunctuator(str[right])) {
            right++;
        }

        if ((right < len && isPunctuator(str[right]) && left == right) || (right == len && left == right)) {
            if (right < len && isOperator(str[right])) {
                std::cout << str[right] << " -> OPERÁTOR\n";
            } else if (right < len && (str[right] == '{' || str[right] == '}')) {
                std::cout << str[right] << " -> ELVÁLASZTÓ\n";
            } else if (right < len && str[right] == ';') {
                std::cout << str[right] << " -> ELVÁLASZTÓ\n";
            }
            right++;
            left = right;
        } else if ((right < len && isPunctuator(str[right]) && left != right) || (right == len && left != right)) {
            std::string sub = subString(str, left, right - 1);

            if (!sub.empty()) {
                if (isKeyword(sub)) {
                    std::cout << sub << " -> KULCSSZÓ\n";
                } else if (isNumber(sub)) {
                    std::cout << sub << " -> ÉRVÉNYES SZÁM\n";
                } else if (validIdentifier(sub)) {
                    std::cout << sub << " -> ÉRVÉNYES KIFEJEZÉS\n";
                } else {
                    std::cout << sub << " -> ÉRVÉNYTELEN KIFEJEZÉS\n";
                }
            }

            left = right;
        }
    }
}


