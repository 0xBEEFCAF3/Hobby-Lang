#pragma once

#include "token.h"
#include "parser.h"
#include "lexer.h"
#include <string>
#include <vector>

#ifndef interperter_h
#define interperter_h

#define NO_OPERATION 1
#define ERROR_NODE_VISIT -1

enum class ASTNodeType{
    VALUE,
    UNARY,
    BINARY,
    VAR,
    COMPOUND,
    ASSIGN,
    NOOP,
};

struct ASTNode
{
    ASTNode *_left;
    Token _token;
    ASTNode *_right;
    std::vector<ASTNode> _children;
    ASTNodeType _type;
};

class AST
{
private:
    ASTNode *_ASTNode;
    std::vector<ASTNode> _children;

public:
    AST();
    AST(Token token);
    AST(struct ASTNode *node);
    AST(std::vector<ASTNode> root);

    struct ASTNode *getNode();
    void setNode(struct ASTNode *node);
};

class Parser
{
private:
    Token _current_token;
    Lexer _lexer;
    ASTNode _root;

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
    struct ASTNode program();
    struct ASTNode compoundStatment();
    std::vector<ASTNode> statmentList();
    struct ASTNode statment();
    struct ASTNode assignmentStatment();
    struct ASTNode variable();
    struct ASTNode empty();

};

class Interperter
{
private:
    Parser _parser;
    std::map<std::string, ASTNode> _globalScope;
    
public:
    Interperter();
    Interperter(Parser parser);
    int visit(ASTNode node);
    int interpert();
    void printGlobalScope();
};

#endif
