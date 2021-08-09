#ifndef BINOPERATIONNODE_H
#define BINOPERATIONNODE_H
#include "ExpressionNode.h"
#include "Token.h"

class BinOperationNode : public ExpressionNode{
    public:
        ExpressionNode* leftNode;
        ExpressionNode* rightNode;

        BinOperationNode(Token operat, ExpressionNode* leftN, ExpressionNode* rightN) : ExpressionNode(operat), leftNode(leftN),  rightNode(rightN) {}


};

#endif // BINOPERATIONNODE_H
