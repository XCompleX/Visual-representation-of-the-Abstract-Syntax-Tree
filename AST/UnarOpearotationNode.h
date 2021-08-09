#ifndef UNAROPEAROTATIONNODE_H
#define UNAROPEAROTATIONNODE_H

#include "ExpressionNode.h"
#include "Token.h"

class UnarOperationNode : public ExpressionNode{
    public:
        ExpressionNode* operand;

        UnarOperationNode(Token operat, ExpressionNode* operan) : ExpressionNode(operat), operand(operan) {}


};

#endif // UNAROPEAROTATIONNODE_H
