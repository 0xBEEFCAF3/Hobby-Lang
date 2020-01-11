#pragma once
#include "token.h"
#include "lexer.h"
#include "interperter.h"


#ifndef parser_h
#define parser_h

class Parser
{
private:
    Token _current_token;
    Lexer _lexer;

public:
    Parser();
    Parser(Lexer lexer);
    Token getToken();
    void setToken(Token token);
    AST* expr();
    AST* term();
    void eat(Type type);
    AST* factor();
    AST* parse();
};

#endif
