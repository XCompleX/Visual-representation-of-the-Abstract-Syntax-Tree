#ifndef TOKENTYPE_H
#define TOKENTYPE_H
#include "QString"

class TokenType{
    public:
        std::string name;
        std::string regex;

        TokenType(std::string name, std::string regex){
            this->name = name;
            this->regex = regex;
        }
        TokenType(){

        }
};

#endif // TOKENTYPE_H
