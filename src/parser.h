#pragma once

#include <utility>

#include "ast.h"
#include "lexer.h"


class Parser {
public:
    explicit Parser(Lexer  lexer) : lexer_(std::move(lexer)) {}

    int consume();
    std::unique_ptr<AST::Node> error(const std::string& str);

private:
    Lexer lexer_;
    int current_token_ = 0;
};
