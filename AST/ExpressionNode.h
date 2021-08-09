#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H
#include "Token.h"
#include <vector>
class ExpressionNode{
    public:
        Token token;
        std::string Statementsnode = "null";
        std::vector<std::string> Errors;
        ExpressionNode(Token token) : token(token){}
};

#endif // EXPRESSIONNODE_H
