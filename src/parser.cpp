#include <cctype>
#include <iostream>

#include "lexer.h"
#include "parser.h"

void Parser::run() {
    std::cerr << ">>> ";
    consume();
    while (true) {
        switch (current_token_) {
            case Lexer::END_OF_FILE: {
                return;
            }
            case Lexer::DEFINITION:
                handleDefinition();
                break;
            case Lexer::EXTERN:
                handleExtern();
                break;
            case ';': {
                std::cerr << ">>> ";
                consume();
                break;
            }
            default: {
                handleTopLevelExpression();
                break;
            }
        }
    }
}

void Parser::handleDefinition() {
    if (parseDefinition()) {
        std::cerr << "Parsed a function definition." << std::endl;
    } else {
        // Skip token for error recovery.
        consume();
    }
}

void Parser::handleExtern() {
    if (parseExternal()) {
        std::cerr << "Parsed an extern." << std::endl;
    } else {
        // Skip token for error recovery.
        consume();
    }
}

void Parser::handleTopLevelExpression() {
    if (parseTopLevelFunction()) {
        std::cerr << "Parsed a top-level expression." << std::endl;
    } else {
        // Skip token for error recovery.
        consume();
    }
}

int Parser::consume() {
    current_token_ = lexer_.getToken();
    return current_token_;
}

int Parser::precedence(int token) {
    if (!isascii(token)) {
        return -1;
    }
    int precedence = precedence_map_[(char) token];
    return (precedence > 0) ? precedence : -1;
}

std::unique_ptr<AST::Expression> Parser::error(const std::string &str) {
    std::cerr << "Error: " << str << std::endl;
    return nullptr;
}

std::unique_ptr<AST::Prototype> Parser::errorP(const std::string &str) {
    error(str);
    return nullptr;
}

// number-expr := number
std::unique_ptr<AST::Expression> Parser::parseNumberExpression() {
    auto result = std::make_unique<AST::Number>(lexer_.getNumber());
    consume();
    return result;
}

// paren-expr := '(' expression ')'
std::unique_ptr<AST::Expression> Parser::parseParenExpression() {
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
std::unique_ptr<AST::Expression> Parser::parseIdentifierExpression() {
    std::string identifier = lexer_.getIdentifier();
    consume(); // identifier token
    if ((char)current_token_ != '(') {
        return std::make_unique<AST::Variable>(identifier);
    }

    consume(); // '(' token
    std::vector<std::unique_ptr<AST::Expression>> arguments;
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

// primary-exp := identifier-expr
//             := number-expr
//             := paren-expr
std::unique_ptr<AST::Expression> Parser::parsePrimary() {
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

// expression := primary-exp bin-op-rhs
std::unique_ptr<AST::Expression> Parser::parseExpression() {
    auto lhs = parsePrimary();
    if (!lhs) {
        return nullptr;
    }
    return parseBinOpRhs(0, std::move(lhs));
}

// bin-op-rhs := (binOp primary)*
std::unique_ptr<AST::Expression> Parser::parseBinOpRhs(int expPrecedence, std::unique_ptr<AST::Expression> lhs) {
    while (true) {
        int tokenPrecedence = precedence(current_token_);

        if (tokenPrecedence < expPrecedence) {
            return lhs;
        }
        int binOp = current_token_;
        consume(); // binOp token
        auto rhs = parsePrimary();
        if (!rhs) {
            return nullptr;
        }
        int nextTokenPrecedence = precedence(current_token_);
        if (tokenPrecedence < nextTokenPrecedence) {
            rhs = parseBinOpRhs(tokenPrecedence + 1, std::move(rhs));
            if (!rhs) {
                return nullptr;
            }
        }
        lhs = std::make_unique<AST::BinaryOp>(binOp, std::move(lhs), std::move(rhs));
    }
}

// prototype := identifier '(' identifier* ')'
std::unique_ptr<AST::Prototype> Parser::parsePrototype() {
    if (current_token_ != Lexer::IDENTIFIER) {
        return errorP("Expected function name in prototype");
    }

    std::string functionName = lexer_.getIdentifier();
    consume(); // identifier token
    if (current_token_ != '(') {
        return errorP("Expected '(' name in prototype");
    }

    std::vector<std::string> arguments;
    while (consume() == Lexer::IDENTIFIER) {
        arguments.push_back(lexer_.getIdentifier());
    }

    if (current_token_ != ')') {
        return errorP("Expected ')' name in prototype");
    }
    consume(); // ')' token

    return std::make_unique<AST::Prototype>(functionName, std::move(arguments));
}

// external := 'extern' prototype
std::unique_ptr<AST::Prototype> Parser::parseExternal() {
    consume(); // 'extern' token
    return parsePrototype();
}

// function := 'def' prototype expression
std::unique_ptr<AST::Function> Parser::parseDefinition() {
    consume(); // 'def' token
    auto prototype = parsePrototype();
    if (!prototype) {
        return nullptr;
    }

    auto expression = parseExpression();
    if (!expression) {
        return nullptr;
    }
    return std::make_unique<AST::Function>(std::move(prototype), std::move(expression));
}

std::unique_ptr<AST::Function> Parser::parseTopLevelFunction() {
    auto expression = parseExpression();
    if (!expression) {
        return nullptr;
    }
    auto prototype = std::make_unique<AST::Prototype>("", std::vector<std::string>());
    return std::make_unique<AST::Function>(std::move(prototype), std::move(expression));
}
