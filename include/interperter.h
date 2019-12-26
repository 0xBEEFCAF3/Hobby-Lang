#include "token.h"
#include <string>

#pragma once

#ifndef interperter_h
#define interperter_h

class Interperter
{
private:
    
    std::string _text;
    int _pos; //pos into text
    Token _current_token;
    bool _has_error;
    char _current_char;
public:
    Interperter(std::string text);
    int expr();
    Token get_next_token();
    void error();
    void eat(Type type);
    Token getToken();
    void setToken(Token token);
    int getPos();
    void setPos(int pos);
    std::string getText();
    bool getError();
    void advance();
    void skipWhiteSpace();
    int interger();
};
#endif
