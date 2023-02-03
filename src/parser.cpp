#include <cctype>
#include <iostream>

#include "lexer.h"
#include "parser.h"

int Parser::consume() {
    current_token_ = lexer_.getToken();
    return current_token_;
}

std::unique_ptr<AST::Node> Parser::error(const std::string &str) {
    std::cerr << "Error: " << str << std::endl;
    return nullptr;
}

// number-expr := number
std::unique_ptr<AST::Node> Parser::parseNumberExpression() {
    auto result = std::make_unique<AST::Number>(lexer_.getNumber());
    consume();
    return result;
}

// paren-expr := '(' expression ')'
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

// identifier-expr := identifier
//                 := identifier '(' expression* ')'
std::unique_ptr<AST::Node> Parser::parseIdentifierExpression() {
    std::string identifier = lexer_.getIdentifier();
    if (current_token_ != '(') {
        return std::make_unique<AST::Variable>(identifier);
    }

    consume(); // '(' token
    std::vector<std::unique_ptr<AST::Node>> arguments;
    if (current_token_ != ')') {
        while (true) {
            auto expression = parseExpression();
            if (expression != nullptr) {
                arguments.push_back(std::move(expression));
            } else {
                return nullptr;
            }

            if (current_token_ == ')') {
                break;
            }
            if (current_token_ != ',') {
                return error("Expected ')', or ',' in argument list");
            }
            consume(); // ',' token
        }
    }
    consume(); // ')' token

    return std::make_unique<AST::Call>(identifier, std::move(arguments));
}

std::unique_ptr<AST::Node> Parser::parsePrimary() {
    switch (current_token_) {
        case Lexer::IDENTIFIER:
            return parseIdentifierExpression();
        case Lexer::NUMBER:
            return parseNumberExpression();
        case '(':
            return parseParenExpression();
        default:
            return error("Expected an expression, got unknown token");
    }
}

std::unique_ptr<AST::Node> Parser::parseExpression() {
    return std::unique_ptr<AST::Node>();
}

int Parser::precedence(char token) {
    if (isascii(token)) {
        return -1;
    }

    int precedence = precedence_map_[token];
    return (precedence > 0) ? precedence : -1;
}
