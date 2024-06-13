// Fordítóprogramok 4.feladat - LL1 elemző
// A lenti program egy egyszerű LL1 elemzőt valósít meg, mely egy adott
// jelen esetben nagyon egyszerű nyelvtan alapján vizsgálja meg az 'a' és 'b'
// karakterekből álló bemenet szintaktikai helyességét. A Lexer osztály a 
// bemenetet tokenekre bontja, a Parser pedig elemzi azok sorrendjét.


#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>

class Token {
public:
    enum Type {
        T_A,
        T_B,
        T_EOF,
        T_ERROR
    };

    Token(Type type) : type(type) {}
    Token(Type type, char value) : type(type), value(value) {}

    Type type;
    char value;
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), position(0) {}

    Token getNextToken() {
        if (position >= input.size()) {
            return Token(Token::T_EOF);
        }

        char currentChar = input[position++];

        if (currentChar == 'a') {
            return Token(Token::T_A, currentChar);
        } else if (currentChar == 'b') {
            return Token(Token::T_B, currentChar);
        } else {
            return Token(Token::T_ERROR, currentChar);
        }
    }

private:
    std::string input;
    size_t position;
};

class Parser {
public:
    Parser(const std::string& input) : lexer(input), currentToken(lexer.getNextToken()) {}

    void parse() {
        S();
        if (currentToken.type == Token::T_EOF) {
            std::cout << "Input elfogadva.\n";
        } else {
            std::cout << "Szintaktikai hiba!\n";
        }
    }

private:
    Lexer lexer;
    Token currentToken;

    void S() {
        if (currentToken.type == Token::T_A) {
            match(Token::T_A);
            B();
        } else {
            error();
        }
    }

    void B() {
        if (currentToken.type == Token::T_B) {
            match(Token::T_B);
        } else {
        }
    }

    void match(Token::Type expected) {
        if (currentToken.type == expected) {
            currentToken = lexer.getNextToken();
        } else {
            error();
        }
    }

    void error() {
        std::cout << "Szintaktikai hiba!\n";
        exit(1);
    }
};

int main() {
    std::string input;
    std::cout << "Adja meg a bemenetet: ";
    std::cin >> input;

    Parser parser(input);
    parser.parse();

    return 0;
}
