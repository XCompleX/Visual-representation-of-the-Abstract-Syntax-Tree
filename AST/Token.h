#ifndef TOKEN_H
#define TOKEN_H
#include "QString"
#include "TokenType.h"
#include <string.h>
class Token{
    public:
        TokenType type;
        std::string text;
        int pos;

        Token(TokenType type, std::string text, int pos){
            this->type = type;
            this->text = text;
            this->pos = pos;
        }
};

#endif // TOKEN_H
