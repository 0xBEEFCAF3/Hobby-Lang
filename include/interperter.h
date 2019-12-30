#include "token.h"
#include "lexer.h"
#include <string>


#pragma once

#ifndef interperter_h
#define interperter_h

class Interperter
{
private:
    Token _current_token;
    Lexer _lexer;
public:
    Interperter();
    Interperter(Lexer lexer);
    Token getToken();
    void setToken(Token token);
    int expr();
    int term();
    void eat(Type type);
    int factor();
};
#endif
