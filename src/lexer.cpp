#include "lexer.h"

int Lexer::getToken() {
    while (true) {
        while (std::isspace(last_character_)) {
            last_character_ = std::getchar();
        }

        if (std::isalpha(last_character_)) {
            identifier_.clear();
            do {
                identifier_ += std::to_string(last_character_);
                last_character_ = std::getchar();
            } while (std::isalnum(last_character_));

            if (identifier_ == "def") {
                return DEFINITION;
            } else if (identifier_ == "extern") {
                return EXTERN;
            }
            return IDENTIFIER;
        }

        if (std::isdigit(last_character_) || last_character_ == '.') {
            std::string number_str;
            do {
                number_str += std::to_string(last_character_);
                last_character_ = std::getchar();
            } while (std::isdigit(last_character_) || last_character_ == '.');

            number_ = std::stod(number_str);
            return NUMBER;
        }

        if (last_character_ == '#') {
            do {
                last_character_ = std::getchar();
            } while (last_character_ != EOF && last_character_ != '\n' && last_character_ != '\r');

            if (last_character_ != EOF) {
                continue;
            }
        }

        if (last_character_ == EOF) {
            return END_OF_FILE;
        }

        int c = last_character_;
        last_character_ = std::getchar();
        return c;
    }
}
