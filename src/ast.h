#pragma once

#include <string>
#include <utility>
#include <memory>
#include <vector>

namespace AST {

class Expression {
public:
    virtual ~Expression() = default;
};

class Number : public Expression {
public:
    explicit Number(double value) : value_(value) {}

private:
    double value_;
};

class Variable : public Expression {
public:
    explicit Variable(std::string name) : name_(std::move(name)) {}

private:
    std::string name_;
};

class BinaryOp : public Expression {
public:
    BinaryOp(char op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs) :
            op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

private:
    char op_;
    std::unique_ptr<Expression> lhs_, rhs_;
};

class Call : public Expression {
public:
    Call(std::string callee, std::vector<std::unique_ptr<Expression>> arguments) :
            callee_(std::move(callee)), arguments_(std::move(arguments)) {}

private:
    std::string callee_;
    std::vector<std::unique_ptr<Expression>> arguments_;
};

class Prototype {
public:
    Prototype(std::string name, std::vector<std::string> arguments) :
            name_(std::move(name)), arguments_(std::move(arguments)) {}

    const std::string &getName() const {
        return name_;
    }

private:
    std::string name_;
    std::vector<std::string> arguments_;
};

class Function {
public:
    Function(std::unique_ptr<Prototype> prototype, std::unique_ptr<Expression> body) :
            prototype_(std::move(prototype)), body_(std::move(body)) {}

private:
    std::unique_ptr<Prototype> prototype_;
    std::unique_ptr<Expression> body_;
};

} // AST