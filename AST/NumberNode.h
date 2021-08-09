#ifndef NUMBERNODE_H
#define NUMBERNODE_H
#include "ExpressionNode.h"
#include "Token.h"

class NumberNode : public ExpressionNode{
    public:
        NumberNode(Token num) : ExpressionNode(num){}
};

#endif // NUMBERNODE_H
