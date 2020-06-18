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
    PROGRAM,
    BLOCK,
    VARDECL,
    Type,

};



struct ASTNode
{
    ASTNode *_left;
    Token _token;
    ASTNode *_right;
    std::vector<ASTNode> _children;
    ASTNodeType _type;
    std::vector<ASTNode> _declarations;
    ASTNode *_varNode;
    ASTNode *_typeNode;
};
struct ASTProgramNode {
    std::string progName;
    ASTNode blockNode;
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
    struct ASTProgramNode parse();
    struct ASTProgramNode program();
    struct ASTNode compoundStatment();
    std::vector<ASTNode> statmentList();
    struct ASTNode statment();
    struct ASTNode assignmentStatment();
    struct ASTNode variable();
    struct ASTNode empty();
    struct ASTNode block();
    std::vector<ASTNode> declarations();
    std::vector<ASTNode>variableDeclarations();
    struct ASTNode typeSpec();

};

class Interperter
{
private:
    Parser _parser;
    std::map<std::string, float> _globalScope;
    
public:
    Interperter();
    Interperter(Parser parser);
    float visit(ASTNode node);
    int interpert();
    void printGlobalScope();
};

#endif
