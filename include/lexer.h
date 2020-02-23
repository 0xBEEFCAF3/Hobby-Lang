#pragma once
    
#include "token.h"
#include <stdlib.h>
#include <string>
#include <map>


#ifndef lexer_h
#define lexer_h

class Lexer
{
private:
    
    std::string _text;
    int _pos; //pos into text
    bool _has_error;
    char _current_char;
    std::map<std::string, Token> _reserved_key_words =
     {
        {"BEGIN",   {Token( Type::BEGIN, "BEGIN")}},
        {"END",     {Token(Type::END, "END")}}    
    };
public:
    Lexer();
    Lexer(std::string text);
    Token get_next_token();
    Token _id();
    void error();
    
    int getPos();
    void setPos(int pos);
    std::string getText();
    bool getError();
    void advance();
    void skipWhiteSpace();
    int interger();
    char peek();
    bool isReservedKeyword(std::string);


};
#endif
