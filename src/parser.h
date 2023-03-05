#pragma once

#include <map>
#include <utility>

#include "ast.h"
#include "lexer.h"


class Parser {
public:
    explicit Parser() {}

    void run();

private:
    void handleDefinition();
    void handleExtern();
    void handleTopLevelExpression();

    int consume();
    int precedence(int token);
    static std::unique_ptr<AST::Expression> error(const std::string& str);
    static std::unique_ptr<AST::Prototype> errorP(const std::string& str);

    std::unique_ptr<AST::Expression> parseNumberExpression();
    std::unique_ptr<AST::Expression> parseParenExpression();
    std::unique_ptr<AST::Expression> parseIdentifierExpression();
    std::unique_ptr<AST::Expression> parsePrimary();
    std::unique_ptr<AST::Expression> parseExpression();
    std::unique_ptr<AST::Expression> parseBinOpRhs(int expPrecedence, std::unique_ptr<AST::Expression> lhs);
    std::unique_ptr<AST::Prototype> parsePrototype();
    std::unique_ptr<AST::Prototype> parseExternal();
    std::unique_ptr<AST::Function> parseDefinition();
    std::unique_ptr<AST::Function> parseTopLevelFunction();


    Lexer lexer_;
    int current_token_ = 0;

    std::map<char, int> precedence_map_ = {
        {'<', 10},
        {'+', 20},
        {'-', 20},
        {'*', 40}
    };
};
