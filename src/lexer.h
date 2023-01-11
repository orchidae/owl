#pragma once

#include <string>

class Lexer {
public:
    enum Token {
        END_OF_FILE = -1,
        DEFINITION = -2,
        EXTERN = -3,
        IDENTIFIER = -4,
        NUMBER = -5
    };

    int getToken();

    const std::string& getIdentifier() const {
        return identifier_;
    }

    double getNumber() const {
        return number_;
    }

private:
    int last_character_ = ' ';
    std::string identifier_;
    double number_;
};
