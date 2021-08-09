#ifndef LEXER_H
#define LEXER_H
#include <mainwindow.h>
#include "Token.h"
#include <QDebug>
#include <regex>
#include <string.h>
#include <vector>

class Lexer{
    public:
        std::string code;
        int pos = 0;

        Lexer(std::string code){
            this->code = code;
        }
        std::vector<Token> lexAnalisys(){
            TokenType tt[] {
                TokenType("NUMBER", "^[0-9]*"),
                TokenType("VARIABLE", "^[a-z]*"),
                TokenType("SEMICOLON", "^;"),
                TokenType("SPACE", "^[ \\n\\t\\r]"),
                TokenType("ASSIGN", "^\\="),
                TokenType("LOG", "^CONSOLE"),
                TokenType("PLUS", "^\\+"),
                TokenType("MINUS", "^\\-"),
                TokenType("LPAR", "^\\("),
                TokenType("RPAR", "^\\)")
            };
            std::vector<Token> TokenList;
            while(this->nextToken(tt, 10, TokenList));
            return TokenList;            
        }
        bool nextToken(TokenType* ttArray, int length, std::vector<Token> &TokenList){
            if(this->code.length() == 0)
                return false;
            for(int i = 0; i < length; i++){
                TokenType tt = ttArray[i];
                std::regex regular(ttArray[i].regex);
                std::smatch match;
                if(std::regex_search(this->code, match, regular) && match[0].length() != 0){
                    const Token t(tt, match[0], this->pos);
                    if(t.type.name != "SPACE") TokenList.push_back(t);
                    this->pos += match[0].length();
                    this->code = this->code.substr(match[0].length());
                    return true;
                }
            }
            qDebug() << "Error in pos " << this->pos << "\n";
            return false;
        }
};
#endif // LEXER_H
