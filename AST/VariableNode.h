#ifndef VARIABLENODE_H
#define VARIABLENODE_H
#include "ExpressionNode.h"
#include "Token.h"

class VariableNode : public ExpressionNode{
    public:
        VariableNode(Token var) : ExpressionNode(var){}
};

#endif // VARIABLENODE_H
