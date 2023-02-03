#pragma once

#include <map>
#include <utility>

#include "ast.h"
#include "lexer.h"


class Parser {
public:
    explicit Parser(Lexer lexer) : lexer_(std::move(lexer)) {}

    int consume();
    static std::unique_ptr<AST::Node> error(const std::string& str);

    std::unique_ptr<AST::Node> parseNumberExpression();
    std::unique_ptr<AST::Node> parseParenExpression();
    std::unique_ptr<AST::Node> parseIdentifierExpression();
    std::unique_ptr<AST::Node> parsePrimary();
    std::unique_ptr<AST::Node> parseExpression();

private:
    int precedence(char token);

    Lexer lexer_;
    int current_token_ = 0;

    std::map<char, int> precedence_map_ = {
        {'<', 10},
        {'+', 20},
        {'-', 20},
        {'*', 40}
    };
};
