#include <iostream>
#include "parser.h"

int Parser::consume() {
    current_token_ = lexer_.getToken();
    return current_token_;
}

std::unique_ptr<AST::Node> Parser::error(const std::string &str) {
    std::cerr << "Error: " << str << std::endl;
    return nullptr;
}
