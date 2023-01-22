#pragma once

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

private:
    Lexer lexer_;
    int current_token_ = 0;
};
