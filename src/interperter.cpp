#include "interperter.h"
#include "token.h"
#include <stdlib.h>
#include <iostream>
#include <string>

AST ::AST() {}

AST ::AST(Token token)
{
    _ASTNode->_token = token;
}

AST ::AST(struct ASTNode *node)
{
    _ASTNode = node;
}

struct ASTNode *AST ::getNode()
{
    return _ASTNode;
}

void AST ::setNode(struct ASTNode *node)
{
    _ASTNode = node;
}

Interperter::Interperter()
{
}

Interperter ::Interperter(Parser parser)
{
    _parser = parser;
}

int Interperter ::visit(ASTNode node)
{
    switch (node._token.getType())
    {
    case Type::DIV:
        return visit(*node._left) / visit(*node._right);
    case Type::MUL:
        return visit(*node._left) * visit(*node._right);
    case Type::PLUS:
        return visit(*node._left) + visit(*node._right);
    case Type::SUBTRACT:
        return visit(*node._left) - visit(*node._right);
    case Type::INTERGER:
        return node._token.getValue();
    default:
        return -1;
    }
}

int Interperter ::interpert()
{
    ASTNode tree;
    tree = _parser.parse();

    return visit(tree);
}
