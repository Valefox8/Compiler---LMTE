#include "Lexer.h"
#include <stdexcept>


Lexer::Lexer(std::string source)    // constructor 
{
    this->source = source;
    this->position = 0;
    this->line = 0;
}

std::vector<Token> Lexer::Tokenize(){
    std::vector<Token> tokens;  // Empty vector of tokens

    while (position < (int)source.length()) // loops for length of string/input
    {
        char current = source[position];    // Current position  

        if (current == '\n')
        {
            // A new line starts after this character
            position++;
            line++;
            continue;
        }

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

        if (current == '<' && checkforward("<!--"))   // check for comment start
        {
            Comment();
            continue;
        }

        if (current == '-' && checkforward("->")) // check for the object arrow
        {
            tokens.push_back(Token(TokenType::Arrow, "->", line));
            position += 2; // two characters, not one
            continue;
        }

        if (current == '$')                        // Check for word start
        {
            tokens.push_back(ReadWord());           // call letter read
            continue;
        }

        if (current == '#')                        // check for letter start
        {
            tokens.push_back(ReadLetter());         // call letter read
            continue;
        }

        if (current == '=')                         // check for equals/assignment
        {
            tokens.push_back(Token(TokenType::Equals, "=", line));    // Push that token into token list
            position++;
            continue;
        }

        if (current == '(')
        {
            // Checks if its a left bracket and pushes
            tokens.push_back(Token(TokenType::LeftParen, "(", line));
            position++;
            continue;
        }

        if (current == ')')
        {
            // Checks if its a right bracket and pushes
            tokens.push_back(Token(TokenType::RightParen, ")", line));
            position++;
            continue;
        }

        if (current == '"')
        {
            // Checks if its a quote and pushes
            tokens.push_back(Token(TokenType::Quote, "\"", line));
            position++;
            continue;
        }

        if (current == '|')
        {
            // Checks if its a list item separator and pushes it
            tokens.push_back(Token(TokenType::Separator, "|", line));
            position++;
            continue;
        }

        if (current == '.')
        {
            // Checks if its a dot/full stop and pushes it 
            tokens.push_back(Token(TokenType::Dot, ".", line));
            position++;
            continue;
        }

        if (current == '[')
        {
            // check for [ and pushes it
            tokens.push_back(Token(TokenType::LeftBracket, "[", line));
            position++;
            continue;
        }

        if (current == ']')
        {
            // check for ] and pushes it
            tokens.push_back(Token(TokenType::RightBracket, "]", line));
            position++;
            continue;
        }

        if (current == ',')
        {
            // check for , and pushes it
            tokens.push_back(Token(TokenType::Comma, ",", line));
            position++;
            continue;
        }

        if (current == ':')
        {
            // check for : and pushes it
            tokens.push_back(Token(TokenType::Colon, ":", line));
            position++;
            continue;
        }

        if (current == ';')
        {
            // check for ; and pushes it
            tokens.push_back(Token(TokenType::Semicolon, ";", line));
            position++;
            continue;
        }

        // If its not an expected symbol
        throw std::runtime_error("Lexical error: unexpected character: " + current);
    }

    //When while loop finishes, implement the end of file token and return
    tokens.push_back(Token(TokenType::EndOfFile, "", line));
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
        return Token(TokenType::Number, value, line);
    }

    Token Lexer::ReadWord(){
        position++; // due to '$' at start of string
        int start = position;   // Start position of letter
        
        while (position < (int)source.length() && source[position] != '$')      // Finds end position of letter (position before '$')
        {
            position++;
        }

        if (position >= (int)source.length())
        {
            throw std::runtime_error("You didnt Addclose the word");     // Throws error if unclosed 
        }

        std::string value = source.substr(start, position - start);     // Finds value between start and end position
        position++; // goes past the end '$'
        return Token(TokenType::Word, value, line);     // Returns it
    }

    Token Lexer::ReadLetter(){
        position++; // due to '#' at start of string
        int start = position;   // Start position of letter
        
        while (position < (int)source.length() && source[position] != '#')      // Finds end position of letter (position before '#')
        {
            position++;
        }

        if (position >= (int)source.length())
        {
            throw std::runtime_error("You didnt close the letter");     // Throws error if unclosed 
        }

        std::string value = source.substr(start, position - start);     // Finds value between start and end position
        position++; // goes past the end '#'
        return Token(TokenType::Letter, value, line);     // Returns it
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
        if (value == "add") return Token(TokenType::Add, value, line);
        if (value == "sub") return Token(TokenType::Sub, value, line);
        if (value == "mul") return Token(TokenType::Mul, value, line);
        if (value == "div") return Token(TokenType::Div, value, line);
        if (value == "mod") return Token(TokenType::Mod, value, line);

        // Variable type identifiers
        if (value == "Vnum") return Token(TokenType::Vnum, value, line);
        if (value == "Vwords") return Token(TokenType::Vwords, value, line);
        if (value == "Vboolean") return Token(TokenType::Vboolean, value, line);
        if (value == "Vletter") return Token(TokenType::Vletter, value, line);
        if (value == "VCharacter") return Token(TokenType::VCharacter, value, line);
        if (value == "VDigit") return Token(TokenType::VDigit, value, line);

        // Real and cake identifiers
        if (value == "real") return Token(TokenType::Real, value, line);
        if (value == "cake") return Token(TokenType::Cake, value, line);

        // List
        if (value == "anotherone") return Token(TokenType::AnotherOne, value, line);

        // Iteration
        if (value == "forwhencake") return Token(TokenType::Forwhencake, value, line);
        if (value == "handbrake") return Token(TokenType::Handbrake, value, line);

        // Input and output keywords
        if (value == "say") return Token(TokenType::Say, value, line);
        if (value == "ask") return Token(TokenType::Ask, value, line);

        // Function keywords
        if (value == "FUNCTION") return Token(TokenType::Function, value, line);
        if (value == "leave") return Token(TokenType::Leave, value, line);
        if (value == "f") return Token(TokenType::Call, value, line);

        // Conditional Statement
        if (value == "iguessif") return Token(TokenType::IguessIf, value, line);
        if (value == "guessthis") return Token(TokenType::Guessthis, value, line);
        if (value == "guessnot") return Token(TokenType::Guessnot, value, line);

        // Comparison Operators
        if (value == "equal") return Token(TokenType::Equalto, value, line);
        if (value == "lessThan") return Token(TokenType::Lessthan, value, line);
        if (value == "moreThan") return Token(TokenType::Morethan, value, line);
        if (value == "isTotallyDefinitelyNotEqualTo") return Token(TokenType::Istotallydefinitelynotequalto, value, line);
        if (value == "equalLessThan") return Token(TokenType::Equallessthan, value, line);
        if (value == "equalMoreThan") return Token(TokenType::Equalmorethan, value, line);
        
        // Boolean Operator
        if (value == "dna") return Token(TokenType::Dna, value, line);
        if (value == "ro") return Token(TokenType::Ro, value, line);

        // Class keywords
        // if (value == "SELF") return Token(TokenType::SELF, value, line);
        // if (value == "selfish") return Token(TokenType::selfish, value);
        if (value == "public") return Token(TokenType::Public, value, line);
        if (value == "private") return Token(TokenType::Private, value, line);
        if (value == "protected") return Token(TokenType::Protected, value, line);


        // Failsafe to treat as an identifier if it doesnt hit any of the above
        return Token(TokenType::Identifier, value, line);
    }


    void Lexer::Comment(){
        position += 4;      // Check full "<!--"  

        while (position < (int)source.length() && !checkforward("-->"))   // keep moving until we find "-->"
        {
            position++;
        }

        if (position >= (int)source.length())   // ran out of source before finding "-->"
        {
            throw std::runtime_error("lexer failed on comment");
        }

    position += 3;   // finish the  "-->"
    }   
    
    bool Lexer::checkforward(std::string text){
    if (position + (int)text.length() > (int)source.length())   // not enough characters left
        {
            return false;
        }

        return source.substr(position, text.length()) == text;   // compare the chunk starting at position
    }   