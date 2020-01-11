#include "interperter.h"
#include "token.h"
#include <stdlib.h>
#include <iostream>
#include <string>


AST ::AST(AST *left, Token token, AST *right)
{
    _left = left;
    _token = token;
    _right = right;
}
AST :: AST(Token token){
    _token = token;
}

AST* AST :: getLeft()
{
    return _left;
}
Token AST ::getToken()
{
    return _token;
}
AST* AST ::getRight()
{
    return _right;
}

Interperter::Interperter()
{
}

Interperter ::Interperter(Parser parser)
{
    _parser = parser;
}

int Interperter ::visit(AST *node)
{
    switch (node->getToken().getType())
    {
    case Type::DIV:
        return visit(node->getLeft()) / visit(node->getRight());
    case Type::MUL:
        return visit(node->getLeft()) * visit(node->getRight());
    case Type::PLUS:
        return visit(node->getLeft()) + visit(node->getRight());
    case Type::SUBTRACT:
        return visit(node->getLeft()) - visit(node->getRight());
    case Type::INTERGER:
        return node->getToken().getValue();
    default:
        return -1;
    }
}


int Interperter ::interpert()
{
    AST *tree = _parser.parse();
    return visit(tree);
}
