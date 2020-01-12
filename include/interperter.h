#pragma once

#include "token.h"
#include "parser.h"
#include "lexer.h"
#include <string>

#ifndef interperter_h
#define interperter_h

struct ASTNode{
    ASTNode *_left;
    Token _token;
    ASTNode *_right;
};


class AST
{
private:
    ASTNode* _ASTNode;
public:
    AST();
    AST(Token token);
    AST(struct ASTNode* node);
    struct ASTNode* getNode();
    void setNode(struct ASTNode* node);
};

class Parser
{
private:
    Token _current_token;
    Lexer _lexer;
    ASTNode  _root;

public:
    Parser();
    Parser(Lexer lexer);
    Token getToken();
    void setToken(Token token);
    void eat(Type type);
    struct ASTNode expr();
    struct ASTNode term();
    struct ASTNode factor();
    struct ASTNode parse();
    AST* getAST();
};

class Interperter
{
private:
    Parser _parser;

public:
    Interperter();
    Interperter(Parser parser);
    int visit(struct ASTNode *node);
    int interpert();
};

#endif
