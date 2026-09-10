#include "Lexer.h"
#include <stdexcept>


Lexer::Lexer(std::string source)    // constructor 
{
    this->source = source;
    this->position = 0;
}

std::vector<Token> Lexer::Tokenize(){
    std::vector<Token> tokens;  // Empty vector of tokens

    while (position < (int)source.length()) // loops for length of string/input
    {
        char current = source[position];    // Current position  
        if (std::isspace(current))
        {
            // If its a space, move on
            position++;
            continue;
        }

        if (std::isdigit(current))
        {
            // Reads number and pushes it onto vector
            tokens.push_back(ReadNumber());
            continue;
        }

        if (std::isalpha(current))
        {
            // Reads token and works out what operator ir is before pushing to vector
            tokens.push_back(ReadIdOrKey());
            continue;
        }

        if (current == '(')
        {
            // Checks if its a left bracket and pushes
            tokens.push_back(Token(TokenType::LeftParen, "("));
            position++;
            continue;
        }

        if (current == ')')
        {
            // Checks if its a left bracket and pushes
            tokens.push_back(Token(TokenType::RightParen, ")"));
            position++;
            continue;
        }

        // If its not an expected symbol
        throw std::runtime_error("Lexical error: unexpected character");
    }

    //When while loop finishes, implement the end of file token and return
    tokens.push_back(Token(TokenType::EndOfFile, ""));
    return tokens;
}

Token Lexer::ReadNumber()
    {

        int start = position; // Start of a potentially multi digit value

        // Reads the whole token length (like if its multi digit)
        while (position < (int)source.length() && std::isdigit(source[position]))
        {
            position++;
        }

        // Uses these positions found (for start and end) to determine value
        std::string value = source.substr(start, position - start);

        // Creates token of number with a value and returns it
        return Token(TokenType::Number, value);
    }

Token Lexer::ReadIdOrKey()
    {
        // Same as above
        int start = position;
        while (position < (int)source.length() && std::isalnum(source[position]))
        {
            position++;
        }

        // Determines what the string is, like above
        std::string value = source.substr(start, position - start);

        // Checks which of the operations it is and returns the token
        if (value == "add") return Token(TokenType::Add, value);
        if (value == "sub") return Token(TokenType::Sub, value);
        if (value == "mul") return Token(TokenType::Mul, value);
        if (value == "div") return Token(TokenType::Div, value);
        if (value == "mod") return Token(TokenType::Mod, value);

        // Failsafe to treat as an identifier if it doesnt hit any of the above
        return Token(TokenType::Identifier, value);
    }