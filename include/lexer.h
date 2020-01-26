#pragma once
    
#include "token.h"
#include <stdlib.h>
#include <string>


#ifndef lexer_h
#define lexer_h

class Lexer
{
private:
    
    std::string _text;
    int _pos; //pos into text
    bool _has_error;
    char _current_char;
public:
    Lexer();
    Lexer(std::string text);
    Token get_next_token();
    void error();
    
    int getPos();
    void setPos(int pos);
    std::string getText();
    bool getError();
    void advance();
    void skipWhiteSpace();
    int interger();
};
#endif
