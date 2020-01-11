#include "token.h"
#include "parser.h"
#include <string>
#pragma once

#ifndef interperter_h
#define interperter_h

class AST 
{
private:
    AST *_left;
    Token _token;
    AST *_right;

public:
    AST();
    AST(Token token);
    AST(AST *left, Token op, AST *right);
    AST *getLeft();
    Token getToken();
    AST *getRight();
};

class Interperter
{
private:
    Parser _parser;
public:
    Interperter();
    Interperter(Parser parser);
    int visitBinOp(AST* node);
    int visitNum(AST* node);
    int visit(AST* node);
    int visitNum(AST* node);
    int interpert();
};

#endif
