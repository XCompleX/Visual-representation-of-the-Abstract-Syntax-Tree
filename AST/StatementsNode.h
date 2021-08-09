#ifndef STATEMENTSNODE_H
#define STATEMENTSNO
#include "ExpressionNode.h"
#include <string>
#include <vector>
class StatementsNode : public ExpressionNode{
    public:
        std::vector<ExpressionNode*> codeStrings;
        StatementsNode(Token token) : ExpressionNode(token){}

        void addNode(ExpressionNode* node){
            this->codeStrings.push_back(node);
        }
};

#endif // STATEMENTSNODE_H
