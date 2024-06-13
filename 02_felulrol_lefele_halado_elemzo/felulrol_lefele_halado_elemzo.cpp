#include <iostream>
#include <string>
#include <cctype>

enum TokenType {
    NUMBER,
    PLUS,
    MULTIPLY,
    END
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
        
        throw std::runtime_error("Ismeretlen karakter");
    }

private:
    std::string text;
    size_t pos;
};

class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer(tokenizer) {
        currentToken = tokenizer.getNextToken();
    }
    
    int parse() {
        return expr();
    }

private:
    Tokenizer &tokenizer;
    Token currentToken;

    void eat(TokenType type) {
        if (currentToken.type == type) {
            currentToken = tokenizer.getNextToken();
        } else {
            throw std::runtime_error("Szintaktikai hiba");
        }
    }
    
    int expr() {
        int result = term();
        
        while (currentToken.type == PLUS) {
            eat(PLUS);
            result += term();
        }
        
        return result;
    }

    int term() {
        int result = factor();
        
        while (currentToken.type == MULTIPLY) {
            eat(MULTIPLY);
            result *= factor();
        }
        
        return result;
    }
    
    int factor() {
        int result = currentToken.value;
        eat(NUMBER);
        return result;
    }
};

int main() {
    std::string text;
    std::cout << "Adjon meg egy kifejezést: ";
    std::getline(std::cin, text);
    
    Tokenizer tokenizer(text);
    Parser parser(tokenizer);
    
    try {
        int result = parser.parse();
        std::cout << "Eredmény: " << result << std::endl;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}

