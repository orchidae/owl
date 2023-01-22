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

// number expr := number
std::unique_ptr<AST::Node> Parser::parseNumberExpression() {
    auto result = std::make_unique<AST::Number>(lexer_.getNumber());
    consume();
    return result;
}

// paren expr := '(' expression ')'
std::unique_ptr<AST::Node> Parser::parseParenExpression() {
    consume(); // '(' token
    auto expression = parseExpression();
    if (!expression) {
        return nullptr;
    }
    if (current_token_ != ')') {
        return error("Expected ')' token");
    }
    consume(); // ')' token
    return expression;
}

// identifier epxr := identifier
//                 := identifier '(' expression* ')'
std::unique_ptr<AST::Node> Parser::parseIdentifierExpression() {
    std::string identifier = lexer_.getIdentifier();
    if (current_token_ != '(') {
        return std::make_unique<AST::Variable>(identifier);
    }

    consume(); // '(' token
}
