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

AST::AST(std::vector<ASTNode> children)
{
    _children = children;
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
    switch (node._type)
    {
    case ASTNodeType::COMPOUND:
        for (ASTNode childNode : node._children)
        {
            visit(childNode);
        }

        return NO_OPERATION;

    case ASTNodeType::VAR:
        /** Check if variable exists */
        if (_globalScope.find(node._token.getStringValue()) == _globalScope.end())
        {
            std::cout << "Key not found" << std::endl;
            return ERROR_NODE_VISIT;
        }
        return _globalScope[node._token.getStringValue()]._token.getValue();

    case ASTNodeType::ASSIGN:
        //TODO add some string checking here
        std::cout << "[INTERPERTER]  assigning in global scope " << std::endl;
        _globalScope[node._left->_token.getStringValue()] = *node._right;
        return NO_OPERATION;
    case ASTNodeType::BINARY:
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
        default:
            return ERROR_NODE_VISIT;
        }
    case ASTNodeType::UNARY:
        switch (node._token.getType())
        {
        case Type::PLUS:
            return visit(*node._right);
        case Type::SUBTRACT:
            return -1 * visit(*node._right);
        default:
            return ERROR_NODE_VISIT;
        }
    case ASTNodeType::VALUE:
        return node._token.getValue();
    case ASTNodeType::NOOP:
        return NO_OPERATION;
    }

    return ERROR_NODE_VISIT;
}

void Interperter::printGlobalScope()
{
    for (auto it = _globalScope.begin(); it != _globalScope.end(); ++it)
    {
        std::cout << it->first << " = ";
        printf("%d\n" , it->second._token.getValue());
    }
}

int Interperter ::interpert()
{
    std::cout << "[INTERPERTER] starting to interpert " << std::endl ;
    ASTNode tree;
    tree = _parser.parse();

    return visit(tree);
}
